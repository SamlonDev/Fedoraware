#include "../Hooks.h"
#include "../../Features/CritHack/CritHack.h"

MAKE_HOOK(C_TFWeaponBase_CalcIsAttackCritical, S::CTFWeaponBase_CalcIsAttackCritical(), void, __fastcall,
	void* ecx, void* edx)
{
	const auto& pLocal = g_EntityCache.GetLocal();
	const auto pWeapon = reinterpret_cast<CBaseCombatWeapon*>(ecx);
	if (!pLocal || !pWeapon || pWeapon != g_EntityCache.GetWeapon())
		Hook.Original<FN>()(ecx, edx);

	if (!F::CritHack.CalcIsAttackCriticalHandler(pLocal, pWeapon))
		return;

	const auto nPreviousWeaponMode = pWeapon->m_iWeaponMode();
	pWeapon->m_iWeaponMode() = 0;
	Hook.Original<FN>()(ecx, edx);
	pWeapon->m_iWeaponMode() = nPreviousWeaponMode;
}