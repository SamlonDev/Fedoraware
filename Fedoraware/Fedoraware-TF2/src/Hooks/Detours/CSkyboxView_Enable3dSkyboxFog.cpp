#include "../Hooks.h"

MAKE_HOOK(CSkyboxView_Enable3dSkyboxFog, S::CSkyboxView_Enable3dSkyboxFog(), void, __fastcall,
	void* ecx, void* edx)
{
	if (I::EngineClient->IsTakingScreenshot() && Vars::Visuals::UI::CleanScreenshots.Value || !(Vars::Visuals::World::Modulations.Value & 1 << 4))
		return Hook.Original<FN>()(ecx, edx);

	if (!Vars::Colors::FogModulation.Value.a)
		return;

	Hook.Original<FN>()(ecx, edx);
	if (const auto pRenderContext = I::MaterialSystem->GetRenderContext())
	{
		float blend[3] = { float(Vars::Colors::FogModulation.Value.r) / 255.f, float(Vars::Colors::FogModulation.Value.g) / 255.f, float(Vars::Colors::FogModulation.Value.b) / 255.f };
		pRenderContext->FogColor3fv(blend);
	}
}