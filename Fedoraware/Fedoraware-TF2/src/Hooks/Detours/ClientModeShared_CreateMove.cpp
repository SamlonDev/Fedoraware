#include "../Hooks.h"

#include "../../Features/Vars.h"
#include "../../Features/EnginePrediction/EnginePrediction.h"
#include "../../Features/Aimbot/Aimbot.h"
#include "../../Features/Aimbot/AimbotProjectile/AimbotProjectile.h"
#include "../../Features/Misc/Misc.h"
#include "../../Features/Followbot/Followbot.h"
#include "../../Features/TickHandler/TickHandler.h"
#include "../../Features/PacketManip/PacketManip.h"
#include "../../Features/Visuals/FakeAngle/FakeAngle.h"
#include "../../Features/Backtrack/Backtrack.h"
#include "../../Features/CritHack/CritHack.h"
#include "../../Features/NoSpread/NoSpread.h"
#include "../../Features/Resolver/Resolver.h"
#include "../../Features/Visuals/Visuals.h"

MAKE_HOOK(ClientModeShared_CreateMove, Utils::GetVFuncPtr(I::ClientModeShared, 21), bool, __fastcall,
	void* ecx, void* edx, float input_sample_frametime, CUserCmd* pCmd)
{
	G::PSilentAngles = G::SilentAngles = G::IsAttacking = false;
	const bool bReturn = Hook.Original<FN>()(ecx, edx, input_sample_frametime, pCmd);
	if (!pCmd || !pCmd->command_number)
		return bReturn;

	const auto& pLocal = g_EntityCache.GetLocal();
	const auto& pWeapon = g_EntityCache.GetWeapon();

	// NOTE: Riley; Using inline assembly for this is unsafe.
	auto bp = reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) - sizeof(void*);
	auto pSendPacket = reinterpret_cast<bool*>(***reinterpret_cast<uintptr_t***>(bp) - 0x1);

	//if (Vars::Misc::Game::NetworkFix.Value || Vars::Misc::Game::PredictionFix.Value)
		I::Prediction->Update(I::ClientState->m_nDeltaTick, I::ClientState->m_nDeltaTick > 0, I::ClientState->last_command_ack, I::ClientState->lastoutgoingcommand + I::ClientState->chokedcommands);

	G::Buttons = pCmd->buttons;
	G::CurrentUserCmd = pCmd;
	if (!G::LastUserCmd)
		G::LastUserCmd = pCmd;

	// correct tick_count for fakeinterp / nointerp
	pCmd->tick_count += TICKS_TO_TIME(F::Backtrack.flFakeInterp) - (Vars::Visuals::Removals::Interpolation.Value ? 0 : TICKS_TO_TIME(G::LerpTime));

	if (pLocal && pWeapon)
	{	// Update Global Info
		const int nItemDefIndex = pWeapon->m_iItemDefinitionIndex();
		if (G::CurItemDefIndex != nItemDefIndex || !pWeapon->m_iClip1() || !pLocal->IsAlive() || pLocal->IsTaunting() || pLocal->IsBonked() || pLocal->IsAGhost() || pLocal->IsInBumperKart())
			G::WaitForShift = 1;

		G::CurItemDefIndex = nItemDefIndex;
		G::CanPrimaryAttack = pWeapon->CanPrimary(pLocal);
		G::CanSecondaryAttack = pWeapon->CanSecondary(pLocal);
		if (pWeapon->GetSlot() != SLOT_MELEE)
		{
			switch (Utils::GetRoundState())
			{
			case GR_STATE_BETWEEN_RNDS:
			case GR_STATE_GAME_OVER: 
				if (int(pLocal->m_flMaxspeed()) == 1)
					G::CanPrimaryAttack = false;
			}

			if (pWeapon->IsInReload())
				G::CanPrimaryAttack = pWeapon->HasPrimaryAmmoForShot();

			if (pWeapon->GetWeaponID() == TF_WEAPON_MINIGUN && pWeapon->GetMinigunState() != AC_STATE_FIRING && pWeapon->GetMinigunState() != AC_STATE_SPINNING)
				G::CanPrimaryAttack = false;

			if (pWeapon->GetWeaponID() == TF_WEAPON_RAYGUN_REVENGE && pCmd->buttons & IN_ATTACK2)
				G::CanPrimaryAttack = false;

			if (pWeapon->IsEnergyWeapon() && !pWeapon->m_flEnergy())
				G::CanPrimaryAttack = false;

			if (G::CurItemDefIndex != Soldier_m_TheBeggarsBazooka && pWeapon->m_iClip1() == 0)
				G::CanPrimaryAttack = false;
		}
		G::CanHeadShot = pWeapon->CanWeaponHeadShot();
		G::CurWeaponType = Utils::GetWeaponType(pWeapon);
		G::IsAttacking = Utils::IsAttacking(pCmd, pWeapon);
	}

	const bool bSkip = F::AimbotProjectile.bLastTickCancel;
	if (bSkip)
	{
		pCmd->weaponselect = F::AimbotProjectile.bLastTickCancel;
		F::AimbotProjectile.bLastTickCancel = 0;
	}

	// Run Features
	F::Misc.RunPre(pCmd);
	F::Backtrack.Run(pCmd);

	F::EnginePrediction.Start(pCmd);
	const bool bAimRan = bSkip ? false : F::Aimbot.Run(pCmd);
	if (!bAimRan && G::CanPrimaryAttack && G::IsAttacking && !F::AimbotProjectile.bLastTickCancel)
		F::Visuals.ProjectileTrace(false);
	F::EnginePrediction.End(pCmd);

	F::PacketManip.Run(pCmd, pSendPacket);
	F::Ticks.MovePost(pCmd);
	F::CritHack.Run(pCmd);
	F::NoSpread.Run(pCmd);
	F::Misc.RunPost(pCmd, *pSendPacket);
	F::Resolver.CreateMove();
	F::Followbot.Run(pCmd);

	{
		static bool bWasSet = false;
		const bool bOverchoking = I::ClientState->chokedcommands >= 21; // failsafe
		if (G::PSilentAngles && G::ShiftedTicks != G::MaxShift && !bOverchoking)
			*pSendPacket = false, bWasSet = true;
		else if (bWasSet || bOverchoking)
			*pSendPacket = true, bWasSet = false;
	}
	F::Misc.DoubletapPacket(pCmd, pSendPacket);
	F::AntiAim.Run(pCmd, pSendPacket);
	G::Choking = !*pSendPacket;
	if (*pSendPacket)
		F::FakeAngle.Run();
	
	G::ViewAngles = pCmd->viewangles;
	G::LastUserCmd = pCmd;

	//const bool bShouldSkip = G::PSilentAngles || G::SilentAngles || G::AntiAim || G::AvoidingBackstab;
	//return bShouldSkip ? false : Hook.Original<FN>()(ecx, edx, input_sample_frametime, pCmd);
	return false;
}