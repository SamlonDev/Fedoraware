#pragma once
#include "../../Feature.h"

class LCESP
{
public:
	std::vector<std::wstring> GetPlayerConditions(CBaseEntity* pEntity) const;
};

ADD_FEATURE(LCESP, LocalConditions)