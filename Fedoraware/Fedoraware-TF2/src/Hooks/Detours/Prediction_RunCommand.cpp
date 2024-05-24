#include "../Hooks.h"

int CalculateTick(int simTicks, CBaseEntity* player)
{
	const int clockcorrect = TIME_TO_TICKS(0.06f); //sv_clockcorrectmsecs

	const int nIdealFinalTick = I::GlobalVars->tickcount + TIME_TO_TICKS(I::EngineClient->GetNetChannelInfo()->GetLatency(0)) + clockcorrect;

	const int estimatedFinal = player->m_nTickBase() + simTicks;

	const int fast = nIdealFinalTick + clockcorrect;
	const int slow = nIdealFinalTick - clockcorrect;

	if (estimatedFinal > fast || estimatedFinal < slow) { 
		return nIdealFinalTick - simTicks; 
	}

	// this is useless
	return estimatedFinal;
}

MAKE_HOOK(Prediction_RunCommand, Utils::GetVFuncPtr(I::Prediction, 17), void, __fastcall,
	void* ecx, void* edx, CBaseEntity* pEntity, CUserCmd* pCmd, CMoveHelper* pMoveHelper)
{
	if (pMoveHelper && !I::MoveHelper)
	{
		I::MoveHelper = pMoveHelper; /*MAKE_INTERFACE(IMoveHelper, MoveHelper, "client.dll", "8B 0D ? ? ? ? 8B 01 FF 50 28 56 8B C8", 2, 2);*/
	}

	const auto& pLocal = g_EntityCache.GetLocal();

	if (pEntity && pEntity->IsAlive() && pLocal && pLocal->IsAlive() && pLocal == pEntity && pCmd && pCmd->command_number)
	{
		const int backupTick = pEntity->m_nTickBase();
		const float curtimeBackup = I::GlobalVars->curtime;

		if (Vars::CL_Move::Doubletap::Enabled.Value && G::DoubleTap)
		{
			if (pCmd->command_number == G::LastUserCmd->command_number)
			{
				pEntity->m_nTickBase() = CalculateTick(I::ClientState->chokedcommands + G::ShiftedTicks + 1, pEntity);
				I::GlobalVars->curtime = TICKS_TO_TIME(pEntity->m_nTickBase());
			}
		}

		Hook.Original<FN>()(ecx, edx, pEntity, pCmd, pMoveHelper);

		if (Vars::CL_Move::Doubletap::Enabled.Value && G::DoubleTap)
		{
			if (pCmd->command_number == G::LastUserCmd->command_number)
			{
				pEntity->m_nTickBase() = backupTick;
				I::GlobalVars->curtime = curtimeBackup;
			}
		}
	}
	else
	{
		Hook.Original<FN>()(ecx, edx, pEntity, pCmd, pMoveHelper);
	}
}