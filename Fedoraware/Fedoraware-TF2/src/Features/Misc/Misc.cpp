#include "Misc.h"

#include "../Vars.h"
#include "../../Utils/Timer/Timer.hpp"
#include "../Aimbot/AimbotGlobal/AimbotGlobal.h"
#include "../Backtrack/Backtrack.h"
#include "../PacketManip/AntiAim/AntiAim.h"
#include "../TickHandler/TickHandler.h"
#include "../Simulation/MovementSimulation/MovementSimulation.h"
#include "../Menu/Playerlist/PlayerUtils.h"

void CMisc::RunPre(CUserCmd* pCmd)
{
	if (const auto& pLocal = g_EntityCache.GetLocal())
	{
		AutoJump(pCmd, pLocal);
		AutoJumpbug(pCmd, pLocal);
		AutoStrafe(pCmd, pLocal);
		AntiBackstab(pCmd, pLocal);
		AutoPeek(pCmd, pLocal);
		AntiAFK(pCmd, pLocal);
		InstantRespawnMVM(pLocal);
	}

	CheatsBypass();
	PingReducer();
	WeaponSway();
	ChatSpam();
}

void CMisc::RunPost(CUserCmd* pCmd, bool pSendPacket)
{
	if (const auto& pLocal = g_EntityCache.GetLocal())
	{
		TauntKartControl(pCmd, pLocal);
		FastStop(pCmd, pLocal);
		FastStrafe(pCmd, pLocal);
		FastAccel(pCmd, pLocal);
		InstaStop(pCmd);
		StopMovement(pCmd, pLocal);
		LegJitter(pCmd, pSendPacket, pLocal);
		MedigunSwitch(pCmd, pLocal);
	}
}

void CMisc::MedigunSwitch(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	if (Vars::Misc::Paste::MedigunSwitch.Value) {


		auto localPlayer = g_EntityCache.GetLocal();
		if (!localPlayer)
		{
			return;
		}

		auto localWeapon = localPlayer->GetActiveWeapon();
		if (!localWeapon)
		{
			return;
		}

		if (localWeapon->GetClassID() == ETFClassID::CWeaponMedigun)
		{
			return;
		}

		I::EngineClient->ClientCmd_Unrestricted("slot2");
	}
}

const std::string SPAM_LBOX[] = {
	"GET GOOD, GET LMAOBOX!",
	"LMAOBOX - WAY TO THE TOP",
	"WWW.LMAOBOX.NET - BEST FREE TF2 HACK!",
	"JOD BLESS TOMAS CURDA!"
};

const std::string SPAM_BP[] = {
	"BigPackets.com, the cutest source cheats provider nyaa~",
	"BigPackets.com UID? fucking NN",
	"BigPackets.com, tapping everyone since 2014"
};

const std::string SPAM_SP[] = {
	"SalmonPaste, best way to win!",
	"SalmonPaste, tapping NNs since... forever!",
	"SalmonPaste, private and good!",
	"SalmonPaste, based on Nitro's src!",
	"SalmonPaste, with followbot!",
	"SalmonPaste, even spam is pasted!"
};

/* Chat & Voicechat Spam */
void CMisc::ChatSpam()
{
	const float flCurTime = I::EngineClient->Time();
	static float flNextSend = 0.0f;

	if (flCurTime > flNextSend)
	{
		// Chat Spam
		if (Vars::Misc::Paste::ChatSpam.Value != 0)
		{
			std::string spamMsg;

			switch (Vars::Misc::Paste::ChatSpam.Value)
			{
			case 2: spamMsg = SPAM_LBOX[Utils::RandIntSimple(0, ARRAYSIZE(SPAM_LBOX) - 1)];
				break;
			case 3: spamMsg = SPAM_BP[Utils::RandIntSimple(0, ARRAYSIZE(SPAM_BP) - 1)];
				break;
			default: spamMsg = SPAM_SP[Utils::RandIntSimple(0, ARRAYSIZE(SPAM_SP) - 1)];
				break;
			}

			Utils::ReplaceSpecials(spamMsg);

			spamMsg.insert(0, "say ");
			I::EngineClient->ClientCmd_Unrestricted(spamMsg.c_str());
		}

		flNextSend = (flCurTime + Vars::Misc::Paste::SpamInterval.Value);
	}
}

