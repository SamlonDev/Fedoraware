#include "../Hooks.h"
#include "../../Features/PacketManip/AntiAim/AntiAim.h"

std::vector<Vec3> vAngles;

MAKE_HOOK(CPrediction_RunCommand, Utils::GetVFuncPtr(I::Prediction, 17), void, __fastcall,
	void* ecx, void* edx, CBaseEntity* pPlayer, CUserCmd* pCmd, CMoveHelper* moveHelper)
{
	Hook.Original<FN>()(ecx, edx, pPlayer, pCmd, moveHelper);

	// credits: KGB
	if (pPlayer != g_EntityCache.GetLocal() || G::Recharge || pCmd->hasbeenpredicted)
		return;

	const auto& pAnimState = pPlayer->GetAnimState();
	vAngles.push_back(G::ViewAngles);
	if (!pAnimState || G::Choking)
		return;

	for (auto& vAngle : vAngles)
	{
		pAnimState->Update(vAngle.y, vAngle.x);
		pPlayer->FrameAdvance(TICK_INTERVAL);
	}
	vAngles.clear();
}