// Include the necessary headers
#include "../Hooks.h"
#include <MinHook.h>

// Define the hook function with the correct signature
typedef void(__fastcall* tCClient_Precipitation_Render)(void* ecx, void* edx);
tCClient_Precipitation_Render oCClient_Precipitation_Render;

// Define the hook function
void __fastcall CClient_Precipitation_Render_Hook(void* ecx, void* edx)
{
    // Call the original function
    oCClient_Precipitation_Render(ecx, edx);

    // Add your custom code here
}

// Initialize the hook
bool Initialize()
{
    // Initialize MinHook
    if (MH_Initialize() != MH_OK)
        return false;

    // Find the pattern in memory
    auto addr = g_Pattern.Find(L"client.dll", L"55 8B EC 81 EC ? ? ? ? 33 C0 89 45 FC 39 05 ? ? ? ? 0F 8E ? ? ? ? 53 56 57 8D 49 00 8B 3D ? ? ? ? 8B 3C 87 A1");
    if (!addr)
        return false;

    // Create the hook
    if (MH_CreateHook(addr, &CClient_Precipitation_Render_Hook, reinterpret_cast<void**>(&oCClient_Precipitation_Render)) != MH_OK)
        return false;

    // Enable the hook
    if (MH_EnableHook(addr) != MH_OK)
        return false;

    return true;
}

// Uninitialize the hook
void Uninitialize()
{
    // Disable the hook
    MH_DisableHook(g_Pattern.Find(L"client.dll", L"55 8B EC 81 EC ? ? ? ? 33 C0 89 45 FC 39 05 ? ? ? ? 0F 8E ? ? ? ? 53 56 57 8D 49 00 8B 3D ? ? ? ? 8B 3C 87 A1"));

    // Uninitialize MinHook
    MH_Uninitialize();
}
