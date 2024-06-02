#include "NetworkFix.h"
#include "../../Hooks/HookManager.h"
#include "../../Hooks/Hooks.h"

class CReadPacketStateBackup
{
public:
    float m_flFrameTimeClientState;
    float m_flFrameTime;
    float m_flCurTime;
    int m_nTickCount;

    void Store()
    {
        m_flFrameTimeClientState = I::ClientState->m_frameTime;
        m_flFrameTime = I::GlobalVars->frametime;
        m_flCurTime = I::GlobalVars->curtime;
        m_nTickCount = I::GlobalVars->tickcount;
    }

    void Restore()
    {
        I::ClientState->m_frameTime = m_flFrameTimeClientState;
        I::GlobalVars->frametime = m_flFrameTime;
        I::GlobalVars->curtime = m_flCurTime;
        I::GlobalVars->tickcount = m_nTickCount;
    }
};

void CNetworkFix::FixInputDelay(bool bFinalTick)
{
    static auto fnCLReadPackets = g_HookManager.GetMapHooks()["CL_ReadPackets"];

    if (!I::EngineClient->IsInGame() ||
        !Vars::Misc::Game::NetworkFix.Value ||
        !fnCLReadPackets)
    {
        return;
    }

    auto netChannelInfo = I::EngineClient->GetNetChannelInfo();
    if (netChannelInfo && netChannelInfo->IsLoopback())
    {
        return;
    }

    CReadPacketStateBackup backup;
    backup.Store();

    fnCLReadPackets->Original<void(__cdecl*)(bool)>()(bFinalTick);

    m_State.Store();

    backup.Restore();
}

bool CNetworkFix::ShouldReadPackets()
{
    if (!I::EngineClient->IsInGame() || !Vars::Misc::Game::NetworkFix.Value)
    {
        return true;
    }

    auto netChannelInfo = I::EngineClient->GetNetChannelInfo();
    if (netChannelInfo && netChannelInfo->IsLoopback())
    {
        return true;
    }

    m_State.Restore();

    return false;
}
