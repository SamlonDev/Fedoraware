#include "CheaterDetection.h"

#include "../Menu/Playerlist/PlayerUtils.h"
#include "../Logs/Logs.h"

bool CCheaterDetection::ShouldScan()
{
	if (!Vars::CheaterDetection::Methods.Value || I::EngineClient->IsPlayingTimeDemo())
		return false;

	const int iProtFlags = Vars::CheaterDetection::Protections.Value;
	if (iLastScanTick == I::GlobalVars->tickcount && iProtFlags & (1 << 2))
		return false;

	if (flLastFrameTime)
	{
		const float flRealFrameTime = I::GlobalVars->realtime - flLastFrameTime;
		flLastFrameTime = I::GlobalVars->realtime;
		const int iRealFPS = static_cast<int>(1.0f / flRealFrameTime);

		if (iRealFPS < server.iTickRate && iProtFlags & (1 << 1))
			return false;
	}

	if (const INetChannel* NetChannel = I::EngineClient->GetNetChannelInfo())
	{
		const float flLastReceive = NetChannel->GetTimeSinceLastReceived();
		const float flMaxReceive = TICK_INTERVAL * 2;
		const bool bIsTimeOut = NetChannel->IsTimingOut();
		const bool bShouldSkip = (flLastReceive > flMaxReceive) || bIsTimeOut;
		if (bShouldSkip && iProtFlags & (1 << 0))
			return false;
	}

	return true;
}

bool CCheaterDetection::ShouldScanEntity(CBaseEntity* pEntity)
{
	const int iIndex = pEntity->GetIndex();

	if (fabsf(I::GlobalVars->curtime - mData[pEntity].flJoinTime) < 10.f)
		return false; // if they haven't been in the server for 10 seconds dont scan them

	// dont scan invalid players
	if (pEntity->IsAlive() || pEntity->IsAGhost() || pEntity->IsTaunting())
		return false;

	// dont scan players that arent simulated this tick
	if (pEntity->m_flSimulationTime() == pEntity->m_flOldSimulationTime())
		return false;

	// dont scan if we can't get playerinfo
	PlayerInfo_t pi{};
	if (!I::EngineClient->GetPlayerInfo(iIndex, &pi))
		return false;

	// dont scan bots or players already marked as cheaters
	if (pi.fakeplayer || F::PlayerUtils.HasTag(pi.friendsID, "Cheater"))
		return false;

	if (!mData[pEntity].flJoinTime)
	{
		mData[pEntity].flJoinTime = I::GlobalVars->curtime;
		return false;
	}

	return true;
}

bool CCheaterDetection::IsPitchLegal(CBaseEntity* pSuspect)
{
	const Vec3 vAngles = pSuspect->GetEyeAngles();
	return fabsf(vAngles.x) < 90.f;
}

void CCheaterDetection::ReportTickCount(std::pair<CBaseEntity*, int> pReport)
{
	if (Vars::CheaterDetection::Methods.Value & (1 << 3))
	{
		if (mData[pReport.first].pChoke.first = 0)
		{
			mData[pReport.first].pChoke.second = pReport.second;
			return;
		}
		mData[pReport.first].pChoke.first++;
		mData[pReport.first].pChoke.second += pReport.second;
	}
}

bool CCheaterDetection::CheckBHop(CBaseEntity* pEntity)
{
	const bool bOnGround = pEntity->OnSolid(); // NOTE: groundentity isn't networked properly sometimes i think
	if (bOnGround)
		mData[pEntity].pBhop.first++;
	else if (mData[pEntity].pBhop.first <= Vars::CheaterDetection::BHopMaxDelay.Value && mData[pEntity].pBhop.first > 0)
	{
		mData[pEntity].pBhop.second++;
		mData[pEntity].pBhop.first = 0;
	}
	else
		mData[pEntity].pBhop = { 0, 0 };

	if (mData[pEntity].pBhop.second >= Vars::CheaterDetection::BHopDetectionsRequired.Value)
	{
		mData[pEntity].iPlayerSuspicion++;
		mData[pEntity].pBhop = { 0, 0 };
		return true;
	}
	return false;
}

