#include "../Hooks.h"
#include "../../Features/Visuals/Materials/Materials.h"

MAKE_HOOK(CBaseClient_Disconnect, S::CBaseClient_Disconnect(), void, __fastcall,
	void* ecx, void* edx, const char* fmt, ...)
{
	F::Materials.RemoveMaterials();
	return Hook.Original<FN>()(ecx, edx, fmt);
}