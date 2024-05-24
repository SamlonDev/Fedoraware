#include "../Hooks.h"

MAKE_HOOK(StudioRender_SetAlphaModulation, Utils::GetVFuncPtr(I::StudioRender, 28), void, __fastcall,
	void* ecx, void* edx, float flAlpha)
{
	if (Vars::Visuals::World::Modulations.Value & 1 << 2 && G::DrawingStaticProps)
		return Hook.Original<FN>()(ecx, edx, float(Vars::Colors::PropModulation.Value.a) / 255.f * flAlpha);

	Hook.Original<FN>()(ecx, edx, flAlpha);
}