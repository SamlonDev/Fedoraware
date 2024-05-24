#include "../Hooks.h"
#include "../../Features/CritHack/CritHack.h"

MAKE_HOOK(C_BaseCombatWeapon_CanFireRandomCriticalShot, g_Pattern.Find(L"client.dll", L"55 8B EC F3 0F 10 4D ? F3 0F 58 0D ? ? ? ? F3 0F 10 81 ? ? ? ? 0F 2F C1 76 ? 32 C0 5D C2 ? ? B0 ? 5D C2"), bool, __fastcall,
	void* ecx, void* edx, float flCritChance)
{
	//const auto& pWeapon = g_EntityCache.GetWeapon();
	//if (pWeapon)
	//	F::CritHack.CanFireCriticalShotHandler(flCritChance);

	return Hook.Original<FN>()(ecx, edx, flCritChance);
}