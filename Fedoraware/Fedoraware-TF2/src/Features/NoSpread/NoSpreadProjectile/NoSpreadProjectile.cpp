#include "NoSpreadProjectile.h"
#include "../../Vars.h"

bool CNoSpreadProjectile::ShouldRun(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	if (G::CurWeaponType != EWeaponType::PROJECTILE)
		return false;

	switch (G::CurItemDefIndex)
	{
	case Soldier_m_RocketJumper:
	case Demoman_s_StickyJumper:
		return false;
	}

	return G::IsAttacking;
}

void CNoSpreadProjectile::Run(CUserCmd* pCmd, CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	if (!ShouldRun(pLocal, pWeapon))
		return;

	Utils::SharedRandomInt(MD5_PseudoRandom(pCmd->command_number) & 0x7FFFFFFF, "SelectWeightedSequence", 0, 0, 0);
	for (int i = 0; i < 6; ++i)
		Utils::RandomFloat();

	switch (pWeapon->GetWeaponID())
	{
	case TF_WEAPON_SYRINGEGUN_MEDIC:
	{
		// don't let the _local_ syringes fool you (is there a way to fix or sync them?)
		pCmd->viewangles.x -= Utils::RandomFloat(-1.5f, 1.5f);
		pCmd->viewangles.y -= Utils::RandomFloat(-1.5f, 1.5f);

		G::PSilentAngles = true;
		return;
	}
	case TF_WEAPON_COMPOUND_BOW:
	{
		// ShouldRun huntsman
		if (pWeapon->m_flChargeBeginTime() > 0.f && I::GlobalVars->curtime - pWeapon->m_flChargeBeginTime() <= 5.0f)
			return;

		float flRand = float(Utils::RandomInt()) / 0x7FFF;
		pCmd->viewangles.x -= -6 + flRand * 12.f;
		flRand = float(Utils::RandomInt()) / 0x7FFF;
		pCmd->viewangles.y -= -6 + flRand * 12.f;

		G::PSilentAngles = true;
		return;
	}
	}

	if (G::CurItemDefIndex == Soldier_m_TheBeggarsBazooka)
	{
		pCmd->viewangles -= pWeapon->GetSpreadAngles() - I::EngineClient->GetViewAngles();

		G::PSilentAngles = true;
	}
}