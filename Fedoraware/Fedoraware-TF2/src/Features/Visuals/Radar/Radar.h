#pragma once
#include "../../Feature.h"

class CRadar
{
	bool GetDrawPosition(CBaseEntity* pLocal, CBaseEntity* pEntity, int& x, int& y, int& z);
	void DrawBackground();
	void DrawPoints();

public:
	void Run();
};

ADD_FEATURE(CRadar, Radar)