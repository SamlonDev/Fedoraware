#include "../Hooks.h"

#include "../../Features/Visuals/Visuals.h"
#include "../../Features/CameraWindow/CameraWindow.h"

MAKE_HOOK(Panel_PaintTraverse, Utils::GetVFuncPtr(I::VGuiPanel, 41), void, __fastcall,
	void* ecx, void* edx, unsigned int vgui_panel, bool force_repaint, bool allow_force)
{
	if (FNV1A::Hash(I::VGuiPanel->GetName(vgui_panel)) != FNV1A::Hash("MatSystemTopPanel") || Vars::Visuals::UI::CleanScreenshots.Value && I::EngineClient->IsTakingScreenshot())
		return Hook.Original<FN>()(ecx, edx, vgui_panel, force_repaint, allow_force);

	F::CameraWindow.Draw();

	Hook.Original<FN>()(ecx, edx, vgui_panel, force_repaint, allow_force);
}