#include "NetworkFix.h"
#include "../../Hooks/HookManager.h"
#include "../../Hooks/Hooks.h"

void CReadPacketState::Store()
{
	m_flFrameTimeClientState = I::ClientState->m_frameTime;
	m_flFrameTime = I::GlobalVars->frametime;
	m_flCurTime = I::GlobalVars->curtime;
	m_nTickCount = I::GlobalVars->tickcount;
}

void CReadPacketState::Restore()
{
	I::ClientState->m_frameTime = m_flFrameTimeClientState;
	I::GlobalVars->frametime = m_flFrameTime;
	I::GlobalVars->curtime = m_flCurTime;
	I::GlobalVars->tickcount = m_nTickCount;
}

void CNetworkFix::FixInputDelay(bool bFinalTick)
{
	static auto fnCLReadPackets = g_HookManager.GetMapHooks()["CL_ReadPackets"];
	if (!I::EngineClient->IsInGame() || !Vars::Misc::Game::NetworkFix.Value || !fnCLReadPackets)
		return;

	if (INetChannel* iNetChan = I::EngineClient->GetNetChannelInfo())
	{
		if (iNetChan->IsLoopback())
			return;
	}

	CReadPacketState Backup = {};

	Backup.Store();

	fnCLReadPackets->Original<void(__cdecl*)(bool)>()(bFinalTick);

	m_State.Store();

	Backup.Restore();
}

bool CNetworkFix::ShouldReadPackets()
{
	if (!I::EngineClient->IsInGame() || !Vars::Misc::Game::NetworkFix.Value)
		return true;

	if (auto pNetChannel = I::EngineClient->GetNetChannelInfo())
	{
		if (pNetChannel->IsLoopback())
			return true;
	}

	m_State.Restore();

	return false;
}