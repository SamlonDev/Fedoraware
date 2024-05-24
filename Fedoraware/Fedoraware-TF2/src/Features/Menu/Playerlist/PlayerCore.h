#pragma once
#include "../../Feature.h"

#include "PlayerUtils.h"
#include "../ConfigManager/ConfigManager.h"

class CPlayerlistCore
{
	void SavePlayers();
	void LoadPlayers();
	void SaveTags();
	void LoadTags();

public:
	void Run();
};

ADD_FEATURE(CPlayerlistCore, PlayerCore)