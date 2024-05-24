#include "../Hooks.h"

MAKE_HOOK(StudioRender_SetColorModulation, Utils::GetVFuncPtr(I::StudioRender, 27), void, __fastcall,
	void* ecx, void* edx, const float* pColor)
{
	const float flCustomBlend[3] = {
		float(Vars::Colors::PropModulation.Value.r) / 255.f,
		float(Vars::Colors::PropModulation.Value.g) / 255.f,
		float(Vars::Colors::PropModulation.Value.b) / 255.f
	};

	const bool bScreenshot = Vars::Visuals::UI::CleanScreenshots.Value && I::EngineClient->IsTakingScreenshot();
	const bool bShouldUseCustomBlend = Vars::Visuals::World::Modulations.Value & 1 << 2 && G::DrawingStaticProps && !bScreenshot;
	Hook.Original<FN>()(ecx, edx, bShouldUseCustomBlend ? flCustomBlend : pColor);
}