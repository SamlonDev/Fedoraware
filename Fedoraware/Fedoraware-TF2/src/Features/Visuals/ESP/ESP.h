#pragma once
#include "../../Feature.h"

class CESP
{
private:
	static const wchar_t* GetPlayerClass(int nClassNum);

	void DrawPlayers(CBaseEntity* pLocal);
	void DrawBuildings(CBaseEntity* pLocal) const;
	void DrawWorld() const;
	static void DrawBones(CBaseEntity* pPlayer, const std::vector<int>& vecBones, Color_t clr);

public:
	void Run();
	static bool GetDrawBounds(CBaseEntity* pEntity, int& x, int& y, int& w, int& h);
};

ADD_FEATURE(CESP, ESP)