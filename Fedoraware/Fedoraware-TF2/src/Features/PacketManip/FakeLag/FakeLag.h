#pragma once
#include "../../Feature.h"

class CFakeLag
{
	Vec3 vLastPosition;
	bool bPreservingBlast = false;
	bool bUnducking = false;

	bool IsAllowed(CBaseEntity* pLocal);
	void Prediction(CUserCmd* pCmd);
	void PreserveBlastJump();
	void Unduck(CUserCmd* pCmd);

public:
	void Run(CUserCmd* pCmd, bool* pSendPacket);
};

ADD_FEATURE(CFakeLag, FakeLag)