void CMisc::AutoJump(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	if (!Vars::Misc::Movement::Bunnyhop.Value || !pLocal->IsAlive() || pLocal->IsAGhost() || pLocal->IsSwimming() || pLocal->IsInBumperKart() || pLocal->MoveType() != MOVETYPE_WALK)
		return;

	const bool bJumpHeld = pCmd->buttons & IN_JUMP;
	const bool bCurHop = bJumpHeld && pLocal->OnSolid();
	static bool bHopping = bCurHop;
	static bool bTried = false;

	if (bCurHop && !bTried)
	{	// this is our initial jump
		bTried = true;
		bHopping = true;
	}
	else if (bCurHop && bTried) 
	{	// we tried and failed to bunnyhop, let go of the key and try again the next tick
		bTried = false;
		pCmd->buttons &= ~IN_JUMP;
	}
	else if (bHopping && bJumpHeld && (!pLocal->OnSolid() || pLocal->IsDucking()))
	{	// we are not on the ground and the key is in the same hold cycle
		bTried = false;
		pCmd->buttons &= ~IN_JUMP;
	}
	else if (bHopping && !bJumpHeld)
	{	// we are no longer in the jump key cycle
		bTried = false;
		bHopping = false;
	}
	else if (!bHopping && bJumpHeld)
	{	// we exited the cycle but now we want back in, don't mess with keys for doublejump, enter us back into the cycle for next tick
		bTried = false;
		bHopping = true;
	}
}

void CMisc::AutoJumpbug(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	if (!Vars::Misc::Movement::AutoJumpbug.Value || !pLocal->IsAlive() || pLocal->IsAGhost() || pLocal->IsSwimming() || pLocal->IsInBumperKart() || pLocal->MoveType() != MOVETYPE_WALK)
		return;

	// don't try if we aren't ducking, on a solid, or won't take fall damage
	if (!(pCmd->buttons & IN_DUCK)
		|| pLocal->OnSolid()
		|| pLocal->m_vecVelocity().z > -650.f)
	{
		return;
	}

	// don't try if we won't be on solid
	{
		PlayerStorage localStorage;
		F::MoveSim.Initialize(pLocal, localStorage, false);
		F::MoveSim.RunTick(localStorage);
		const bool bWillSolid = pLocal->OnSolid();
		F::MoveSim.Restore(localStorage);
		if (bWillSolid)
			return;
	}

	CGameTrace trace;
	CTraceFilterWorldAndPropsOnly filter;
	filter.pSkip = pLocal;

	Vec3 origin = pLocal->m_vecOrigin();
	Utils::TraceHull(origin, origin - Vec3(0, 0, 23), pLocal->m_vecMins(), pLocal->m_vecMaxs(), MASK_PLAYERSOLID, &filter, &trace);

	// don't try if we aren't in range to unduck
	if (!trace.DidHit())
		return;

	// this seems to be the range where this works
	const float flDist = origin.DistTo(trace.vEndPos);
	if (20.f < flDist && flDist < 22.f)
	{
		pCmd->buttons &= ~IN_DUCK;
		pCmd->buttons |= IN_JUMP;
	}
}

