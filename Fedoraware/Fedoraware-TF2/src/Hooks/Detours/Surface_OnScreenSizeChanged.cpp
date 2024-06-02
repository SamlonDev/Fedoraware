#include "../Hooks.h"

// Use a lambda to wrap the original function and avoid potential issues with function pointers
auto Surface_OnScreenSizeChanged_Original =
    [original = Utils::GetVFuncPtr<FN>(I::MatSystemSurface, 111)]() -> void(__fastcall*)(void*, void*, int, int) {
        return original;
    }();

void Hooked_Surface_OnScreenSizeChanged(void* ecx, void* edx, int nOldWidth, int nOldHeight) {
    // Call the original function
    Surface_OnScreenSizeChanged_Original(ecx, edx, nOldWidth, nOldHeight);

    // Call the custom code
    g_ScreenSize.Update();
    g_Draw.ReloadFonts();
}

MAKE_HOOK(Surface_OnScreenSizeChanged, &Hooked_Surface_OnScreenSizeChanged);
