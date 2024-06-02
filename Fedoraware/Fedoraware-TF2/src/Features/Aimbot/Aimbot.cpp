#include "Aimbot.h"
#include "../Vars.h"

#include "AimbotHitscan/AimbotHitscan.h"
#include "AimbotProjectile/AimbotProjectile.h"
#include "AimbotMelee/AimbotMelee.h"
#include "AutoDetonate/AutoDetonate.h"
#include "AutoAirblast/AutoAirblast.h"
#include "AutoUber/AutoUber.h"
#include "AutoRocketJump/AutoRocketJump.h"
#include "../Misc/Misc.h"

bool CAimbot::ShouldRun(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
    // Return false if the game UI is visible or the cursor is visible
    if (I::EngineVGui->IsGameUIVisible() || I::MatSystemSurface->IsCursorVisible())
        return false;

    // Return false if the local player is not alive, taunting, bonked, feign death ready, cloaked, in bumper kart, or a ghost
    if (!pLocal ||
        !pLocal->IsAlive() ||
        pLocal->IsTaunting() ||

