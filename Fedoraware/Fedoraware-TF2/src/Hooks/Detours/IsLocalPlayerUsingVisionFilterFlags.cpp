#include "../Hooks.h"

#define TF_VISION_FILTER_NONE			0
#define TF_VISION_FILTER_PYRO			(1<<0)		// 1
#define TF_VISION_FILTER_HALLOWEEN		(1<<1)		// 2
#define TF_VISION_FILTER_ROME			(1<<2)		// 4

MAKE_HOOK(IsLocalPlayerUsingVisionFilterFlags, S::IsLocalPlayerUsingVisionFilterFlags(), bool, __cdecl,
	int nFlags, bool bWeaponsCheck)
{
	if (I::EngineClient->IsTakingScreenshot() && Vars::Visuals::UI::CleanScreenshots.Value) { return Hook.Original<FN>()(nFlags, bWeaponsCheck); }
	switch (Vars::Visuals::VisionModifier.Value)
	{
	case 1:
	{
		return nFlags == TF_VISION_FILTER_PYRO ? true : false;
	}
	case 2:
	{
		return nFlags == TF_VISION_FILTER_HALLOWEEN ? true : false;
	}
	case 3:
	{
		return nFlags == TF_VISION_FILTER_ROME ? true : false;
	}
	}

	return Hook.Original<FN>()(nFlags, bWeaponsCheck);
}