void CMisc::AutoStrafe(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	if (!Vars::Misc::Movement::AutoStrafe.Value || !pLocal->IsAlive() || pLocal->IsAGhost() || pLocal->OnSolid() || pLocal->IsSwimming() || pLocal->IsInBumperKart() || pLocal->MoveType() != MOVETYPE_WALK || !(pLocal->m_afButtonLast() & IN_JUMP) && (pCmd->buttons & IN_JUMP) || pLocal->m_nWaterLevel() > static_cast<byte>(WL_Feet))
		return;

	switch (Vars::Misc::Movement::AutoStrafe.Value)
	{
	case 1:
	{
		static auto cl_sidespeed = g_ConVars.FindVar("cl_sidespeed");
		const float flSideSpeed = cl_sidespeed ? cl_sidespeed->GetFloat() : 450.f;

		if (pCmd->mousedx)
		{
			pCmd->forwardmove = 0.f;
			pCmd->sidemove = pCmd->mousedx > 0 ? flSideSpeed : -flSideSpeed;
		}
		break;
	}
	case 2:
	{
		//credits: KGB
		if (!(pCmd->buttons & (IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT)))
			break;

		float flForwardMove = pCmd->forwardmove;
		float flSideMove = pCmd->sidemove;

		Vec3 vForward = {}, vRight = {};
		Math::AngleVectors(pCmd->viewangles, &vForward, &vRight, nullptr);

		vForward.z = vRight.z = 0.f;

		vForward.Normalize();
		vRight.Normalize();

		Vec3 vWishDir = {};
		Math::VectorAngles({ (vForward.x * flForwardMove) + (vRight.x * flSideMove), (vForward.y * flForwardMove) + (vRight.y * flSideMove), 0.f }, vWishDir);

		Vec3 vCurDir = {};
		Math::VectorAngles(pLocal->m_vecVelocity(), vCurDir);

		float flDirDelta = Math::NormalizeAngle(vWishDir.y - vCurDir.y);
		float flTurnScale = Math::RemapValClamped(Vars::Misc::Movement::AutoStrafeTurnScale.Value, 0.f, 1.f, 0.9f, 1.f);
		float flRotation = DEG2RAD((flDirDelta > 0.f ? -90.f : 90.f) + (flDirDelta * flTurnScale));

		float flCosRot = cosf(flRotation);
		float flSinRot = sinf(flRotation);

		pCmd->forwardmove = (flCosRot * flForwardMove) - (flSinRot * flSideMove);
		pCmd->sidemove = (flSinRot * flForwardMove) + (flCosRot * flSideMove);
	}
	}
}

void CMisc::AntiBackstab(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	G::AvoidingBackstab = false;
	Vec3 vTargetPos;

	if (!pLocal->IsAlive() || pLocal->IsStunned() || pLocal->IsInBumperKart() || pLocal->IsAGhost() || !Vars::Misc::Automation::AntiBackstab.Value) {
		return;
	}

	const Vec3 vLocalPos = pLocal->GetWorldSpaceCenter();
	CBaseEntity* target = nullptr;

	for (const auto& pEnemy : g_EntityCache.GetGroup(EGroupType::PLAYERS_ENEMIES))
	{
		if (!pEnemy || !pEnemy->IsAlive() || pEnemy->m_iClass() != CLASS_SPY || pEnemy->IsCloaked() || pEnemy->IsAGhost() || pEnemy->m_bFeignDeathReady())
		{
			continue;
		}

		if (CBaseCombatWeapon* pWeapon = pEnemy->GetActiveWeapon())
		{
			if (pWeapon->GetWeaponID() != TF_WEAPON_KNIFE) { continue; }
		}

		PlayerInfo_t pi{};
		if (I::EngineClient->GetPlayerInfo(pEnemy->GetIndex(), &pi) && F::PlayerUtils.IsIgnored(pi.friendsID))
			continue;

		const Vec3 vEnemyPos = pEnemy->GetShootPos();
		if (!Utils::VisPos(pLocal, pEnemy, vLocalPos, vEnemyPos)) { continue; }
		if (!target && vLocalPos.DistTo(vEnemyPos) < 150.f)
		{
			target = pEnemy;
			vTargetPos = target->GetShootPos();
		}
		else if (vLocalPos.DistTo(vEnemyPos) < vLocalPos.DistTo(vTargetPos) && vLocalPos.DistTo(vEnemyPos) < 150.f)
		{
			target = pEnemy;
			vTargetPos = target->GetShootPos();
		}
	}

	if (target)
	{
		vTargetPos = target->GetShootPos();
		const Vec3 vAngleToSpy = Math::CalcAngle(vLocalPos, vTargetPos);
		G::AvoidingBackstab = true;
		Utils::FixMovement(pCmd, vAngleToSpy);
		pCmd->viewangles = vAngleToSpy;
	}
}

