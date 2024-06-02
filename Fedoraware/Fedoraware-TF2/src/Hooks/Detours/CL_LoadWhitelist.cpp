#include "../Hooks.h"

// Renamed the function to better reflect its purpose
bool HOOK_CL_LoadWhitelist(void* table, const char* name)
{
    // Use a static variable to store the original function, so it's not recreated every time
    static auto originalFunction = Hook.Original<FN_CL_LoadWhitelist>();

    if (Vars::Misc::Exploits::BypassPure.Value)
    {
        // Log that the function is being bypassed
        Log::Debug("CL_LoadWhitelist bypassed");
        return false;
    }

    // Call the original function
    return originalFunction(table, name);
}

// Define the function pointer type outside the hook
using FN_CL_LoadWhitelist = bool(__cdecl*)(void*, const char*);
