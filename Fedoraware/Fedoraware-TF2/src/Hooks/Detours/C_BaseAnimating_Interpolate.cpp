#include <hooks/Hooks.h> // Include the correct header file for the MAKE_HOOK macro

// Rename the function pointer type to be more descriptive
using InterpolateFn = bool(__fastcall*)(void* ecx, void* edx, float currentTime);

// Rename the hook to be more descriptive
MAKE_HOOK(CBaseAnimating_Interpolate, // No need to include the "../" in the hook name
	S::CBaseAnimating_Interpolate, // The original function signature
	InterpolateFn, // The function pointer type
	&CBaseAnimating::Interpolate, // The original function address
	[](void* ecx, void* edx, float currentTime, InterpolateFn original) -> bool { // The hook function
		// Check if the interpolation removal option is enabled and the entity is not the local player
	