void CMisc::AutoPeek(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	static bool bPosPlaced = false;
	static bool bReturning = false;

	if (Vars::CL_Move::AutoPeek.Value && pLocal->IsAlive() && !pLocal->IsAGhost())
	{
		const Vec3 localPos = pLocal->GetAbsOrigin();

		// We just started peeking. Save the return position!
		if (!bPosPlaced)
		{
			if (pLocal->OnSolid())
			{
				vPeekReturnPos = localPos;
				bPosPlaced = true;
			}
		}
		else
		{
			static Timer particleTimer{};
			if (particleTimer.Run(700))
				Particles::DispatchParticleEffect("ping_circle", vPeekReturnPos, {});
		}

		// We've just attacked. Let's return!
		if (G::LastUserCmd->buttons & IN_ATTACK || G::IsAttacking)
			bReturning = true;

		if (bReturning)
		{
			if (localPos.DistTo(vPeekReturnPos) < 7.f)
			{
				bReturning = false;
				return;
			}

			Utils::WalkTo(pCmd, pLocal, vPeekReturnPos);
		}
	}
	else
	{
		bPosPlaced = bReturning = false;
		vPeekReturnPos = Vec3();
	}
}

void CMisc::AntiAFK(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	if (Vars::Misc::Automation::AntiAFK.Value && g_ConVars.afkTimer->GetInt() != 0)
	{
		if (pCmd->command_number % 2)
		{
			pCmd->buttons |= 1 << 27;
		}
	}
}

void CMisc::InstantRespawnMVM(CBaseEntity* pLocal)
{
	if (I::EngineClient->IsInGame() && pLocal->IsAlive() && Vars::Misc::MannVsMachine::InstantRespawn.Value)
	{
		auto kv = new KeyValues("MVM_Revive_Response");
		kv->SetInt("accepted", 1);
		I::EngineClient->ServerCmdKeyValues(kv);
	}
}

void CMisc::CheatsBypass()
{
	static bool bCheatSet = false;
	static auto sv_cheats = g_ConVars.FindVar("sv_cheats");
	if (sv_cheats)
	{
		if (Vars::Misc::Exploits::CheatsBypass.Value)
		{
			sv_cheats->m_Value.m_nValue = 1;
			bCheatSet = true;
		}
		else if (bCheatSet)
		{
			sv_cheats->m_Value.m_nValue = 0;
			bCheatSet = false;
		}
	}
}

void CMisc::Event(CGameEvent* pEvent, FNV1A_t uNameHash)
{
	if (uNameHash == FNV1A::HashConst("teamplay_round_start") || uNameHash == FNV1A::HashConst("client_disconnect") ||
		uNameHash == FNV1A::HashConst("client_beginconnect") || uNameHash == FNV1A::HashConst("game_newmap"))
	{
		iLastCmdrate = -1;
		F::Backtrack.flWishInterp = 0.f;

		G::BulletsStorage.clear();
		G::BoxesStorage.clear();
		G::LinesStorage.clear();
	}
}

void CMisc::PingReducer()
{
	CNetChannel* netChannel = I::EngineClient->GetNetChannelInfo();
	if (!netChannel)
		return;

	static auto cl_cmdrate = g_ConVars.FindVar("cl_cmdrate");
	const int iCmdRate = cl_cmdrate ? cl_cmdrate->GetInt() : 66;

	static Timer updateRateTimer{};
	if (updateRateTimer.Run(100))
	{
		const int iTarget = Vars::Misc::Exploits::PingReducer.Value ? Vars::Misc::Exploits::PingTarget.Value : iCmdRate;
		if (iTarget == iLastCmdrate)
			return;
		iLastCmdrate = iTarget;

		Utils::ConLog("SendNetMsg", std::format("cl_cmdrate: {}", iTarget).c_str(), { 224, 255, 131, 255 }, Vars::Debug::Logging.Value);

		NET_SetConVar cmd("cl_cmdrate", std::to_string(iTarget).c_str());
		netChannel->SendNetMsg(cmd);
	}
}


