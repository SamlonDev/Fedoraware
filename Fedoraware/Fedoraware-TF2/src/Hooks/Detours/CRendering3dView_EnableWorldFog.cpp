#include "../Hooks.h"

MAKE_HOOK(CRendering3dView_EnableWorldFog, S::CRendering3dView_EnableWorldFog(), void, __cdecl,
	)
{
	if (I::EngineClient->IsTakingScreenshot() && Vars::Visuals::UI::CleanScreenshots.Value || !(Vars::Visuals::World::Modulations.Value & 1 << 4))
		return Hook.Original<FN>()();

	if (!Vars::Colors::FogModulation.Value.a)
		return;

	Hook.Original<FN>()();
	if (const auto pRenderContext = I::MaterialSystem->GetRenderContext())
	{
		float blend[3] = { float(Vars::Colors::FogModulation.Value.r) / 255.f, float(Vars::Colors::FogModulation.Value.g) / 255.f, float(Vars::Colors::FogModulation.Value.b) / 255.f };
		pRenderContext->FogColor3fv(blend);
	}
}