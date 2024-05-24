#include "../Hooks.h"

//	xref C_BaseEntity_BaseInterpolatePart1
MAKE_HOOK(C_BaseEntity_Interpolate, S::CBaseEntity_Interpolate(), bool, __fastcall,
	void* ecx, void* edx, float currentTime)
{
	return Vars::Visuals::Removals::Interpolation.Value
		? true
		: Hook.Original<FN>()(ecx, edx, currentTime);
}