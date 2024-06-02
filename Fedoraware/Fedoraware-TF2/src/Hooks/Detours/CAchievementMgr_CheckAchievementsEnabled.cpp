#include "../Hooks.h"

// Renamed the function to follow naming conventions
// and added a brief comment describing its purpose
bool Hook_CAchievementMgr_CheckAchievementsEnabled(void* ecx, void* edx)
{
    // The original function always returns true
    // This implementation is equivalent, but more explicit
    return true;
}

MAKE_HOOK(CAchievementMgr_CheckAchievementsEnabled, Hook_CAchievementMgr_CheckAchievementsEnabled, bool, __fastcall);