void CMisc::WeaponSway()
{
	static auto cl_wpn_sway_interp = g_ConVars.FindVar("cl_wpn_sway_interp");
	static auto cl_wpn_sway_scale = g_ConVars.FindVar("cl_wpn_sway_scale");
	if (cl_wpn_sway_interp)
		cl_wpn_sway_interp->SetValue(Vars::Visuals::Viewmodel::Sway.Value ? Vars::Visuals::Viewmodel::SwayInterp.Value : 0.f);
	if (cl_wpn_sway_scale)
		cl_wpn_sway_scale->SetValue(Vars::Visuals::Viewmodel::Sway.Value ? Vars::Visuals::Viewmodel::SwayScale.Value : 0.f);
}



void CMisc::FastStop(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	if (!Vars::Misc::Movement::FastStop.Value
		|| !pLocal->IsAlive() || pLocal->IsAGhost() || !pLocal->OnSolid() || pLocal->IsSwimming() || pLocal->IsCharging() || pLocal->IsInBumperKart() || pLocal->MoveType() != MOVETYPE_WALK
		|| pCmd->buttons & (IN_JUMP | IN_MOVELEFT | IN_MOVERIGHT | IN_FORWARD | IN_BACK))
	{
		return;
	}

	const float flSpeed = pLocal->m_vecVelocity().Length2D();
	const float flLimit = 10.f;
	if (flSpeed > flLimit)
	{
		if (G::ShiftedTicks != G::MaxShift && !G::IsAttacking && !G::AntiAim)
			G::ShouldStop = true;
		else
		{
			Vec3 direction = pLocal->m_vecVelocity().toAngle();
			direction.y = pCmd->viewangles.y - direction.y;
			const Vec3 negatedDirection = direction.fromAngle() * -flSpeed;
			pCmd->forwardmove = negatedDirection.x;
			pCmd->sidemove = negatedDirection.y;
		}
	}
}

void CMisc::FastAccel(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	if ((pLocal->IsDucking() ? !Vars::Misc::Movement::CrouchSpeed.Value : !Vars::Misc::Movement::FastAccel.Value) || G::IsAttacking || G::DoubleTap || G::Recharge || G::AntiAim || pCmd->command_number % 2
		|| !pLocal->IsAlive() || pLocal->IsAGhost() || !pLocal->OnSolid() || pLocal->IsSwimming() || pLocal->IsTaunting() || pLocal->IsCharging() || pLocal->MoveType() != MOVETYPE_WALK)
	{
		return;
	}

	const int maxSpeed = std::min(pLocal->m_flMaxspeed() * 0.9f, 520.f) - 10.f;
	const float curSpeed = pLocal->m_vecVelocity().Length2D();
	if (curSpeed > maxSpeed)
		return;

	if (pLocal->m_iClass() == ETFClass::CLASS_HEAVY && pCmd->buttons & IN_ATTACK2 && pLocal->IsDucking())
		return;

	if (!(pCmd->buttons & (IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT)))
		return;

	const Vec3 vecMove(pCmd->forwardmove, pCmd->sidemove, 0.f);
	const float flLength = vecMove.Length();
	Vec3 angMoveReverse;
	Math::VectorAngles(vecMove * -1.f, angMoveReverse);
	pCmd->forwardmove = -flLength;
	pCmd->sidemove = 0.f;
	pCmd->viewangles.y = fmodf(pCmd->viewangles.y - angMoveReverse.y, 360.f);
	pCmd->viewangles.z = 270.f;
	G::PSilentAngles = true;
}

