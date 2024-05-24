#include "../Hooks.h"

#include "../../Features/Visuals/Materials/Materials.h"
#include "../../Features/Visuals/Glow/Glow.h"
#include "../../Features/CameraWindow/CameraWindow.h"

#include <mutex>

MAKE_HOOK(ViewRender_RenderView, Utils::GetVFuncPtr(I::ViewRender, 6), void, __fastcall,
	void* ecx, void* edx, const CViewSetup& view, ClearFlags_t nClearFlags, RenderViewInfo_t whatToDraw)
{
	static std::once_flag onceFlag;
	std::call_once(onceFlag, []
	{
		F::Materials.ReloadMaterials();
		F::Glow.Init();
		F::CameraWindow.Init();
	});

	Hook.Original<void(__thiscall*)(void*, const CViewSetup&, int, int)>()(ecx, view, nClearFlags, whatToDraw);

	if (!(I::EngineClient->IsTakingScreenshot() && Vars::Visuals::UI::CleanScreenshots.Value))
		F::CameraWindow.RenderView(ecx, view);
}