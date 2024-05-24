#include "../Hooks.h"

#include "../../Features/Visuals/Chams/Chams.h"
#include "../../Features/Visuals/Glow/Glow.h"

MAKE_HOOK(ModelRender_ForcedMaterialOverride, Utils::GetVFuncPtr(I::ModelRender, 1), void, __fastcall,
	void* ecx, void* edx, IMaterial* mat, EOverrideType type)
{
	//if (F::Glow.bRendering && !F::Glow.IsGlowMaterial(mat) || F::Chams.bRendering && !F::Chams.IsChamsMaterial(mat))
	//	return;

	Hook.Original<FN>()(ecx, edx, mat, type);
}