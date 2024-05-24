#include "TraceFilters.h"
#include "../../SDK.h"

bool CTraceFilterHitscan::ShouldHitEntity(void* pEntityHandle, int nContentsMask)
{
	auto pEntity = reinterpret_cast<CBaseEntity*>(pEntityHandle);
	if (!pEntity || pEntityHandle == pSkip)
		return false;

	auto pLocal = g_EntityCache.GetLocal();
	auto pWeapon = g_EntityCache.GetWeapon();

	const int iTargetTeam = pEntity->m_iTeamNum(), iLocalTeam = pLocal ? pLocal->m_iTeamNum() : iTargetTeam;
	bool bSniperRifle = false;
	if (pLocal && pLocal == pSkip && pWeapon)
	{
		switch (pWeapon->GetWeaponID())
		{
		case TF_WEAPON_SNIPERRIFLE:
		case TF_WEAPON_SNIPERRIFLE_CLASSIC:
		case TF_WEAPON_SNIPERRIFLE_DECAP:
			bSniperRifle = true;
		}
	}

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
ETraceType CTraceFilterHitscan::GetTraceType() const
{
	return TRACE_EVERYTHING;
}

bool CTraceFilterProjectile::ShouldHitEntity(void* pEntityHandle, int nContentsMask)
{
	auto pEntity = reinterpret_cast<CBaseEntity*>(pEntityHandle);
	if (!pEntity || pEntityHandle == pSkip)
		return false;

	auto pLocal = g_EntityCache.GetLocal();
	auto pWeapon = g_EntityCache.GetWeapon();

	const int iTargetTeam = pEntity->m_iTeamNum(), iLocalTeam = pLocal ? pLocal->m_iTeamNum() : iTargetTeam;
	const bool bCrossbow = (pLocal && pLocal == pSkip && pWeapon) ? pWeapon->GetWeaponID() == TF_WEAPON_CROSSBOW : false;

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
ETraceType CTraceFilterProjectile::GetTraceType() const
{
	return TRACE_EVERYTHING;
}

bool CTraceFilterWorldAndPropsOnly::ShouldHitEntity(void* pEntityHandle, int nContentsMask)
{
	auto pEntity = reinterpret_cast<CBaseEntity*>(pEntityHandle);
	if (!pEntity || pEntityHandle == pSkip)
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
ETraceType CTraceFilterWorldAndPropsOnly::GetTraceType() const
{
	return TRACE_EVERYTHING;
}