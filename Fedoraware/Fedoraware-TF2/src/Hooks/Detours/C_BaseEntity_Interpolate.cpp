#include "../Hooks.h"

// Xref: C_BaseEntity_BaseInterpolatePart1
HOOK_DECLARE(bool, C_BaseEntity_Interpolate, void* ecx, void* edx, float currentTime);

bool Hook_C_BaseEntity_Interpolate(void* ecx, void* edx, float currentTime)
{
	if (Vars::Visuals::Removals::Interpolation.Value)
		return true;

	return HOOK_ORIGINAL(C_BaseEntity_Interpolate)(ecx, edx, currentTime);
}
