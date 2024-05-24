#include "../Hooks.h"

#include "../../SDK/Includes/icons.h"
#include "../../Features/Visuals/ESP/ESP.h"
#include "../../Features/Visuals/Visuals.h"
#include "../../Features/Visuals/PlayerArrows/PlayerArrows.h"
#include "../../Features/CritHack/CritHack.h"
#include "../../Features/Visuals/SpectatorList/SpectatorList.h"
#include "../../Features/Visuals/Radar/Radar.h"
#include "../../Features/Menu/Menu.h"
#include "../../Features/Followbot/Followbot.h"
#include "../../Features/Misc/Notifications/Notifications.h"
#include "../../Features/AutoQueue/AutoQueue.h"
#include "../../Features/Menu/Playerlist/PlayerCore.h"

void Paint()
{
	if (!g_ScreenSize.w || !g_ScreenSize.h)
		g_ScreenSize.Update();

	{	// HACK: for some reason we need to do this
		static bool bInitIcons = false;

		if (!bInitIcons)
		{
			for (int nIndex = 0; nIndex < ICONS::TEXTURE_AMOUNT; nIndex++)
			{
				ICONS::ID[nIndex] = -1;
				g_Draw.Texture(-200, 0, 18, 18, nIndex);
			}

			bInitIcons = true;
		}
	}

	I::MatSystemSurface->StartDrawing();
	{
		F::PlayerCore.Run();
		if (Vars::Visuals::UI::CleanScreenshots.Value && I::EngineClient->IsTakingScreenshot())
			return I::MatSystemSurface->FinishDrawing();

		F::AutoQueue.Run();
		F::Notifications.Draw();

		if (I::EngineVGui->IsGameUIVisible())
			return I::MatSystemSurface->FinishDrawing();

		F::ESP.Run();
		F::Visuals.PickupTimers();
		F::PlayerArrows.Run();
		F::SpectatorList.Run();
		F::CritHack.Draw();
		F::Radar.Run();

		if (CBaseEntity* pLocal = g_EntityCache.GetLocal())
		{
			F::Visuals.DrawAntiAim(pLocal);
			F::Visuals.DrawTickbaseText();
			F::Visuals.DrawAimbotFOV(pLocal);
			F::Visuals.DrawSeedPrediction(pLocal);
			F::Visuals.DrawOnScreenConditions(pLocal);
			F::Visuals.DrawOnScreenPing(pLocal);
			F::Visuals.DrawServerHitboxes();
			F::Visuals.DrawDebugInfo(pLocal);
		}
	}
	I::MatSystemSurface->FinishDrawing();
}

MAKE_HOOK(EngineVGui_Paint, Utils::GetVFuncPtr(I::EngineVGui, 14), void, __fastcall,
	void* ecx, void* edx, int iMode)
{
	Hook.Original<FN>()(ecx, edx, iMode);

	if (iMode & PAINT_UIPANELS)
	{	
		CViewSetup viewSetup = {}; //Update W2S
		if (I::BaseClientDLL->GetPlayerView(viewSetup))
		{
			VMatrix worldToView = {}, viewToProjection = {}, worldToPixels = {};
			I::RenderView->GetMatricesForView(viewSetup, &worldToView, &viewToProjection, &G::WorldToProjection, &worldToPixels);
		}

		Paint();
	}
}