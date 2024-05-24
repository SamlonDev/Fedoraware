#include "../Hooks.h"
#include "../../Features/CritHack/CritHack.h"

MAKE_HOOK(C_BaseCombatWeapon_AddToCritBucket, S::CBaseCombatWeapon_AddToCritBucket(), void, __fastcall,
	void* ecx, void* edx, float flAmount)
{
	//if (Vars::Debug::Logging.Value)
	//	I::Cvar->ConsoleColorPrintf({ 0, 255, 255, 255 }, "Called AddToCritBucket.\n");

	//const auto& pWeapon = g_EntityCache.GetWeapon();
	//if (pWeapon)
	//{
	//	if (F::CritHack.Storage[pWeapon->GetSlot()].BaseDamage == 0)
	//		F::CritHack.Storage[pWeapon->GetSlot()].BaseDamage = flAmount;
	//}

	if (F::CritHack.ProtectData)
		return;

	Hook.Original<FN>()(ecx, edx, flAmount);
}