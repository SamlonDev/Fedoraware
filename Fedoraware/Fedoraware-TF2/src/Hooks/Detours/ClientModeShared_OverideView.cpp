#include "../Hooks.h"

#include "../../Features/Visuals/Visuals.h"

MAKE_HOOK(ClientModeShared_OverrrideView, Utils::GetVFuncPtr(I::ClientModeShared, 16), void, __fastcall,
	void* ecx, void* edx, CViewSetup* pView)
{
	if (Vars::Visuals::UI::CleanScreenshots.Value && I::EngineClient->IsTakingScreenshot())
		return Hook.Original<FN>()(ecx, edx, pView);
	Hook.Original<FN>()(ecx, edx, pView);

	F::Visuals.FOV(pView);
	F::Visuals.ThirdPerson(pView);
}