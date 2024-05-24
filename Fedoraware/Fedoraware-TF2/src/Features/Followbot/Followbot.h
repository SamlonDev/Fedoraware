#pragma once
#include <mutex>
#include "../Feature.h"

class CFollowbot
{
	bool ValidTarget(CBaseEntity* pTarget, CBaseEntity* pLocal);
	void OptimizePath(CBaseEntity* pLocal);
	CBaseEntity* FindTarget(CBaseEntity* pLocal);

	struct PathNode
	{
		Vec3 Location{};
		bool OnGround = false;
	};

	CBaseEntity* CurrentTarget = nullptr;
	std::deque<PathNode> PathNodes;
	std::mutex PathMutex;

public:
	void Run(CUserCmd* pCmd);
	void Reset();
	void Draw();
	void Medigun(CUserCmd* pCmd);
};

ADD_FEATURE(CFollowbot, Followbot)