bool CCheaterDetection::AreAnglesSuspicious(CBaseEntity* pEntity)
{
	// first check if we should scan this player at all
	if (G::ChokeMap[pEntity->GetIndex()] > 0 || pEntity->m_vecVelocity().Length() < 10.f || mData[pEntity].vLastAngle.IsZero())
	{
		mData[pEntity].pTrustAngles = { false, {0, 0} }; // angles don't update the way I WANT them to if the player is not moving.
		return false;
	}
	if (!mData[pEntity].pTrustAngles.first)
	{	// we are not suspicious of this player yet
		const Vec3 vCurAngle = pEntity->GetEyeAngles();
		const float flDeltaX = RAD2DEG(Math::AngleDiffRad(DEG2RAD(vCurAngle.x), DEG2RAD(mData[pEntity].vLastAngle.x)));
		const float flDeltaY = RAD2DEG(Math::AngleDiffRad(DEG2RAD(vCurAngle.y), DEG2RAD(mData[pEntity].vLastAngle.y)));
		const float flDelta = sqrtf(pow(flDeltaX, 2) + pow(flDeltaY, 2));

		if (flDelta > (Vars::CheaterDetection::MinimumFlickDistance.Value))
			mData[pEntity].pTrustAngles = { true, {vCurAngle.x, vCurAngle.y} };
	}
	else
	{
		// check for noise on this player (how much their mouse moves after the initial flick)
		const Vec3 vCurAngle = pEntity->GetEyeAngles();
		const float flDeltaX = RAD2DEG(Math::AngleDiffRad(DEG2RAD(vCurAngle.x), DEG2RAD(mData[pEntity].vLastAngle.x)));
		const float flDeltaY = RAD2DEG(Math::AngleDiffRad(DEG2RAD(vCurAngle.y), DEG2RAD(mData[pEntity].vLastAngle.y)));
		const float flDelta = sqrtf(pow(flDeltaX, 2) + pow(flDeltaY, 2));

		if (flDelta < (Vars::CheaterDetection::MaximumNoise.Value * server.flMultiplier))
		{
			mData[pEntity].pTrustAngles = { false, {0, 0} };
			return true;
		}
		else
			mData[pEntity].pTrustAngles = { false, {0, 0} };
	}
	return false;
}

void CCheaterDetection::AimbotCheck(CBaseEntity* pEntity)
{
	if (fabsf(mData[pEntity].flLastAimbotTime - I::GlobalVars->curtime < 1.f))
		return;
	if (!(Vars::CheaterDetection::Methods.Value & (1 << 7)))
		return;

	const Vec3 vCurAngle = pEntity->GetEyeAngles();
	const float flDeltaX = RAD2DEG(Math::AngleDiffRad(DEG2RAD(vCurAngle.x), DEG2RAD(mData[pEntity].vLastAngle.x)));
	const float flDeltaY = RAD2DEG(Math::AngleDiffRad(DEG2RAD(vCurAngle.y), DEG2RAD(mData[pEntity].vLastAngle.y)));
	const float flDelta = sqrtf(pow(flDeltaX, 2) + pow(flDeltaY, 2));
	const float flScaled = std::clamp(flDelta * G::ChokeMap[pEntity->GetIndex()], 0.f, Vars::CheaterDetection::MaxScaledAimbotFoV.Value); // aimbot flick scaled

	if (flScaled > Vars::CheaterDetection::MinimumAimbotFoV.Value)
	{	// this person is aimbotting in our eyes.
		PlayerInfo_t pi{};
		if (!I::EngineClient->GetPlayerInfo(pEntity->GetIndex(), &pi))
			return;

		mData[pEntity].iPlayerSuspicion++;
		F::Logs.CheatDetection(pi.name, "infracted", "aimbot");
		mData[pEntity].flLastAimbotTime = I::GlobalVars->curtime;
	}
}

bool CCheaterDetection::IsDuckSpeed(CBaseEntity* pEntity)
{
	const bool bIsDucking = pEntity->IsDucking() && pEntity->OnSolid(); // this may break on movement sim?? (ty spook >:C)
	if (!bIsDucking)
	{
		mData[pEntity].pDuckInfo = { 0, 0 };
		return false;
	}
	if (mData[pEntity].pDuckInfo.first == -1)
		return false; // dont double infract

	if (TICKS_TO_TIME(mData[pEntity].pDuckInfo.first) > 1.f)
	{	// only check once we have a full second of data.
		const float flAvg = mData[pEntity].pDuckInfo.second / mData[pEntity].pDuckInfo.first;
		if (flAvg > pEntity->m_flMaxspeed() * 0.5f)
		{	// this is meant to be exactly 1/3rd but since we're averaging we should be nicer...
			mData[pEntity].pDuckInfo.first = -1;
			return true;
		}
	}
	else
	{
		mData[pEntity].pDuckInfo.first++;
		mData[pEntity].pDuckInfo.second += pEntity->m_vecVelocity().Length2D();
	}
	return false;
}

