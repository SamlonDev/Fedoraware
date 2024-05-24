#pragma once
#include "../../Feature.h"

class CAutoAirblast
{
	bool CanAirblastEntity(CBaseEntity* pLocal, CBaseEntity* pEntity, Vec3& vAngle, Vec3& vPos);

public:
	void Run(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd);
};

ADD_FEATURE(CAutoAirblast, AutoAirblast)