void CMisc::FastStrafe(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	if (!Vars::Misc::Movement::FastStrafe.Value || G::IsAttacking
		|| !pLocal->IsAlive() || pLocal->IsAGhost() || !pLocal->OnSolid() || pLocal->IsSwimming() || pLocal->IsTaunting() || pLocal->IsCharging() || pLocal->MoveType() != MOVETYPE_WALK)
	{
		return;
	}

	static bool bFwd = pCmd->forwardmove > 0;
	static bool bSde = pCmd->sidemove > 0;
	const bool bCurFwd = pCmd->forwardmove > 0;
	const bool bCurSde = pCmd->sidemove > 0;

	bool bChanged = false;
	if (fabsf(pCmd->sidemove) > 400)
	{
		if (bSde != bCurSde)
		{
			pCmd->viewangles.x = 90.f;
			pCmd->viewangles.y += bSde ? -90.f : 90.f;
			pCmd->sidemove = bSde ? -pCmd->forwardmove : pCmd->forwardmove;

			G::PSilentAngles = bChanged = true;
		}

		bSde = bCurSde;
		if (bChanged)
			return;
	}
	if (fabsf(pCmd->forwardmove) > 400)
	{
		if (bFwd != bCurFwd)
		{
			pCmd->viewangles.x = 90.f;
			pCmd->viewangles.y += bFwd ? 0.f : 180.f;
			pCmd->sidemove *= bFwd ? 1 : -1;

			G::PSilentAngles = bChanged = true;
		}

		bFwd = bCurFwd;
		if (bChanged)
			return;
	}
}

void CMisc::InstaStop(CUserCmd* pCmd)
{
	if (!G::ShouldStop)
		return;

	Utils::StopMovement(pCmd);
	if (G::ShouldStop)
		return;

	if (G::Recharge || G::DoubleTap)
		G::SilentAngles = true;
	else
		G::PSilentAngles = true;
}

void CMisc::StopMovement(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	if (!pLocal || !pLocal->IsAlive() || pLocal->IsAGhost() || pLocal->IsCharging() || pLocal->IsTaunting() || pLocal->IsStunned())
		return;
	
	static Vec3 vVelocity = {};
	if (G::AntiWarp)
	{
		const int iDoubletapTicks = F::Ticks.GetTicks(pLocal);

		Vec3 angles = {}; Math::VectorAngles(vVelocity, angles);
		angles.y = pCmd->viewangles.y - angles.y;
		Vec3 forward = {}; Math::AngleVectors(angles, &forward);
		forward *= vVelocity.Length();

		if (iDoubletapTicks > std::max(Vars::CL_Move::Doubletap::TickLimit.Value - 8, 3))
		{
			pCmd->forwardmove = -forward.x;
			pCmd->sidemove = -forward.y;
		}
		else if (iDoubletapTicks > 3)
		{
			pCmd->forwardmove = pCmd->sidemove = 0.f;
			pCmd->buttons &= ~(IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT);
		}
		else
		{
			pCmd->forwardmove = forward.x;
			pCmd->sidemove = forward.y;
		}
	}
	else
		vVelocity = pLocal->m_vecVelocity();
}

void CMisc::LegJitter(CUserCmd* pCmd, bool pSendPacket, CBaseEntity* pLocal)
{
	if (!F::AntiAim.AntiAimOn() || !pLocal->OnSolid() || pLocal->IsInBumperKart() || pLocal->IsAGhost() || !pLocal->IsAlive()
		|| G::IsAttacking || G::DoubleTap || pSendPacket)
	{
		return;
	}

	static bool pos = true;
	const float scale = pLocal->IsDucking() ? 14.f : 1.f;
	if (pCmd->forwardmove == 0.f && pCmd->sidemove == 0.f && pLocal->m_vecVelocity().Length2D() < 10.f)
	{
		pos ? pCmd->forwardmove = scale : pCmd->forwardmove = -scale;
		pos ? pCmd->sidemove = scale : pCmd->sidemove = -scale;
		pos = !pos;
	}
}

void CMisc::DoubletapPacket(CUserCmd* pCmd, bool* pSendPacket)
{
	if (G::DoubleTap || G::Warp)
	{
		*pSendPacket = G::ShiftedGoal == G::ShiftedTicks;
		if ((G::DoubleTap || pCmd->buttons & IN_ATTACK) && I::ClientState->chokedcommands >= 21)
			*pSendPacket = true;
	}
}



