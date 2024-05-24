#pragma once
#include "../../Feature.h"

#include "../AimbotGlobal/AimbotGlobal.h"

class CAutoDetonate
{
	bool CheckDetonation(CBaseEntity* pLocal, EGroupType entityGroup, float flRadiusScale, CUserCmd* pCmd);

public:
	void Run(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd);
};

ADD_FEATURE(CAutoDetonate, AutoDetonate)