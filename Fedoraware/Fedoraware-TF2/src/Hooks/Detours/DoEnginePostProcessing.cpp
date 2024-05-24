#include "../Hooks.h"

MAKE_HOOK(DoEnginePostProcessing, S::DoEnginePostProcessing(), void, __cdecl,
	int x, int y, int w, int h, bool bFlashlightIsOn, bool bPostVGui)
{
	if (!Vars::Visuals::Removals::PostProcessing.Value || Vars::Visuals::UI::CleanScreenshots.Value && I::EngineClient->IsTakingScreenshot())
		Hook.Original<FN>()(x, y, w, h, bFlashlightIsOn, bPostVGui);
}