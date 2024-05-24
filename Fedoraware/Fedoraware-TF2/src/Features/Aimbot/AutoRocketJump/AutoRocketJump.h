#pragma once
#include "../../Feature.h"

class CAutoRocketJump
{
	void ManageAngle(CBaseCombatWeapon* pWeapon, CUserCmd* pCmd, Vec3& viewAngles);

	bool bLastGrounded = false;

	bool bFull = false;
	int iDelay = 0;

public:
	void Run(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd);

	int iFrame = -1;
};

ADD_FEATURE(CAutoRocketJump, AutoRocketJump)