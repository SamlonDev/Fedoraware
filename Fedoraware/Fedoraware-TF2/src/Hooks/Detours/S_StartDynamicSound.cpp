#include "../Hooks.h"
#include "../../Features/Visuals/Visuals.h"

// Define the hook for the S_StartDynamicSound function
MAKE_HOOK(S_StartDynamicSound, S::S_StartDynamicSound, int, __cdecl, StartSoundParams_t& params)
{
    // Use the Visuals feature to manually network the sound parameters
    F::Visuals.ManualNetwork(params);

    // Call the original function with the provided parameters
    return Hook.Original<FN>()(params);
}
