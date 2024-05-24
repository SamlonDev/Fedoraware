#include "NoSpreadHitscan.h"
#include "../../Vars.h"
#include <regex>

void CNoSpreadHitscan::Reset(bool bResetPrint)
{
	bWaitingForPlayerPerf = false;
	flServerTime = 0.f;
	flFloatTimeDelta = 0.f;

	iSeed = 0;
	flMantissaStep = 0;

	bSynced = false;
	if (bResetPrint)
		iBestSync = 0;
}

bool CNoSpreadHitscan::ShouldRun(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, bool bCreateMove)
{
	if (G::CurWeaponType != EWeaponType::HITSCAN)
		return false;

	if (pWeapon->GetWeaponSpread() <= 0.f)
		return false;

	return bCreateMove ? G::IsAttacking : true;
}

int CNoSpreadHitscan::GetSeed(CUserCmd* pCmd)
{
	static auto sv_usercmd_custom_random_seed = g_ConVars.FindVar("sv_usercmd_custom_random_seed");
	if (sv_usercmd_custom_random_seed ? sv_usercmd_custom_random_seed->GetBool() : true)
	{
		const float flFloatTime = float(Utils::PlatFloatTime()) + flFloatTimeDelta;
		//Utils::ConLog("SeedPrediction", std::format("{}\n", flFloatTime).c_str());

		const float flTime = (flFloatTime) * 1000;
		return std::bit_cast<int32_t>(flTime) & 255;
	}
	else
		return pCmd->random_seed; // i don't think this is right
}

float CNoSpreadHitscan::CalcMantissaStep(float val)
{
	// Calculate the delta to the next representable value
	const float nextValue = std::nextafter(val, std::numeric_limits<float>::infinity());
	const float mantissaStep = (nextValue - val) * 1000;

	// Get the closest mantissa (next power of 2)
	return powf(2, ceilf(logf(mantissaStep) / logf(2)));
}

std::string CNoSpreadHitscan::GetFormat(int m_ServerTime)
{
	const int iDays = m_ServerTime / 86400;
	const int iHours = m_ServerTime / 3600 % 24;
	const int iMinutes = m_ServerTime / 60 % 60;
	const int iSeconds = m_ServerTime % 60;

	if (iDays)
		return std::format("{}d {}h", iDays, iHours);
	else if (iHours)
		return std::format("{}h {}m", iHours, iMinutes);
	else
		return std::format("{}m {}s", iMinutes, iSeconds);
}

void CNoSpreadHitscan::AskForPlayerPerf()
{
	if (!Vars::Aimbot::General::NoSpread.Value)
		return Reset();

	static Timer playerperfTimer{};
	if (playerperfTimer.Run(50) && !bWaitingForPlayerPerf)
	{
		I::EngineClient->ClientCmd_Unrestricted("playerperf");
		bWaitingForPlayerPerf = true;
	}
}

bool CNoSpreadHitscan::ParsePlayerPerf(bf_read& msgData)
{
	if (!Vars::Aimbot::General::NoSpread.Value)
		return false;

	char rawMsg[256] = {};

	msgData.ReadString(rawMsg, sizeof(rawMsg), true);
	msgData.Seek(0);

	std::string msg(rawMsg);
	msg.erase(msg.begin());

	std::smatch matches = {};
	std::regex_match(msg, matches, std::regex(R"((\d+.\d+)\s\d+\s\d+\s\d+.\d+\s\d+.\d+\svel\s\d+.\d+)"));

	if (matches.size() == 2)
	{
		bWaitingForPlayerPerf = false;

		// credits to kgb for idea
		const float flNewServerTime = std::stof(matches[1].str());
		if (flNewServerTime < flServerTime)
			return true;

		flMantissaStep = CalcMantissaStep(flNewServerTime);
		const int iSynced = flMantissaStep < 1.f ? 2 : 1;
		bSynced = iSynced == 1;

		flServerTime = flNewServerTime;
		flFloatTimeDelta = flServerTime - float(Utils::PlatFloatTime());

		if (!iBestSync || iBestSync == 2 && bSynced)
		{
			iBestSync = iSynced;
			Utils::ConLog("SeedPrediction", bSynced ? std::format("Synced ({})", flFloatTimeDelta).c_str() : "Not synced, step too low", {127, 0, 255, 255});
			Utils::ConLog("SeedPrediction", std::format("Age {}; Step {}", GetFormat(flServerTime), CalcMantissaStep(flServerTime)).c_str(), { 127, 0, 255, 255 });
		}

		return true;
	}

	return std::regex_match(msg, std::regex(R"(\d+.\d+\s\d+\s\d+)"));
}

void CNoSpreadHitscan::Run(CUserCmd* pCmd, CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	iSeed = GetSeed(pCmd);
	if (!bSynced || !ShouldRun(pLocal, pWeapon, true))
		return;

	// credits to cathook for average spread stuff
	const float flSpread = pWeapon->GetWeaponSpread();
	auto tfWeaponInfo = pWeapon->GetTFWeaponInfo();
	int iBulletsPerShot = tfWeaponInfo ? tfWeaponInfo->GetWeaponData(0).m_nBulletsPerShot : 1;
	iBulletsPerShot = static_cast<int>(Utils::AttribHookValue(static_cast<float>(iBulletsPerShot), "mult_bullets_per_shot", pWeapon));

	std::vector<Vec3> vBulletCorrections = {};
	Vec3 vAverageSpread = {};
	for (int iBullet = 0; iBullet < iBulletsPerShot; iBullet++)
	{
		Utils::RandomSeed(iSeed + iBullet);

		if (!iBullet) // Check if we'll get a guaranteed perfect shot
		{
			const float flTimeSinceLastShot = (pLocal->m_nTickBase() * TICK_INTERVAL) - pWeapon->m_flLastFireTime();

			if ((iBulletsPerShot == 1 && flTimeSinceLastShot > 1.25f) || (iBulletsPerShot > 1 && flTimeSinceLastShot > 0.25f))
				return;
		}

		const float x = Utils::RandomFloat(-0.5f, 0.5f) + Utils::RandomFloat(-0.5f, 0.5f);
		const float y = Utils::RandomFloat(-0.5f, 0.5f) + Utils::RandomFloat(-0.5f, 0.5f);

		Vec3 forward, right, up;
		Math::AngleVectors(pCmd->viewangles, &forward, &right, &up);

		Vec3 vFixedSpread = forward + (right * x * flSpread) + (up * y * flSpread);
		vFixedSpread.Normalize();
		vAverageSpread += vFixedSpread;

		vBulletCorrections.push_back(vFixedSpread);
	}
	vAverageSpread /= static_cast<float>(iBulletsPerShot);

	const auto cFixedSpread = std::ranges::min_element(vBulletCorrections,
		[&](const Vec3& lhs, const Vec3& rhs)
		{
			return lhs.DistTo(vAverageSpread) < rhs.DistTo(vAverageSpread);
		});

	if (cFixedSpread == vBulletCorrections.end())
		return;

	Vec3 vFixedAngles{};
	Math::VectorAngles(*cFixedSpread, vFixedAngles);

	pCmd->viewangles += pCmd->viewangles - vFixedAngles;
	Math::ClampAngles(pCmd->viewangles);

	G::SilentAngles = true;
}