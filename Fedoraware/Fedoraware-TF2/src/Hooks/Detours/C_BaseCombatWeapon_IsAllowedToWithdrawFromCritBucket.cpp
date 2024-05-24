#include "../Hooks.h"
#include "../../Features/CritHack/CritHack.h"

MAKE_HOOK(C_BaseCombatWeapon_IsAllowedToWithdrawFromCritBucket, S::CBaseCombatWeapon_IsAllowedToWithdrawFromCritBucket(), bool, __fastcall,
	void* ecx, void* edx, float flDamage)
{
	//if (Vars::Debug::Logging.Value)
	//	I::Cvar->ConsoleColorPrintf({ 0, 255, 255, 255 }, "Called WithdrawFromCritBucket.\n");

	//const auto& pWeapon = g_EntityCache.GetWeapon();
	//if (pWeapon)
	//{
	//	if (F::CritHack.Storage[pWeapon->GetSlot()].BaseDamage == 0)
	//		F::CritHack.Storage[pWeapon->GetSlot()].BaseDamage = flDamage;
	//}

	if (F::CritHack.ProtectData)
		return true;

	return Hook.Original<FN>()(ecx, edx, flDamage);
}