void CCheaterDetection::BacktrackCheck(CGameEvent* pEvent)
{
	// get victim & attacker
	CBaseCombatWeapon* pWeapon = reinterpret_cast<CBaseCombatWeapon*>(I::ClientEntityList->GetClientEntityFromHandle(pEvent->GetInt("weaponid")));
	if (Utils::GetWeaponType(pWeapon) == EWeaponType::PROJECTILE || Utils::GetWeaponType(pWeapon) == EWeaponType::UNKNOWN)
		return; // check that our attacker has lag compensation on their weapon.

	CBaseEntity* pAttacker = I::ClientEntityList->GetClientEntity(pEvent->GetInt("attacker"));
	CBaseEntity* pVictim = I::ClientEntityList->GetClientEntity(pEvent->GetInt("userid"));
	if (!pVictim || !pAttacker)
		return;
	if (pVictim->GetDormant() || pAttacker->GetDormant())
		return;
	if (pVictim->m_vecVelocity().Length() < 16.f)
		return; // don't check static players

	const Vec3 vEyeAng = pAttacker->GetEyeAngles(); // get attacker angles & pos
	const Vec3 vEyePos = pAttacker->GetEyePosition();

	//const float flAttackerLatency = 0.f; // TODO: Get player latencies every 30 seconds from the server somehow :thinking:

	Vec3 vForward = {};
	Math::AngleVectors(vEyeAng, &vForward); // create a fwd vector for the attacker

	const Vec3 vTraceStart = vEyePos;
	const Vec3 vTraceEnd = (vTraceStart + (vForward * 8192.0f));
	
	CGameTrace trace = {};
	CTraceFilterHitscan filter = {};
	filter.pSkip = pAttacker;

	const Vec3 vRestorePos = pVictim->m_vecOrigin();
	float flTime = 0.f;

	const auto& pRecords = F::Backtrack.GetRecords(pVictim);
	if (!pRecords)
		return;

	for (const auto& pTick : *pRecords)
	{
		pVictim->m_vecOrigin() = pTick.vOrigin;
		Utils::Trace(vTraceStart, vTraceEnd, (MASK_SHOT), &filter, &trace); // do a trace along the players view angle
		if (trace.entity == pVictim)
		{	// we hit a player with the trace and therefor have the time that the attacker was lag comped to
			flTime = pTick.flCreateTime;
			break;
		}
	}

	pVictim->m_vecOrigin() = vRestorePos; // restore old player origin

	if (flTime == 0)
		return;

	const float flDelta = I::GlobalVars->curtime - flTime; // get delta between attackers shot and servers time
	switch (mData[pAttacker].iLagCompChecks)
	{
	case 0: mData[pAttacker].iOrigDelta += flDelta; mData[pAttacker].iLagCompChecks++; return;
	case 1: mData[pAttacker].iOrigDelta += flDelta; mData[pAttacker].iOrigDelta /= 2; mData[pAttacker].iLagCompChecks++; return;
	}

	if (flDelta > mData[pAttacker].iOrigDelta + TICKS_TO_TIME(2))
	{
		// infract?
	}
}

void CCheaterDetection::SimTime(CBaseEntity* pEntity)
{
	if (Vars::CheaterDetection::Methods.Value & ~(1 << 2))
	{
		if (mData[pEntity].iNonDormantCleanQueries < 6)
			return;
		const float flSimDelta = pEntity->m_flSimulationTime() - pEntity->m_flOldSimulationTime();
		const int iTickDelta = TIME_TO_TICKS(flSimDelta);
		if (mData[pEntity].pChoke.first = 0)
		{
			mData[pEntity].pChoke.second = iTickDelta;
			return;
		}
		mData[pEntity].pChoke.first++;
		mData[pEntity].pChoke.second += iTickDelta;
	}
	return;
}

void CCheaterDetection::OnDormancy(CBaseEntity* pEntity)
{
	mData[pEntity].pTrustAngles = { false, {0, 0} };
	mData[pEntity].iNonDormantCleanQueries = 0;
	mData[pEntity].vLastAngle = {};
	mData[pEntity].pBhop = { false, 0 };
	mFired[pEntity] = false;
}

