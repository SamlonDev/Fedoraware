#include "../Hooks.h"

#include "../../Features/Visuals/Chams/Chams.h"
#include "../../Features/Visuals/Glow/Glow.h"
#include "../../Features/Visuals/Visuals.h"

MAKE_HOOK(ClientModeShared_DoPostScreenSpaceEffects, Utils::GetVFuncPtr(I::ClientModeShared, 39), bool, __fastcall,
	void* ecx, void* edx, const CViewSetup* pSetup)
{
	if (Vars::Visuals::UI::CleanScreenshots.Value && I::EngineClient->IsTakingScreenshot() || I::EngineVGui->IsGameUIVisible())
		return Hook.Original<FN>()(ecx, edx, pSetup);

	F::Chams.RenderMain();
	F::Glow.RenderMain();

	F::Visuals.DrawBoxes();
	F::Visuals.DrawBulletLines();
	F::Visuals.DrawSimLines();
	F::Visuals.DrawSightlines();
	F::Visuals.ProjectileTrace();

	return Hook.Original<FN>()(ecx, edx, pSetup);
}