void CMisc::TauntKartControl(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	// Handle Taunt Slide
	if (Vars::Misc::Automation::TauntControl.Value && pLocal->IsTaunting())
	{
		if (pCmd->buttons & IN_FORWARD)
		{
			pCmd->forwardmove = 450.f;
			pCmd->viewangles.x = 0.f;
		}
		if (pCmd->buttons & IN_BACK)
		{
			pCmd->forwardmove = 450.f;
			pCmd->viewangles.x = 91.f;
		}
		if (pCmd->buttons & IN_MOVELEFT)
			pCmd->sidemove = -450.f;
		if (pCmd->buttons & IN_MOVERIGHT)
			pCmd->sidemove = 450.f;

		if (!(pCmd->buttons & (IN_MOVELEFT | IN_MOVERIGHT | IN_FORWARD | IN_BACK)))
			pCmd->viewangles.x = 90.f;

		Vec3 vAngle = I::EngineClient->GetViewAngles();
		pCmd->viewangles.y = vAngle.y;

		G::SilentAngles = true;
	}
	else if (Vars::Misc::Automation::KartControl.Value && pLocal->IsInBumperKart())
	{
		const bool bForward = pCmd->buttons & IN_FORWARD;
		const bool bBack = pCmd->buttons & IN_BACK;
		const bool bLeft = pCmd->buttons & IN_MOVELEFT;
		const bool bRight = pCmd->buttons & IN_MOVERIGHT;

		const bool flipVar = pCmd->command_number % 2;
		if (bForward && (!bLeft && !bRight || !flipVar))
		{
			pCmd->forwardmove = 450.f;
			pCmd->viewangles.x = 0.f;
		}
		else if (bBack && (!bLeft && !bRight || !flipVar))
		{
			pCmd->forwardmove = 450.f;
			pCmd->viewangles.x = 91.f;
		}
		else if (pCmd->buttons & (IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT))
		{
			if (flipVar)
			{	// you could just do this if you didn't care about viewangles
				const Vec3 vecMove(pCmd->forwardmove, pCmd->sidemove, 0.f);
				const float flLength = vecMove.Length();
				Vec3 angMoveReverse;
				Math::VectorAngles(vecMove * -1.f, angMoveReverse);
				pCmd->forwardmove = -flLength;
				pCmd->sidemove = 0.f;
				pCmd->viewangles.y = fmodf(pCmd->viewangles.y - angMoveReverse.y, 360.f);
				pCmd->viewangles.z = 270.f;
				G::PSilentAngles = true;
			}
		}
		else
			pCmd->viewangles.x = 90.f;

		G::SilentAngles = true;
	}
}

void CMisc::UnlockAchievements()
{
	using FN = IAchievementMgr * (*)(void);
	const auto achievementmgr = GetVFunc<FN>(I::EngineClient, 114)();
	if (achievementmgr)
	{
		g_SteamInterfaces.UserStats->RequestCurrentStats();
		for (int i = 0; i < achievementmgr->GetAchievementCount(); i++)
			achievementmgr->AwardAchievement(achievementmgr->GetAchievementByIndex(i)->GetAchievementID());
		g_SteamInterfaces.UserStats->StoreStats();
		g_SteamInterfaces.UserStats->RequestCurrentStats();
	}
}

void CMisc::LockAchievements()
{
	using FN = IAchievementMgr * (*)(void);
	const auto achievementmgr = GetVFunc<FN>(I::EngineClient, 114)();
	if (achievementmgr)
	{
		g_SteamInterfaces.UserStats->RequestCurrentStats();
		for (int i = 0; i < achievementmgr->GetAchievementCount(); i++)
			g_SteamInterfaces.UserStats->ClearAchievement(achievementmgr->GetAchievementByIndex(i)->GetName());
		g_SteamInterfaces.UserStats->StoreStats();
		g_SteamInterfaces.UserStats->RequestCurrentStats();
	}
}

