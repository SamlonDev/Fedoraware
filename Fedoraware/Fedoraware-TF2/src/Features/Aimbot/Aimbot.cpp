#include "Aimbot.h"
#include "../Vars.h"

#include "AimbotHitscan/AimbotHitscan.h"
#include "AimbotProjectile/AimbotProjectile.h"
#include "AimbotMelee/AimbotMelee.h"
#include "AutoDetonate/AutoDetonate.h"
#include "AutoAirblast/AutoAirblast.h"
#include "AutoUber/AutoUber.h"
#include "AutoRocketJump/AutoRocketJump.h"
#include "../Misc/Misc.h"

bool CAimbot::ShouldRun(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	if (I::EngineVGui->IsGameUIVisible() || I::MatSystemSurface->IsCursorVisible())
		return false;

	if (!pLocal->IsAlive() ||
		pLocal->IsTaunting() ||
		pLocal->IsBonked() ||
		pLocal->m_bFeignDeathReady() ||
		pLocal->IsCloaked() ||
		pLocal->IsInBumperKart() ||
		pLocal->IsAGhost())
		return false;

	switch (G::CurItemDefIndex)
	{
	case Soldier_m_RocketJumper:
	case Demoman_s_StickyJumper:
		return false;
	}

	return true;
}

bool CAimbot::Run(CUserCmd* pCmd)
{
	G::AimPos = Vec3();

	const auto pLocal = g_EntityCache.GetLocal();
	const auto pWeapon = g_EntityCache.GetWeapon();

	F::AutoRocketJump.Run(pLocal, pWeapon, pCmd);

	if (!pLocal || !pWeapon || !ShouldRun(pLocal, pWeapon))
		return false;

	F::AutoDetonate.Run(pLocal, pWeapon, pCmd);
	F::AutoAirblast.Run(pLocal, pWeapon, pCmd);
	F::AutoUber.Run(pLocal, pWeapon, pCmd);

	const bool bAttacking = G::IsAttacking;
	switch (G::CurWeaponType)
	{
		case EWeaponType::HITSCAN:
			F::AimbotHitscan.Run(pLocal, pWeapon, pCmd); break;
		case EWeaponType::PROJECTILE:
			F::AimbotProjectile.Run(pLocal, pWeapon, pCmd); break;
		case EWeaponType::MELEE:
			F::AimbotMelee.Run(pLocal, pWeapon, pCmd); break;
	}
	if (pWeapon->GetWeaponID() == TF_WEAPON_COMPOUND_BOW || pWeapon->GetWeaponID() == TF_WEAPON_PIPEBOMBLAUNCHER || pWeapon->GetWeaponID() == TF_WEAPON_STICKY_BALL_LAUNCHER || pWeapon->GetWeaponID() == TF_WEAPON_GRENADE_STICKY_BALL || pWeapon->GetWeaponID() == TF_WEAPON_CANNON)
	{
		if (!(G::Buttons & IN_ATTACK) && pCmd->buttons & IN_ATTACK)
			return true;
	}
	return bAttacking != G::IsAttacking;
}
