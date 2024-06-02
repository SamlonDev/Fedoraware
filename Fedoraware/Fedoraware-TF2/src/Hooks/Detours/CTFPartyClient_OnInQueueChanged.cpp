#include "../Hooks.h"

// Look for the specified pattern in the client.dll module
static auto pattern = g_Pattern.Find(L"client.dll", L"55 8B EC 56 8B 75 08 57 56 8B F9 E8 ? ? ? ? 83 C4 04 84 C0 74 38 83 FE FF 74 33 8B 57 38 33 C0 85 D2 7E 0F 8B 4F 2C");

// The hook function. This function will be called instead of the original function.
void CTFPartyClient_OnInQueueChanged_Hook(void* ecx, void* edx, int iMatchGroup)
{
    // Call the original function.
    Hook.Original<FN>()(ecx, edx, iMatchGroup);
}

// The hook definition.
MAKE_HOOK(CTFPartyClient_OnInQueueChanged, pattern, CTFPartyClient_OnInQueueChanged_Hook, MHOOK_THISCALL);

// Initialize the hook.
bool CTFPartyClient_OnInQueueChanged_Init()
{
    // Check if the pattern was found.
    if (!pattern)
    {
        // The pattern was not found.
        return false;
    }

    // Install the hook.
    if (!MAKE_HOOK_FUNCTION(CTFPartyClient_OnInQueueChanged, CTFPartyClient_OnInQueueChanged_Hook, MHOOK_THISCALL))
    {
        // Failed to install the hook.
        return false;
    }

    // The hook was installed successfully.
    return true;
}

// Clean up the hook.
void CTFPartyClient_OnInQueueChanged_Cleanup()
{
    // Uninstall the hook.
    UNMAKE_HOOK(CTFPartyClient_OnInQueueChanged);
}