void CMisc::SteamRPC()
{
	if (!Vars::Misc::Steam::EnableRPC.Value)
	{
		if (!bSteamCleared) // stupid way to return back to normal rpc
		{
			g_SteamInterfaces.Friends->SetRichPresence("steam_display", ""); // this will only make it say "Team Fortress 2" until the player leaves/joins some server. its bad but its better than making 1000 checks to recreate the original
			bSteamCleared = true;
		}
		return;
	}

	bSteamCleared = false;
	g_SteamInterfaces.Friends->SetRichPresence("steam_display", "#TF_RichPresence_Display");

	/*
	"TF_RichPresence_State_MainMenu"              "Main Menu"
	"TF_RichPresence_State_SearchingGeneric"      "Searching for a Match"
	"TF_RichPresence_State_SearchingMatchGroup"   "Searching - %matchgrouploc_token%"
	"TF_RichPresence_State_PlayingGeneric"        "In Match - %currentmap%"
	"TF_RichPresence_State_LoadingGeneric"        "Joining Match"
	"TF_RichPresence_State_PlayingMatchGroup"     "%matchgrouploc_token% - %currentmap%" <--!!!! used
	"TF_RichPresence_State_LoadingMatchGroup"     "Joining %matchgrouploc_token%"
	"TF_RichPresence_State_PlayingCommunity"      "Community - %currentmap%"
	"TF_RichPresence_State_LoadingCommunity"      "Joining Community Server"
	*/
	if (!I::EngineClient->IsInGame() && Vars::Misc::Steam::OverrideMenu.Value)
		g_SteamInterfaces.Friends->SetRichPresence("state", "MainMenu");
	else
	{
		g_SteamInterfaces.Friends->SetRichPresence("state", "PlayingMatchGroup");

		switch (Vars::Misc::Steam::MatchGroup.Value)
		{
		case 0: g_SteamInterfaces.Friends->SetRichPresence("matchgrouploc", "SpecialEvent"); break;
		case 1: g_SteamInterfaces.Friends->SetRichPresence("matchgrouploc", "MannUp"); break;
		case 2: g_SteamInterfaces.Friends->SetRichPresence("matchgrouploc", "Competitive6v6"); break;
		case 3: g_SteamInterfaces.Friends->SetRichPresence("matchgrouploc", "Casual"); break;
		case 4: g_SteamInterfaces.Friends->SetRichPresence("matchgrouploc", "BootCamp"); break;
		default: g_SteamInterfaces.Friends->SetRichPresence("matchgrouploc", "SpecialEvent"); break;
		}
	}

	/*
	"TF_RichPresence_MatchGroup_Competitive6v6"   "Competitive"
	"TF_RichPresence_MatchGroup_Casual"           "Casual"
	"TF_RichPresence_MatchGroup_SpecialEvent"     "Special Event"
	"TF_RichPresence_MatchGroup_MannUp"           "MvM Mann Up"
	"TF_RichPresence_MatchGroup_BootCamp"         "MvM Boot Camp"
	*/
	g_SteamInterfaces.Friends->SetRichPresence("currentmap", Vars::Misc::Steam::MapText.Value.empty() ? "Fedoraware" : Vars::Misc::Steam::MapText.Value.c_str());
	
	g_SteamInterfaces.Friends->SetRichPresence("steam_player_group_size", std::to_string(Vars::Misc::Steam::GroupSize.Value).c_str());
}

#ifdef DEBUG
void CMisc::DumpClassIDS() {
	std::ofstream fDump("CLASSIDDUMP.txt");
	fDump << "enum struct ETFClassID\n{\n";
	CClientClass* ClientClass = I::BaseClientDLL->GetAllClasses();
	while (ClientClass) {
		fDump << "	" << ClientClass->GetName() << " = " << ClientClass->m_ClassID << ",\n";
		ClientClass = ClientClass->m_pNext;
	}
	fDump << "}";
	fDump.close();
}
#endif