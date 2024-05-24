#pragma once
#include "../../Feature.h"

class CAntiAim
{
private:
	void FakeShotAngles(CUserCmd* pCmd);

	float EdgeDistance(float flEdgeRayYaw, CBaseEntity* pEntity);
	bool IsOverlapping(float epsilon);

	float GetYawOffset(const bool bFake);
	float GetBaseYaw(CBaseEntity* pLocal, CUserCmd* pCmd, const bool bFake);
	float CalculateCustomRealPitch(float WishPitch, bool FakeDown);
	float GetPitch(const float flCurPitch);

	float flBaseYaw = 0.f;

public:
	bool AntiAimOn();
	bool ShouldRun(CBaseEntity* pLocal);

	bool GetEdge(const float flEdgeOrigYaw = I::EngineClient->GetViewAngles().y, CBaseEntity* pEntity = g_EntityCache.GetLocal());
	void Run(CUserCmd* pCmd, bool* pSendPacket);

	Vec2 vFakeAngles{};
	Vec2 vRealAngles{};
};

ADD_FEATURE(CAntiAim, AntiAim)