#pragma once
#include "../../Feature.h"

class CNoSpreadProjectile
{
private:
	bool ShouldRun(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon);

public:
	void Run(CUserCmd* pCmd, CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon);
};

ADD_FEATURE(CNoSpreadProjectile, NoSpreadProjectile)