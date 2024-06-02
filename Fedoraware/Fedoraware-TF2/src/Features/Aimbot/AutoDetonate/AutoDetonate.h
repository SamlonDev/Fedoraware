#ifndef AUTODETONATE_H
#define AUTODETONATE_H

#include "../../Feature.h"
#include "../AimbotGlobal/AimbotGlobal.h"

namespace Features = Client::Features;

class CAutoDetonate
{
public:
    std::optional<bool> CheckDetonation(const CBaseEntity* pLocal, EGroupType entityGroup, float flRadiusScale, const CUserCmd* pCmd);

    void Run(const CBaseEntity* pLocal, const CBaseCombatWeapon* pWeapon, CUserCmd* pCmd);
};

ADD_FEATURE(CAutoDetonate, Features::AutoDetonate)

#endif //AUTODETONATE_H
