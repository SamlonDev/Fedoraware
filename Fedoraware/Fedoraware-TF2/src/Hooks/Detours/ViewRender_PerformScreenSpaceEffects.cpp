#include "../Hooks.h"

// Hook function for ViewRender_PerformScreenSpaceEffects.
// This function is called every frame when the game performs screen space effects.
// In this example, the hook function does nothing.
MAKE_HOOK(ViewRender_PerformScreenSpaceEffects, S::ViewRender_PerformScreenSpaceEffects, void, __stdcall,
    void* ecx, void* edx, int x, int y, int w, int h)
{
    // Only call the hook function when the game is in a valid state.
    if (g_GameState != GAMESTATE_INGAME)
        return;

    // Call the original function.
    ORIGINAL_CALL(ViewRender_PerformScreenSpaceEffects)(ecx, edx, x, y, w, h);

    // Do nothing.
}

// Install the hook.
bool InstallHooks()
{
    // Only install the hook once.
    static bool s_HookInstalled = false;
    if (s_HookInstalled)
        return true;

    // Install the hook.
    if (!InstallHook(S::ViewRender_PerformScreenSpaceEffects, ViewRender_PerformScreenSpaceEffects))
        return false;

    s_HookInstalled = true;
    return true;
}
