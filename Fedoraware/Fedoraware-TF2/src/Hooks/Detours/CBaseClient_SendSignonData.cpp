#include "../Hooks.h"
#include "../../Features/Visuals/Materials/Materials.h"

MAKE_HOOK(CBaseClient_SendSignonData, S::CBaseClient_SendSignonData(), bool, __fastcall,
	void* ecx, void* edx)
{
	F::Materials.ReloadMaterials();
	return Hook.Original<FN>()(ecx, edx);
}