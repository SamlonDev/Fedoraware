#include "../Hooks.h"

MAKE_HOOK(CTFWeaponBase_GetShootSound, S::CTFWeaponBase_GetShootSound(), const char*, __fastcall,
	void* ecx, void* edx, int iIndex)
{
	if (Vars::Misc::Sound::GiantWeaponSounds.Value)
	{
		const auto& pWeapon = reinterpret_cast<CBaseCombatWeapon*>(ecx);
		const auto& pOwner = pWeapon ? I::ClientEntityList->GetClientEntityFromHandle(pWeapon->m_hOwnerEntity()) : nullptr;
		const auto& pLocal = g_EntityCache.GetLocal();
		if (pWeapon && pOwner == pLocal)
		{
			int nOldTeam = pWeapon->m_iTeamNum();
			pWeapon->m_iTeamNum() = 4;
			auto ret = Hook.Original<FN>()(ecx, edx, iIndex);
			pWeapon->m_iTeamNum() = nOldTeam;

			// credits: KGB

			static auto FireHash = FNV1A::Hash("Weapon_FlameThrower.Fire");
			if (FireHash == FNV1A::HashConst(ret))
				return "MVM.GiantPyro_FlameStart";

			static auto FireLoopHash = FNV1A::Hash("Weapon_FlameThrower.FireLoop");
			if (FireLoopHash == FNV1A::HashConst(ret))
				return "MVM.GiantPyro_FlameLoop";

			static auto GrenadeHash = FNV1A::Hash("Weapon_GrenadeLauncher.Single");
			if (GrenadeHash == FNV1A::HashConst(ret))
				return "MVM.GiantDemoman_Grenadeshoot";

			return ret;
		}
	}

	return Hook.Original<FN>()(ecx, edx, iIndex);
}