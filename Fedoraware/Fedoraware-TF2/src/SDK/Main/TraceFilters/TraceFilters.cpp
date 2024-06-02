#include "TraceFilters.h"
#include "../../SDK.h"

// Base class for all trace filters
class CTraceFilter
{
public:
    virtual ETraceType GetTraceType() const = 0;
};

// Trace filter for hitscan weapons
class CTraceFilterHitscan : public CTraceFilter
{
public:
    bool ShouldHitEntity(void* pEntityHandle, int nContentsMask) const
    {
        const auto pEntity = reinterpret_cast<CBaseEntity*>(pEntityHandle);
        if (!pEntity)
            return false;

        const auto pLocal = g_EntityCache.GetLocal();
        const auto pWeapon = g_EntityCache.GetWeapon();
        if (!pLocal || !pWeapon)
            return false;

        const auto iTargetTeam = pEntity->m_iTeamNum();
        const auto iLocalTeam = pLocal->m_iTeamNum();
        const bool bSniperRifle = IsSniperRifle(pWeapon);

        switch (pEntity->GetClassID())
        {
        case ETFClassID::CTFAmmoPack:
        case ETFClassID::CFuncAreaPortalWindow:
        case ETFClassID::CFuncRespawnRoomVisualizer:
        case ETFClassID::CSniperDot:
        case ETFClassID::CTFReviveMarker: return false;
        case ETFClassID::CTFMedigunShield:
            if (iTargetTeam == iLocalTeam)
                return false;
            break;
        case ETFClassID::CTFPlayer:
        case ETFClassID::CObjectSentrygun:
        case ETFClassID::CObjectDispenser:
        case ETFClassID::CObjectTeleporter:
            if (bSniperRifle && iTargetTeam == iLocalTeam)
                return false;
        }

        return true;
    }

    ETraceType GetTraceType() const override
    {
        return TRACE_EVERYTHING;
    }

private:
    static bool IsSniperRifle(CBaseCombatWeapon* pWeapon)
    {
        const auto weaponId = pWeapon->GetWeaponID();
        return weaponId == TF_WEAPON_SNIPERRIFLE ||
               weaponId == TF_WEAPON_SNIPERRIFLE_CLASSIC ||
               weaponId == TF_WEAPON_SNIPERRIFLE_DECAP;
    }
};

// Trace filter for projectiles
class CTraceFilterProjectile : public CTraceFilter
{
public:
    bool ShouldHitEntity(void* pEntityHandle, int nContentsMask) const
    {
        const auto pEntity = reinterpret_cast<CBaseEntity*>(pEntityHandle);
        if (!pEntity)
            return false;

        const auto pLocal = g_EntityCache.GetLocal();
        const auto pWeapon = g_EntityCache.GetWeapon();
        if (!pLocal || !pWeapon)
            return false;

        const auto iTargetTeam = pEntity->m_iTeamNum();
        const auto iLocalTeam = pLocal->m_iTeamNum();
        const bool bCrossbow = pWeapon->GetWeaponID() == TF_WEAPON_CROSSBOW;

        switch (pEntity->GetClassID())
        {
        case ETFClassID::CBaseEntity:
        case ETFClassID::CBaseDoor:
        case ETFClassID::CDynamicProp:
        case ETFClassID::CPhysicsProp:
        case ETFClassID::CObjectCartDispenser:
        case ETFClassID::CFuncTrackTrain:
        case ETFClassID::CFuncConveyor:
        case ETFClassID::CObjectSentrygun:
        case ETFClassID::CObjectDispenser:
        case ETFClassID::CObjectTeleporter: return true;
        case ETFClassID::CTFPlayer: return bCrossbow ? true : iTargetTeam != iLocalTeam;
        }

        return false;
    }

    ETraceType GetTraceType() const override
    {
        return TRACE_EVERYTHING;
    }
};

// Trace filter for world and props only
class CTraceFilterWorldAndPropsOnly : public CTraceFilter
{
public:
    bool ShouldHitEntity(void* pEntityHandle, int nContentsMask) const
    {
        const auto pEntity = reinterpret_cast<CBaseEntity*>(pEntityHandle);
        if (!pEntity)
            return false;

        switch (pEntity->GetClassID())
        {
        case ETFClassID::CBaseEntity:
        case ETFClassID::CBaseDoor:
        case ETFClassID::CDynamicProp:
        case ETFClassID::CPhysicsProp:
        case ETFClassID::CObjectCartDispenser:
        case ETFClassID::CFuncTrackTrain:
        case ETFClassID::CFuncConveyor: return true;
        }

        return false;
    }

    ETraceType GetTraceType() const override
    {
        return TRACE_EVERYTHING;
    }
};
