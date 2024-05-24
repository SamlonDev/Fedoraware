#pragma once
#include "../Feature.h"

class CNoSpread
{
private:
	bool ShouldRun(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon);

public:
	void Run(CUserCmd* pCmd);
};

ADD_FEATURE(CNoSpread, NoSpread)