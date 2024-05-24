#include "../Hooks.h"

#include "../../Features/NoSpread/NoSpreadHitscan/NoSpreadHitscan.h"

MAKE_HOOK(ViewRender_LevelShutdown, Utils::GetVFuncPtr(I::ViewRender, 2), void, __fastcall,
	void* ecx, void* edx)
{
	F::NoSpreadHitscan.Reset(true);

	Hook.Original<FN>()(ecx, edx);
}