void CCheaterDetection::OnTick()
{
	const auto pLocal = g_EntityCache.GetLocal();
	if (!pLocal || !I::EngineClient->IsConnected() || G::DoubleTap || !ShouldScan())
		return;

	iLastScanTick = I::GlobalVars->tickcount;
	flScanningTime = I::GlobalVars->curtime - flFirstScanTime;
	FindScores();
	FindHitchances();

	const int iDetectionFlags = Vars::CheaterDetection::Methods.Value;

	for (int n = 1; n <= I::EngineClient->GetMaxClients(); n++)
	{
		CBaseEntity* pEntity = I::ClientEntityList->GetClientEntity(n);
		if (!pEntity || !pEntity->IsPlayer())
			continue;

		if (pEntity->GetDormant())
		{
			OnDormancy(pEntity);
			continue;
		}
		if (!ShouldScanEntity(pEntity))
		{
			OnDormancy(pEntity);
			continue;
		}
		if (pEntity == pLocal) // i think for this code to run the local player has to be cheating anyway :thinking:
			continue;

		PlayerInfo_t pi{};
		if (!I::EngineClient->GetPlayerInfo(pEntity->GetIndex(), &pi))
			continue;

		CalculateHitChance(pEntity);
		SimTime(pEntity);

		if (iDetectionFlags & (1 << 6) && !IsPitchLegal(pEntity))
		{
			mData[pEntity].iPlayerSuspicion = Vars::CheaterDetection::SuspicionGate.Value;
			F::Logs.CheatDetection(pi.name, "marked", "OOB angles");
		}
		if (iDetectionFlags & (1 << 5) && AreAnglesSuspicious(pEntity))
		{
			mData[pEntity].iPlayerSuspicion++;
			F::Logs.CheatDetection(pi.name, "infracted", "suspicious angles");
		}
		if (iDetectionFlags & (1 << 4) && CheckBHop(pEntity))
		{
			mData[pEntity].iPlayerSuspicion++;
			F::Logs.CheatDetection(pi.name, "infracted", "bunny-hopping");
		}
		if (iDetectionFlags & (1 << 8) && IsDuckSpeed(pEntity))
		{
			mData[pEntity].iPlayerSuspicion++;
			F::Logs.CheatDetection(pi.name, "infracted", "suspected duck speed");
		}

		// analytical analysis
		if (mData[pEntity].pChoke.second && mData[pEntity].pChoke.first)
		{
			if (((float)mData[pEntity].pChoke.second / (float)mData[pEntity].pChoke.first) > (Vars::CheaterDetection::PacketManipGate.Value / server.flMultiplier) && mData[pEntity].pChoke.first > 10)
			{
				mData[pEntity].iPlayerSuspicion++;
				const float chocked = mData[pEntity].pChoke.second;
				const float ratio = (14.f / server.flMultiplier);
				F::Logs.CheatDetection(pi.name, "infracted", std::format("high avg packet choking [{:.1f} / {:.1f}]", chocked, ratio));
				mData[pEntity].pChoke = { 0, 0.f };
			}
		}

		if (flScanningTime > 60.f)
		{
			if (iDetectionFlags & (1 << 0) && mData[pEntity].flHitchance && server.flHighAccuracy && mData[pEntity].pShots.first > 25)
			{
				if (mData[pEntity].flHitchance > (server.flHighAccuracy) && !mData[pEntity].pDetections.first)
				{
					mData[pEntity].iPlayerSuspicion += 5;
					F::Logs.CheatDetection(pi.name, "infracted", std::format("extremely high accuracy {{{:.1f} / {:.1f}}}", mData[pEntity].flHitchance, server.flHighAccuracy));
				}
			}
			if (iDetectionFlags & (1 << 1) && mData[pEntity].flScorePerSecond && server.flAverageScorePerSecond)
			{
				if (mData[pEntity].flScorePerSecond > (std::max(server.flAverageScorePerSecond, server.flFloorScore) * Vars::CheaterDetection::ScoreMultiplier.Value) && !mData[pEntity].pDetections.second)
				{
					mData[pEntity].iPlayerSuspicion += 5;
					F::Logs.CheatDetection(pi.name, "infracted", std::format("extremely high score per second {{{:.1f} / {:.1f}}}", mData[pEntity].flScorePerSecond, server.flAverageScorePerSecond));
				}
			}
		}

		const Vec3 vAngles = pEntity->GetEyeAngles();
		mData[pEntity].vLastAngle = { vAngles.x, vAngles.y };
		if (mData[pEntity].iPlayerSuspicion >= Vars::CheaterDetection::SuspicionGate.Value)
		{
			mData[pEntity].iPlayerSuspicion = 0;
			F::PlayerUtils.AddTag(pi.friendsID, "Cheater", true, pi.name);
		}
	}
}

