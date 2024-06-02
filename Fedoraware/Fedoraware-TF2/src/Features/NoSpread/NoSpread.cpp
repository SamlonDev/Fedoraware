#include "NoSpread.h"
#include "../Vars.h"
#include "NoSpreadProjectile/NoSpreadProjectile.h"
#include "NoSpreadHitscan/NoSpreadHitscan.h"

bool CNoSpread::ShouldRun(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
    if (!Vars::Aimbot::General::NoSpread.Value)
        return false;

    if (!pLocal || !pWeapon)
        return false;

    if (!pLocal->IsAlive()
        || pLocal->IsTaunting()
        || pLocal->IsBonked()
        || pLocal->m_bFeignDeathReady()
        || pLocal->IsCloaked()
        || pLocal->IsInBumperKart()
        || pLocal->IsAGhost())
    {
        return false;
    }

    return true;
}

void CNoSpread::Run(CUserCmd* pCmd)
{
    const auto pLocal = g_EntityCache.GetLocal();
    if (!pLocal)
        return;

    const auto pWeapon = g_EntityCache.GetWeapon();
    if (!pWeapon)
        return;

    if (!ShouldRun(pLocal, pWeapon))
        return;

    F::NoSpreadHitscan.Run(pCmd, pLocal, pWeapon);
    F::NoSpreadProjectile.Run(pCmd, pLocal, pWeapon);
}
