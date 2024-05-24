#include "EnginePrediction.h"

#include "../TickHandler/TickHandler.h"

int CEnginePrediction::GetTickbase(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	static int nTick = 0;
	static CUserCmd* pLastCmd = nullptr;

	if (pCmd)
	{
		if (!pLastCmd || pLastCmd->hasbeenpredicted)
			nTick = pLocal->m_nTickBase();
		else
			nTick++;
		pLastCmd = pCmd;
	}

	return nTick;
}

void CEnginePrediction::Simulate(CUserCmd* pCmd)
{
	CBaseEntity* pLocal = g_EntityCache.GetLocal();
	if (!pLocal || !pLocal->IsAlive() || !I::MoveHelper)
		return;

	pLocal->SetCurrentCmd(pCmd);

	*I::RandomSeed = MD5_PseudoRandom(pCmd->command_number) & std::numeric_limits<int>::max();

	const int nOldTickBase = pLocal->m_nTickBase();
	const bool bOldIsFirstPrediction = I::Prediction->m_bFirstTimePredicted;
	const bool bOldInPrediction = I::Prediction->m_bInPrediction;

	I::Prediction->m_bFirstTimePredicted = false;
	I::Prediction->m_bInPrediction = true;

	I::Prediction->SetLocalViewAngles(pCmd->viewangles);

	I::Prediction->SetupMove(pLocal, pCmd, I::MoveHelper, &m_MoveData);
	//if (G::DoubleTap && G::CurWeaponType == EWeaponType::MELEE && pLocal->IsCharging() && F::Ticks.GetTicks(pLocal) <= 14) // demo charge fix pt 2
	//	m_MoveData.m_flMaxSpeed = m_MoveData.m_flClientMaxSpeed = pLocal->TeamFortress_CalculateMaxSpeed(true);
	I::GameMovement->ProcessMovement(pLocal, &m_MoveData);
	I::Prediction->FinishMove(pLocal, pCmd, &m_MoveData);

	pLocal->m_nTickBase() = nOldTickBase;
	I::Prediction->m_bFirstTimePredicted = bOldIsFirstPrediction;
	I::Prediction->m_bInPrediction = bOldInPrediction;
}

void CEnginePrediction::Start(CUserCmd* pCmd)
{
	CBaseEntity* pLocal = g_EntityCache.GetLocal();
	if (!pLocal || !pLocal->IsAlive() || !I::MoveHelper)
		return;

	m_nOldTickCount = I::GlobalVars->tickcount;
	m_fOldCurrentTime = I::GlobalVars->curtime;
	m_fOldFrameTime = I::GlobalVars->frametime;

	I::GlobalVars->tickcount = GetTickbase(pCmd, pLocal);
	I::GlobalVars->curtime = TICKS_TO_TIME(I::GlobalVars->tickcount);
	I::GlobalVars->frametime = I::Prediction->m_bEnginePaused ? 0.0f : TICK_INTERVAL;

	bSimulated = false;
	if (F::Ticks.GetTicks(pLocal) && Vars::CL_Move::Doubletap::AntiWarp.Value && pLocal->OnSolid())
		return; // hopefully more accurate eyepos while dting

	bSimulated = true;
	Simulate(pCmd);
}

void CEnginePrediction::End(CUserCmd* pCmd)
{
	CBaseEntity* pLocal = g_EntityCache.GetLocal();
	if (!pLocal || !pLocal->IsAlive() || !I::MoveHelper)
		return;

	I::MoveHelper->SetHost(nullptr);

	I::GlobalVars->tickcount = m_nOldTickCount;
	I::GlobalVars->curtime = m_fOldCurrentTime;
	I::GlobalVars->frametime = m_fOldFrameTime;

	pLocal->SetCurrentCmd(nullptr);

	*I::RandomSeed = -1;

	if (!bSimulated)
		Simulate(pCmd);
}