void CCheaterDetection::FillServerInfo()
{
	server.flAverageScorePerSecond = 0.f;
	server.iTickRate = 1.f / TICK_INTERVAL;
	server.flMultiplier = 66.7 / server.iTickRate;
	//Utils::ConLog("CheaterDetection[UTIL]", std::format("Calculated server tickrate & created appropriate multiplier {{{:.1f} / {:.1f}}}", server.iTickRate, server.flMultiplier).c_str(), { 224, 255, 131, 255 }, Vars::Debug::Logging.Value);
}

void CCheaterDetection::FindScores()
{	// runs every 30 seconds
	if (I::GlobalVars->curtime - flLastScoreTime < 30.f)
		return;

	CTFPlayerResource* cResource = g_EntityCache.GetPR();
	if (!cResource)
		return;
	flLastScoreTime = I::GlobalVars->curtime;

	float flTotalAvg = 0;
	int iTotalPlayers = 0;
	for (int n = 1; n <= I::EngineClient->GetMaxClients(); n++)
	{
		CBaseEntity* pEntity = I::ClientEntityList->GetClientEntity(n);
		if (!pEntity || !cResource->GetValid(pEntity->GetIndex()))
			continue;

		const float iScore = cResource->GetScore(pEntity->GetIndex()) / cResource->GetConnectionTime(pEntity->GetIndex());
		flTotalAvg += iScore; iTotalPlayers++; // used for calculating the average score
	}

	if (!flTotalAvg || !iTotalPlayers)
		return;

	// now that we've gone through all players (including local) find the avg
	server.flAverageScorePerSecond = (flTotalAvg / (float)iTotalPlayers);
	//Utils::ConLog("CheaterDetection[UTIL]", std::format("Calculated average server score per second at {:.1f}", server.flAverageScorePerSecond).c_str(), { 224, 255, 131, 255 }, Vars::Debug::Logging.Value);
}

void CCheaterDetection::FindHitchances()
{	// runs every 30 seconds
	if (I::GlobalVars->curtime - flLastAccuracyTime < 30.f || !server.iHits || !server.iMisses)
		return;
	flLastAccuracyTime = I::GlobalVars->curtime;

	const float flAvg = (float)server.iHits / (float)server.iMisses;
	server.flHighAccuracy = std::clamp(flAvg * 2, .001f, .95f);

	//Utils::ConLog("CheaterDetection[UTIL]", std::format("Calculated server hitchance data {{{:.5f} / {:.5f}}}", flAvg, server.flHighAccuracy).c_str(), { 224, 255, 131, 255 }, Vars::Debug::Logging.Value);
}

void CCheaterDetection::Reset()
{
	server = ServerInfo{};
	mData.clear();
	mFired.clear();
	iLastScanTick = 0;
	flLastFrameTime = 0.f;
	flFirstScanTime = 0.f;
	flScanningTime = 0.f;
	flLastScoreTime = 0.f;
	flLastAccuracyTime = 0.f;
}

void CCheaterDetection::OnLoad()
{
	Reset();
	FillServerInfo();
	flLastAccuracyTime = I::GlobalVars->curtime; flLastScoreTime = I::GlobalVars->curtime;
}

void CCheaterDetection::CalculateHitChance(CBaseEntity* pEntity)
{
	mData[pEntity].flHitchance = (float)mData[pEntity].pShots.first / (float)mData[pEntity].pShots.second;
}

void CCheaterDetection::ReportShot(int iIndex)
{
	CBaseEntity* pEntity = I::ClientEntityList->GetClientEntity(iIndex);
	if (!pEntity)
		return;
	mData[pEntity].pShots.second++;
	server.iMisses++;
}

void CCheaterDetection::ReportDamage(CGameEvent* pEvent)
{
	const int userid = pEvent->GetInt("userid");

	const int index = I::EngineClient->GetPlayerForUserID(userid);
	if (index == I::EngineClient->GetLocalPlayer())
		return;

	CBaseEntity* pEntity = I::ClientEntityList->GetClientEntity(index);
	if (!pEntity)
		return;

	if (pEntity->GetDormant())
		return;

	CBaseCombatWeapon* pWeapon = pEntity->GetActiveWeapon();
	if (!pWeapon)
		return;

	AimbotCheck(pEntity);
	//BacktrackCheck(pEvent);
	if (I::GlobalVars->tickcount - mData[pEntity].iLastDamageEventTick <= 1)
		return;

	mData[pEntity].iLastDamageEventTick = I::GlobalVars->tickcount;
	mData[pEntity].pShots.first++; mData[pEntity].bDidDamage = true;
	server.iHits++;
}
