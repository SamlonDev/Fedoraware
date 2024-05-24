#include "../Hooks.h"

MAKE_HOOK(GetClientInterpAmount, S::GetClientInterpAmount(), float, __cdecl,
	)
{
	if (Vars::Visuals::UI::CleanScreenshots.Value && I::EngineClient->IsTakingScreenshot())
		return Hook.Original<FN>()();

	return 0.f;

	/*
	return (Vars::Misc::DisableInterpolation.Value && !(Vars::Visuals::UI::CleanScreenshots.Value && I::EngineClient->IsTakingScreenshot()))
		? 0.f
		: Hook.Original<FN>()();
	*/
}