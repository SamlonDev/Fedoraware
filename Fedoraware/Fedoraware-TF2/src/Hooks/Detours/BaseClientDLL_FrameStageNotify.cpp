#include "../Hooks.h"

#include "../../Features/Resolver/Resolver.h"
#include "../../Features/Visuals/Visuals.h"
#include "../../Features/Visuals/FakeAngle/FakeAngle.h"
#include "../../Features/Menu/Playerlist/PlayerUtils.h"
#include "../../Features/Backtrack/Backtrack.h"
#include "../../Features/Simulation/MovementSimulation/MovementSimulation.h"

MAKE_HOOK(BaseClientDLL_FrameStageNotify, Utils::GetVFuncPtr(I::BaseClientDLL, 35), void, __fastcall,
	void* ecx, void* edx, EClientFrameStage curStage)
{
	switch (curStage)
	{
	case EClientFrameStage::FRAME_RENDER_START:
		G::PunchAngles = Vec3();
		if (const auto& pLocal = g_EntityCache.GetLocal())
		{
			// Remove punch effect
			G::PunchAngles = pLocal->m_vecPunchAngle(); // use in aimbot 
			if (Vars::Visuals::Removals::ViewPunch.Value)
				pLocal->ClearPunchAngle(); // visual no-recoil
			F::Resolver.FrameStageNotify(pLocal);
		}
	}

	Hook.Original<FN>()(ecx, edx, curStage);

	switch (curStage)
	{
	case EClientFrameStage::FRAME_NET_UPDATE_START:
		g_EntityCache.Clear();

		break;
	case EClientFrameStage::FRAME_NET_UPDATE_END:
		g_EntityCache.Fill();
		F::PlayerUtils.UpdatePlayers();
		for (int n = 1; n <= I::EngineClient->GetMaxClients(); n++)
		{
			CBaseEntity* pEntity = I::ClientEntityList->GetClientEntity(n);
			if (!pEntity || !pEntity->IsPlayer() || n == I::EngineClient->GetLocalPlayer())
				continue; // local player managed in CPrediction_RunCommand

			static auto sv_maxusrcmdprocessticks = g_ConVars.FindVar("sv_maxusrcmdprocessticks");
			const int iTicks = sv_maxusrcmdprocessticks ? sv_maxusrcmdprocessticks->GetInt() : 24;
			if (auto iDifference = std::min(TIME_TO_TICKS(pEntity->m_flSimulationTime() - pEntity->m_flOldSimulationTime()), iTicks))
			{
				float flOldFrameTime = I::GlobalVars->frametime;
				I::GlobalVars->frametime = I::Prediction->m_bEnginePaused ? 0.f : TICK_INTERVAL;
				for (int i = 0; i < iDifference; i++)
				{
					G::UpdatingAnims = true;
					pEntity->UpdateClientSideAnimation();
					G::UpdatingAnims = false;
				}
				I::GlobalVars->frametime = flOldFrameTime;
			}
		}

		F::Backtrack.FrameStageNotify();
		F::MoveSim.FillVelocities();
		F::Visuals.FillSightlines();
		for (int n = 1; n <= I::EngineClient->GetMaxClients(); n++)
		{
			if (const auto& pPlayer = I::ClientEntityList->GetClientEntity(n))
				G::VelFixRecords[pPlayer] = { pPlayer->m_vecOrigin(), pPlayer->m_vecMaxs().z - pPlayer->m_vecMins().z, pPlayer->m_flSimulationTime() };
		}

		break;
	case EClientFrameStage::FRAME_RENDER_START:
		if (!G::UnloadWndProcHook)
		{
			F::Visuals.SkyboxChanger();
			F::Visuals.Modulate();
		}
	}
}