#ifndef CMISC_H
#define CMISC_H

#include "sdk.h"

#include "../Interfaces.h"
#include "../Utils/Math.hpp"
#include "../Utils/Singleton.hpp"
#include "../Utils/Memory.hpp"
#include "../Utils/Convar.hpp"
#include "../Utils/Netvar.hpp"

#include "../Render/Render.h"

#include "../SDK/CUserCmd.hpp"
#include "../SDK/CBaseEntity.hpp"
#include "../SDK/CTraceFilter.hpp"
#include "../SDK/CGameTrace.hpp"
#include "../SDK/CBaseCombatWeapon.hpp"
#include "../SDK/CGameEvent.hpp"
#include "../SDK/CClientClass.hpp"

class CMisc : public Singleton<CMisc>
{
public:
    void RunPre(CUserCmd* pCmd) const;
    void RunPost(CUserCmd* pCmd, bool pSendPacket) const;

private:
    void AutoJump(CUserCmd* pCmd, CBaseEntity* pLocal) const;
    void AutoJumpbug(CUserCmd* pCmd, CBaseEntity* pLocal) const;
    void AutoStrafe(CUserCmd* pCmd, CBaseEntity* pLocal) const;
    void AntiBackstab(CUserCmd* pCmd, CBaseEntity* pLocal) const;
    void AutoPeek(CUserCmd* pCmd, CBaseEntity* pLocal) const;
    void AntiAFK(CUserCmd* pCmd, CBaseEntity* pLocal) const;
    void InstantRespawnMVM(CBaseEntity* pLocal) const;
    void CheatsBypass() const;
    void Event(CGameEvent* pEvent, FNV1A_t uNameHash) const;
    void PingReducer() const;
    void WeaponSway() const;

    void FastStop(CUserCmd* pCmd, CBaseEntity* pLocal) const;
    void FastAccel(CUserCmd* pCmd, CBaseEntity* pLocal) const;
    void FastStrafe(CUserCmd* pCmd, CBaseEntity* pLocal) const;
    void InstaStop(CUserCmd* pCmd) const;
    void StopMovement(CUserCmd* pCmd, CBaseEntity* pLocal) const;
    void LegJitter(CUserCmd* pCmd, bool pSendPacket, CBaseEntity* pLocal) const;

    void DoubletapPacket(CUserCmd* pCmd, bool* pSendPacket) const;

    void TauntKartControl(CUserCmd* pCmd, CBaseEntity* pLocal) const;

    void UnlockAchievements() const;
    void LockAchievements() const;

    void SteamRPC() const;

#ifdef DEBUG
    void DumpClassIDS() const;
#endif

    // Add other private member variables and functions here
};

#endif // CMISC_H
