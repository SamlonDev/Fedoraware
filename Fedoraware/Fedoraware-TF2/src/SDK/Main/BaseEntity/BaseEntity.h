#pragma once
#include "../../Interfaces/Interfaces.h"
#include "../../NetVars/NetVars.h"

#include "AnimState/TFPlayerAnimState.h" //includes class C_BaseCombatWeapon

namespace S
{
	MAKE_SIGNATURE(CBaseEntity_SetAbsOrigin, CLIENT_DLL, "55 8B EC 56 57 8B F1 E8 ? ? ? ? 8B 7D ? F3 0F 10 07", 0x0);
	MAKE_SIGNATURE(CBaseEntity_GetVelocity, CLIENT_DLL, "55 8B EC 83 EC ? 56 8B F1 E8 ? ? ? ? 3B F0", 0x0);
	MAKE_SIGNATURE(CBaseEntity_CreateShadow, CLIENT_DLL, "56 8B F1 8B 46 04 8D 4E 04 57 FF 50 7C 8B C8 85 C9 75 27", 0x0);
	MAKE_SIGNATURE(CBaseEntity_InvalidateBoneCache, CLIENT_DLL, "A1 ? ? ? ? 48 C7 81 60 08 00 00 FF FF 7F FF", 0x0);
	MAKE_SIGNATURE(CBaseEntity_PostDataUpdate, CLIENT_DLL, "55 8B EC 83 EC ? 53 8B 5D ? 56 57 8B F9 85 DB 75 ? 8B 0D", 0x0);
	MAKE_SIGNATURE(CBaseEntity_SetNextThink, CLIENT_DLL, "55 8B EC F3 0F 10 45 ? 0F 2E 05 ? ? ? ? 53", 0x0);
	MAKE_SIGNATURE(CBaseEntity_GetNextThinkTick, CLIENT_DLL, "55 8B EC 8B 45 ? 56 8B F1 85 C0 75 ? 8B 86", 0x0);
	MAKE_SIGNATURE(CBaseEntity_PhysicsRunThink, CLIENT_DLL, "55 8B EC 53 8B D9 56 57 8B 83 ? ? ? ? C1 E8", 0x0);
	MAKE_SIGNATURE(CBaseEntity_UpdateButtonState, CLIENT_DLL, "55 8B EC 8B 81 ? ? ? ? 8B D0", 0x0);
	MAKE_SIGNATURE(CBaseEntity_RemoveEffect, CLIENT_DLL, "55 8B EC 56 FF 75 ? 8B F1 B8", 0x0);

	MAKE_SIGNATURE(CBasePlayer_GetAmmoCount, CLIENT_DLL, "55 8B EC 56 8B 75 08 57 8B F9 83 FE FF 75 08 5F 33 C0 5E 5D", 0x0);

	MAKE_SIGNATURE(TeamFortress_CalculateMaxSpeed, CLIENT_DLL, "55 8B EC 83 EC ? 83 3D ? ? ? ? ? 56 8B F1 75", 0x0);
	MAKE_SIGNATURE(CTFPlayer_UpdateWearables, CLIENT_DLL, "56 8B F1 E8 ? ? ? ? 8B 06 8B CE 6A 00 68 ? ? ? ? 68 ? ? ? ? 6A 00 FF 90 ? ? ? ? 50 E8 ? ? ? ? 83 C4 14", 0x0);
	MAKE_SIGNATURE(CTFPlayer_ThirdPersonSwitch, CLIENT_DLL, "E8 ? ? ? ? 8A 87 ? ? ? ? 88 87 ? ? ? ?", 0x0);

	MAKE_SIGNATURE(CBaseAnimating_GetBonePosition, CLIENT_DLL, "55 8B EC 83 EC 30 56 6A 00 8B F1 E8", 0x0);
}

enum ShouldTransmitState_t
{
	SHOULDTRANSMIT_START = 0,
	SHOULDTRANSMIT_END
};


// has all these in them. Left it as an enum in case we want to go back though
enum DataUpdateType_t
{
	DATA_UPDATE_CREATED = 0,	// indicates it was created +and+ entered the pvs
	//	DATA_UPDATE_ENTERED_PVS,
	DATA_UPDATE_DATATABLE_CHANGED,
	//	DATA_UPDATE_LEFT_PVS,
	//	DATA_UPDATE_DESTROYED,		// FIXME: Could enable this, but it's a little worrying
									// since it changes a bunch of existing code
};

class IClientNetworkable;
class CBaseEntity;
class IClientRenderable;
class ICollideable;
class IClientEntity;
class IClientThinkable;

class IClientUnknown : public IHandleEntity
{
public:
	virtual ICollideable* GetCollideable() = 0;
	virtual IClientNetworkable* GetClientNetworkable() = 0;
	virtual IClientRenderable* GetClientRenderable() = 0;
	virtual IClientEntity* GetIClientEntity() = 0;
	virtual CBaseEntity* GetBaseEntity() = 0;
	virtual IClientThinkable* GetClientThinkable() = 0;
};

class IClientNetworkable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual void Release() = 0;
	virtual CClientClass* GetClientClass() = 0;
	virtual void NotifyShouldTransmit(ShouldTransmitState_t state) = 0;
	virtual void OnPreDataChanged(DataUpdateType_t updateType) = 0;
	virtual void OnDataChanged(DataUpdateType_t updateType) = 0;
	virtual void PreDataUpdate(DataUpdateType_t updateType) = 0;
	virtual void PostDataUpdate(DataUpdateType_t updateType) = 0;
	virtual bool IsDormant(void) = 0;
	virtual int	entindex(void) const = 0;
	virtual void ReceiveMessage(int classID, bf_read& msg) = 0;
	virtual void* GetDataTableBasePtr() = 0;
	virtual void SetDestroyedOnRecreateEntities(void) = 0;
	virtual void OnDataUnchangedInPVS() = 0;
};

class CBaseEntity
{
public:
	// thanks litebase this is just better ngl
	NETVAR(m_flAnimTime, float, "CBaseEntity", "m_flAnimTime")
	NETVAR(m_flSimulationTime, float, "CBaseEntity", "m_flSimulationTime")
	NETVAR_OFF(m_flOldSimulationTime, float, "CBaseEntity", "m_flSimulationTime", 4)
	NETVAR(m_ubInterpolationFrame, int, "CBaseEntity", "m_ubInterpolationFrame")
	NETVAR(m_vecOrigin, Vec3, "CBaseEntity", "m_vecOrigin")
	NETVAR(m_angRotation, Vec3, "CBaseEntity", "m_angRotation")
	NETVAR(m_nModelIndex, int, "CBaseEntity", "m_nModelIndex")
	NETVAR(m_fEffects, int, "CBaseEntity", "m_fEffects")
	NETVAR(m_nRenderMode, int, "CBaseEntity", "m_nRenderMode")
	NETVAR(m_nRenderFX, int, "CBaseEntity", "m_nRenderFX")
	NETVAR(m_clrRender, Color_t, "CBaseEntity", "m_clrRender")
	NETVAR(m_iTeamNum, int, "CBaseEntity", "m_iTeamNum")
	NETVAR(m_CollisionGroup, int, "CBaseEntity", "m_CollisionGroup")
	NETVAR(m_flElasticity, float, "CBaseEntity", "m_flElasticity")
	NETVAR(m_flShadowCastDistance, float, "CBaseEntity", "m_flShadowCastDistance")
	NETVAR(m_hOwnerEntity, int /*EHANDLE*/, "CBaseEntity", "m_hOwnerEntity")
	NETVAR(m_hEffectEntity, int /*EHANDLE*/, "CBaseEntity", "m_hEffectEntity")
	NETVAR(moveparent, int, "CBaseEntity", "moveparent")
	NETVAR(m_iParentAttachment, int, "CBaseEntity", "m_iParentAttachment")
	NETVAR(m_Collision, void*, "CBaseEntity", "m_Collision")
	NETVAR(m_vecMinsPreScaled, Vec3, "CBaseEntity", "m_vecMinsPreScaled")
	NETVAR(m_vecMaxsPreScaled, Vec3, "CBaseEntity", "m_vecMaxsPreScaled")
	NETVAR(m_vecMins, Vec3, "CBaseEntity", "m_vecMins")
	NETVAR(m_vecMaxs, Vec3, "CBaseEntity", "m_vecMaxs")
	NETVAR(m_nSolidType, int, "CBaseEntity", "m_nSolidType")
	NETVAR(m_usSolidFlags, int, "CBaseEntity", "m_usSolidFlags")
	NETVAR(m_nSurroundType, int, "CBaseEntity", "m_nSurroundType")
	NETVAR(m_triggerBloat, int, "CBaseEntity", "m_triggerBloat")
	NETVAR(m_bUniformTriggerBloat, bool, "CBaseEntity", "m_bUniformTriggerBloat")
	NETVAR(m_vecSpecifiedSurroundingMinsPreScaled, Vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMinsPreScaled")
	NETVAR(m_vecSpecifiedSurroundingMaxsPreScaled, Vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMaxsPreScaled")
	NETVAR(m_vecSpecifiedSurroundingMins, Vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMins")
	NETVAR(m_vecSpecifiedSurroundingMaxs, Vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMaxs")
	NETVAR(m_iTextureFrameIndex, int, "CBaseEntity", "m_iTextureFrameIndex")
	NETVAR(m_PredictableID, int, "CBaseEntity", "m_PredictableID")
	NETVAR(m_bIsPlayerSimulated, bool, "CBaseEntity", "m_bIsPlayerSimulated")
	NETVAR(m_bSimulatedEveryTick, bool, "CBaseEntity", "m_bSimulatedEveryTick")
	NETVAR(m_bAnimatedEveryTick, bool, "CBaseEntity", "m_bAnimatedEveryTick")
	NETVAR(m_bAlternateSorting, bool, "CBaseEntity", "m_bAlternateSorting")
	NETVAR(m_nModelIndexOverrides, void*, "CBaseEntity", "m_nModelIndexOverrides")
	NETVAR(movetype, int, "CBaseEntity", "movetype")
	VIRTUAL(AbsOrigin, Vec3&, this, Vec3& (__thiscall*)(void*), 0x9)
	VIRTUAL(AbsAngles, Vec3&, this, Vec3& (__thiscall*)(void*), 0xa)
	__inline CCollisionProperty* GetCollision()
	{
		return reinterpret_cast<CCollisionProperty*>(this + 0x1C8);
	}
	__inline const char* GetModelName()
	{
		return I::ModelInfoClient->GetModelName(GetModel());
	}
	__inline ETFClassID GetClassID()
	{
		const auto& pCC = GetClientClass();
		return pCC ? ETFClassID(pCC->m_ClassID) : ETFClassID(0);
	}
	__inline bool IsSentrygun()
	{
		return GetClassID() == ETFClassID::CObjectSentrygun;
	}
	__inline bool IsDispenser()
	{
		return GetClassID() == ETFClassID::CObjectDispenser;
	}
	__inline bool IsTeleporter()
	{
		return GetClassID() == ETFClassID::CObjectTeleporter;
	}
	__inline bool IsBaseCombatWeapon()
	{
		return GetClassID() == ETFClassID::CBaseCombatWeapon;
	}
	__inline bool IsWearable()
	{
		return GetClassID() == ETFClassID::CTFWearable;
	}
	__inline bool IsPlayer()
	{
		return GetClassID() == ETFClassID::CTFPlayer;
	}
	__inline bool IsBuilding()
	{
		switch (GetClassID())
		{
		case ETFClassID::CObjectDispenser:
		case ETFClassID::CObjectSentrygun:
		case ETFClassID::CObjectTeleporter: return true;
		default: return false;
		}
	}
	__inline bool IsPickup()
	{
		switch (GetClassID())
		{
		case ETFClassID::CBaseAnimating: return GetModelName()[24] != 'h';
		case ETFClassID::CTFAmmoPack: return true;
		default: return false;
		}
	}
	__inline bool IsNPC()
	{
		switch (GetClassID())
		{
		case ETFClassID::CHeadlessHatman:
		case ETFClassID::CTFTankBoss:
		case ETFClassID::CMerasmus:
		case ETFClassID::CZombie:
		case ETFClassID::CEyeballBoss:
			return true;
		default: return false;
		}
	}
	__inline bool IsBomb()
	{
		switch (GetClassID())
		{
		case ETFClassID::CTFPumpkinBomb:
		case ETFClassID::CTFGenericBomb:
			return true;
		default: return false;
		}
	}
	__inline Vec3 GetCenter()
	{
		Vec3 vMin = {}, vMax = {};
		GetRenderBounds(vMin, vMax);
		return m_vecOrigin() + Vec3(0.f, 0.f, (vMin.z + vMax.z) / 2.f);
	}
	__inline bool IsInValidTeam()
	{
		const int nTeam = m_iTeamNum();
		return nTeam == 2 || nTeam == 3;
	}
	__inline CBaseEntity* GetMoveParent()
	{
		return I::ClientEntityList->GetClientEntity(*reinterpret_cast<int*>(this + 0x1B8) & 0xFFF);
	}
	__inline CBaseEntity* FirstMoveChild()
	{
		return I::ClientEntityList->GetClientEntity(*reinterpret_cast<int*>(this + 0x1B0) & 0xFFF);
	}
	__inline CBaseEntity* NextMovePeer()
	{
		return I::ClientEntityList->GetClientEntity(*reinterpret_cast<int*>(this + 0x1B4) & 0xFFF);
	}
	__inline void UpdateVisibility()
	{
		GetVFunc<void(__thiscall*)(CBaseEntity*)>(this, 91)(this);
	}
	__inline byte& m_MoveType()
	{
		static int nOffset = 420;
		return *reinterpret_cast<byte*>(reinterpret_cast<DWORD>(this) + nOffset);
	}
	__inline byte& m_MoveCollide()
	{
		static int nOffset = 421;
		return *reinterpret_cast<byte*>(reinterpret_cast<DWORD>(this) + nOffset);
	}
	__inline float& m_flGravity()
	{
		static int nOffset = 400;
		return *reinterpret_cast<float*>(reinterpret_cast<DWORD>(this) + nOffset);
	}
	__inline byte& m_nWaterLevel_C_BaseEntity()
	{
		static int nOffset = 424;
		return *reinterpret_cast<byte*>(reinterpret_cast<DWORD>(this) + nOffset);
	}
	__inline byte& m_nWaterType()
	{
		static int nOffset = 425;
		return *reinterpret_cast<byte*>(reinterpret_cast<DWORD>(this) + nOffset);
	}
	__inline void SetAbsOrigin(const Vec3& vOrigin)
	{
		S::CBaseEntity_SetAbsOrigin.As<void(__thiscall*)(CBaseEntity*, const Vec3&)>()(this, vOrigin);
	}
	__inline void SetAbsAngles(const Vec3& vAngles)
	{
		Vec3* pAbsAngles = const_cast<Vec3*>(&GetAbsAngles());
		*pAbsAngles = vAngles;
	}
	__inline void SetAbsVelocity(const Vector& vVelocity)
	{
		S::CBaseEntity_SetAbsVelocity.As<void(__thiscall*)(CBaseEntity*, const Vec3&)>()(this, vVelocity);
	}
	__inline Vec3 GetVelocity()
	{
		Vec3 out;
		S::CBaseEntity_GetVelocity.As<void(__thiscall*)(CBaseEntity*, Vec3&)>()(this, out);
		return out;
	}
	__inline void CreateShadow()
	{
		S::CBaseEntity_CreateShadow.As<void(__thiscall*)(void*)>()(this);
	}
	__inline void InvalidateBoneCache()
	{
		S::CBaseEntity_InvalidateBoneCache.As<void(__thiscall*)(void*)>()(this);
	}
	__inline void SetNextThink(float thinkTime, const char* szContext)
	{
		S::CBaseEntity_SetNextThink.As<void(__thiscall*)(void*, float, const char*)>()(this, thinkTime, szContext);
	}
	__inline int GetNextThinkTick(const char* szContext)
	{
		return S::CBaseEntity_GetNextThinkTick.As<int(__thiscall*)(void*, const char*)>()(this, szContext);
	}
	__inline bool PhysicsRunThink(int thinkMethod = 0)
	{
		return S::CBaseEntity_PhysicsRunThink.As<bool(__thiscall*)(void*, int)>()(this, thinkMethod);
	}
	__inline void SelectItem(const char* pstr, int iSubType)
	{
		typedef bool(__thiscall* FN)(PVOID, const char*, int);
		GetVFunc<FN>(this, 270)(this, pstr, iSubType);
	}
	__inline void PreThink()
	{
		typedef bool(__thiscall* FN)(PVOID);
		GetVFunc<FN>(this, 260)(this);
	}
	__inline void Think()
	{
		typedef bool(__thiscall* FN)(PVOID);
		GetVFunc<FN>(this, 174)(this);
	}
	__inline void PostThink()
	{
		typedef bool(__thiscall* FN)(PVOID);
		GetVFunc<FN>(this, 261)(this);
	}
	__inline void PostDataUpdate(int updateType)
	{
		S::CBaseEntity_PostDataUpdate.As<void(__thiscall*)(CBaseEntity*, int)>()(this, updateType);
	}
	__inline void UpdateButtonState(int nUserCmdButtonMask)
	{
		S::CBaseEntity_UpdateButtonState.As<void(__thiscall*)(void*, int)>()(this, nUserCmdButtonMask);
	}
	__inline void RemoveEffect(const BYTE Effect)
	{
		*reinterpret_cast<byte*>(this + 0x7C) &= ~Effect;

		if (Effect == EF_NODRAW)
		{
			static auto fnAddToLeafSystem = S::CBaseEntity_RemoveEffect.As<int(__thiscall*)(PVOID, int)>();
			if (fnAddToLeafSystem)
				fnAddToLeafSystem(this, RENDER_GROUP_OPAQUE_ENTITY);
		}
	}

public:
	NETVAR(m_Local, void*, "CBasePlayer", "m_Local")
	NETVAR(m_chAreaBits, void*, "CBasePlayer", "m_chAreaBits")
	NETVAR(m_chAreaPortalBits, void*, "CBasePlayer", "m_chAreaPortalBits")
	NETVAR(m_iHideHUD, int, "CBasePlayer", "m_iHideHUD")
	NETVAR(m_flFOVRate, float, "CBasePlayer", "m_flFOVRate")
	NETVAR(m_bDucked, bool, "CBasePlayer", "m_bDucked")
	NETVAR(m_bDucking, bool, "CBasePlayer", "m_bDucking")
	NETVAR(m_bInDuckJump, bool, "CBasePlayer", "m_bInDuckJump")
	NETVAR(m_flDucktime, float, "CBasePlayer", "m_flDucktime")
	NETVAR(m_flDuckJumpTime, float, "CBasePlayer", "m_flDuckJumpTime")
	NETVAR(m_flJumpTime, float, "CBasePlayer", "m_flJumpTime")
	NETVAR(m_flFallVelocity, float, "CBasePlayer", "m_flFallVelocity")
	NETVAR(m_vecPunchAngle, Vec3, "CBasePlayer", "m_vecPunchAngle")
	NETVAR(m_vecPunchAngleVel, Vec3, "CBasePlayer", "m_vecPunchAngleVel")
	NETVAR(m_bDrawViewmodel, bool, "CBasePlayer", "m_bDrawViewmodel")
	NETVAR(m_bWearingSuit, bool, "CBasePlayer", "m_bWearingSuit")
	NETVAR(m_bPoisoned, bool, "CBasePlayer", "m_bPoisoned")
	NETVAR(m_flStepSize, float, "CBasePlayer", "m_flStepSize")
	NETVAR(m_bAllowAutoMovement, bool, "CBasePlayer", "m_bAllowAutoMovement")
	NETVAR(m_vecViewOffset, Vec3, "CBasePlayer", "m_vecViewOffset[0]")
	NETVAR(m_flFriction, float, "CBasePlayer", "m_flFriction")
	NETVAR(m_iAmmo, int/*void**/, "CBasePlayer", "m_iAmmo")
	NETVAR(m_fOnTarget, int, "CBasePlayer", "m_fOnTarget")
	NETVAR(m_nTickBase, int, "CBasePlayer", "m_nTickBase")
	NETVAR(m_nNextThinkTick, int, "CBasePlayer", "m_nNextThinkTick")
	NETVAR(m_hLastWeapon, int /*EHANDLE*/, "CBasePlayer", "m_hLastWeapon")
	NETVAR(m_hGroundEntity, int /*EHANDLE*/, "CBasePlayer", "m_hGroundEntity")
	NETVAR(m_vecVelocity, Vec3, "CBasePlayer", "m_vecVelocity[0]")
	NETVAR(m_vecBaseVelocity, Vec3, "CBasePlayer", "m_vecBaseVelocity")
	NETVAR(m_hConstraintEntity, CUserCmd* /*int handle*/, "CBasePlayer", "m_hConstraintEntity")
	NETVAR_OFF(m_nButtons, int, "CBasePlayer", "m_hConstraintEntity", -8)
	NETVAR_OFF(m_afButtonLast, int, "CBasePlayer", "m_hConstraintEntity", -20)
	NETVAR(m_vecConstraintCenter, Vec3, "CBasePlayer", "m_vecConstraintCenter")
	NETVAR(m_flConstraintRadius, float, "CBasePlayer", "m_flConstraintRadius")
	NETVAR(m_flConstraintWidth, float, "CBasePlayer", "m_flConstraintWidth")
	NETVAR(m_flConstraintSpeedFactor, float, "CBasePlayer", "m_flConstraintSpeedFactor")
	NETVAR(m_flDeathTime, float, "CBasePlayer", "m_flDeathTime")
	NETVAR(m_nWaterLevel, byte, "CBasePlayer", "m_nWaterLevel")
	NETVAR(m_flLaggedMovementValue, float, "CBasePlayer", "m_flLaggedMovementValue")
	NETVAR(m_AttributeList, void*, "CBasePlayer", "m_AttributeList")
	NETVAR(pl, void*, "CBasePlayer", "pl")
	NETVAR(deadflag, int, "CBasePlayer", "deadflag")
	NETVAR(m_iFOV, int, "CBasePlayer", "m_iFOV")
	NETVAR(m_iFOVStart, int, "CBasePlayer", "m_iFOVStart")
	NETVAR(m_flFOVTime, float, "CBasePlayer", "m_flFOVTime")
	NETVAR(m_iDefaultFOV, int, "CBasePlayer", "m_iDefaultFOV")
	NETVAR(m_hZoomOwner, int /*EHANDLE*/, "CBasePlayer", "m_hZoomOwner")
	NETVAR(m_hVehicle, int /*EHANDLE*/, "CBasePlayer", "m_hVehicle")
	NETVAR(m_hUseEntity, int /*EHANDLE*/, "CBasePlayer", "m_hUseEntity")
	NETVAR(m_iHealth, int, "CBasePlayer", "m_iHealth")
	NETVAR(m_lifeState, byte, "CBasePlayer", "m_lifeState")
	NETVAR(m_iBonusProgress, int, "CBasePlayer", "m_iBonusProgress")
	NETVAR(m_iBonusChallenge, int, "CBasePlayer", "m_iBonusChallenge")
	NETVAR(m_flMaxspeed, float, "CBasePlayer", "m_flMaxspeed")
	NETVAR(m_fFlags, int, "CBasePlayer", "m_fFlags")
	NETVAR(m_iObserverMode, int, "CBasePlayer", "m_iObserverMode")
	NETVAR(m_hObserverTarget, int /*EHANDLE*/, "CBasePlayer", "m_hObserverTarget")
	NETVAR(m_hViewModel, int /*EHANDLE*/, "CBasePlayer", "m_hViewModel[0]")
	NETVAR(m_szLastPlaceName, const char*, "CBasePlayer", "m_szLastPlaceName")
	__inline bool IsAlive()
	{
		return m_lifeState() == LIFE_ALIVE;
	}
	__inline void ClearPunchAngle()
	{	//m_vecPunchAngle
		*reinterpret_cast<Vec3*>(this + 0xE8C) = Vec3(0.f, 0.f, 0.f);
	}
	__inline float& m_flWaterJumpTime()
	{
		return *reinterpret_cast<float*>(reinterpret_cast<DWORD>(this) + 0x1120);
	}
	__inline float& m_flSwimSoundTime()
	{
		return *reinterpret_cast<float*>(reinterpret_cast<DWORD>(this) + 0x1128);
	}
	__inline Vec3& m_vecLadderNormal()
	{
		return *reinterpret_cast<Vec3*>(reinterpret_cast<DWORD>(this) + 0x112C);
	}
	__inline void SetCurrentCmd(CUserCmd* pCmd)
	{
		static int nOffset = g_NetVars.GetNetVar("CBasePlayer", "m_hConstraintEntity") - 4;
		*reinterpret_cast<CUserCmd**>(reinterpret_cast<DWORD>(this) + nOffset) = pCmd;
	}
	__inline int& m_surfaceProps()
	{
		return *reinterpret_cast<int*>(reinterpret_cast<DWORD>(this) + 0x12A0);
	}
	__inline void*& m_pSurfaceData()
	{
		return *reinterpret_cast<void**>(reinterpret_cast<DWORD>(this) + 0x12A4);
	}
	__inline float& m_surfaceFriction()
	{
		return *reinterpret_cast<float*>(reinterpret_cast<DWORD>(this) + 0x12A8);
	}
	__inline char& m_chTextureType()
	{
		return *reinterpret_cast<char*>(reinterpret_cast<DWORD>(this) + 0x12AC);
	}
	__inline int GetAmmoCount(int iAmmoType)
	{
		return S::CBasePlayer_GetAmmoCount.As<int(__thiscall*)(CBaseEntity*, int)>()(this, iAmmoType);
	}
	__inline float TickVelocity2D()
	{	// bad
		const int iDivide = floor(1000.f / I::GlobalVars->interval_per_tick);
		const float flVel = m_vecVelocity().Length2D();
		return flVel / iDivide;
	}
	__inline IClientNetworkable* Networkable()
	{
		return reinterpret_cast<IClientNetworkable*>((reinterpret_cast<uintptr_t>(this) + 0x8));
	}
	VIRTUAL(ClientClass, CClientClass*, Networkable(), CClientClass* (__thiscall*)(void*), 2)
	VIRTUAL(Dormant, bool, Networkable(), bool(__thiscall*)(void*), 8)
	VIRTUAL(Index, int, Networkable(), int(__thiscall*)(void*), 9)

public:
	NETVAR(m_flNextAttack, float, "CBaseCombatCharacter", "m_flNextAttack")
	NETVAR(m_hActiveWeapon, int /*EHANDLE*/, "CBaseCombatCharacter", "m_hActiveWeapon")
	NETVAR(m_hMyWeapons, int /*EHANDLE*/, "CBaseCombatCharacter", "m_hMyWeapons")
	NETVAR(m_bGlowEnabled, bool, "CBaseCombatCharacter", "m_bGlowEnabled")
	__inline CBaseCombatWeapon* GetActiveWeapon()
	{
		return reinterpret_cast<CBaseCombatWeapon*>(I::ClientEntityList->GetClientEntityFromHandle(m_hActiveWeapon()));
	}

public:
	NETVAR(m_bSaveMeParity, bool, "CTFPlayer", "m_bSaveMeParity")
	NETVAR(m_bIsMiniBoss, bool, "CTFPlayer", "m_bIsMiniBoss")
	NETVAR(m_bIsABot, bool, "CTFPlayer", "m_bIsABot")
	NETVAR(m_nBotSkill, int, "CTFPlayer", "m_nBotSkill")
	NETVAR(m_nTFWaterLevel, byte, "CTFPlayer", "m_nWaterLevel")
	NETVAR(m_hRagdoll, int /*EHANDLE*/, "CTFPlayer", "m_hRagdoll")
	NETVAR(m_PlayerClass, void*, "CTFPlayer", "m_PlayerClass")
	NETVAR(m_iClass, int, "CTFPlayer", "m_iClass")
	NETVAR(m_iszClassIcon, const char*, "CTFPlayer", "m_iszClassIcon")
	NETVAR(m_iszCustomModel, const char*, "CTFPlayer", "m_iszCustomModel")
	NETVAR(m_vecCustomModelOffset, Vec3, "CTFPlayer", "m_vecCustomModelOffset")
	NETVAR(m_angCustomModelRotation, Vec3, "CTFPlayer", "m_angCustomModelRotation")
	NETVAR(m_bCustomModelRotates, bool, "CTFPlayer", "m_bCustomModelRotates")
	NETVAR(m_bCustomModelRotationSet, bool, "CTFPlayer", "m_bCustomModelRotationSet")
	NETVAR(m_bCustomModelVisibleToSelf, bool, "CTFPlayer", "m_bCustomModelVisibleToSelf")
	NETVAR(m_bUseClassAnimations, bool, "CTFPlayer", "m_bUseClassAnimations")
	NETVAR(m_iClassModelParity, int, "CTFPlayer", "m_iClassModelParity")
	NETVAR(m_Shared, void*, "CTFPlayer", "m_Shared")
	NETVAR(m_nPlayerCond, int, "CTFPlayer", "m_nPlayerCond")
	NETVAR(m_bJumping, bool, "CTFPlayer", "m_bJumping")
	NETVAR(m_nNumHealers, int, "CTFPlayer", "m_nNumHealers")
	NETVAR(m_iCritMult, int, "CTFPlayer", "m_iCritMult")
	NETVAR(m_iAirDash, int, "CTFPlayer", "m_iAirDash")
	NETVAR(m_nAirDucked, int, "CTFPlayer", "m_nAirDucked")
	NETVAR(m_flDuckTimer, float, "CTFPlayer", "m_flDuckTimer")
	NETVAR(m_nPlayerState, int, "CTFPlayer", "m_nPlayerState")
	NETVAR(m_iDesiredPlayerClass, int, "CTFPlayer", "m_iDesiredPlayerClass")
	NETVAR(m_flMovementStunTime, float, "CTFPlayer", "m_flMovementStunTime")
	NETVAR(m_iMovementStunAmount, int, "CTFPlayer", "m_iMovementStunAmount")
	NETVAR(m_iMovementStunParity, int, "CTFPlayer", "m_iMovementStunParity")
	NETVAR(m_hStunner, int /*EHANDLE*/, "CTFPlayer", "m_hStunner")
	NETVAR(m_iStunFlags, int, "CTFPlayer", "m_iStunFlags")
	NETVAR(m_nArenaNumChanges, int, "CTFPlayer", "m_nArenaNumChanges")
	NETVAR(m_bArenaFirstBloodBoost, bool, "CTFPlayer", "m_bArenaFirstBloodBoost")
	NETVAR(m_iWeaponKnockbackID, int, "CTFPlayer", "m_iWeaponKnockbackID")
	NETVAR(m_bLoadoutUnavailable, bool, "CTFPlayer", "m_bLoadoutUnavailable")
	NETVAR(m_iItemFindBonus, int, "CTFPlayer", "m_iItemFindBonus")
	NETVAR(m_bShieldEquipped, bool, "CTFPlayer", "m_bShieldEquipped")
	NETVAR(m_bParachuteEquipped, bool, "CTFPlayer", "m_bParachuteEquipped")
	NETVAR(m_iNextMeleeCrit, int, "CTFPlayer", "m_iNextMeleeCrit")
	NETVAR(m_iDecapitations, int, "CTFPlayer", "m_iDecapitations")
	NETVAR(m_iRevengeCrits, int, "CTFPlayer", "m_iRevengeCrits")
	NETVAR(m_iDisguiseBody, int, "CTFPlayer", "m_iDisguiseBody")
	NETVAR(m_hCarriedObject, int /*EHANDLE*/, "CTFPlayer", "m_hCarriedObject")
	NETVAR(m_bCarryingObject, bool, "CTFPlayer", "m_bCarryingObject")
	NETVAR(m_flNextNoiseMakerTime, float, "CTFPlayer", "m_flNextNoiseMakerTime")
	NETVAR(m_iSpawnRoomTouchCount, int, "CTFPlayer", "m_iSpawnRoomTouchCount")
	NETVAR(m_iKillCountSinceLastDeploy, int, "CTFPlayer", "m_iKillCountSinceLastDeploy")
	NETVAR(m_flFirstPrimaryAttack, float, "CTFPlayer", "m_flFirstPrimaryAttack")
	NETVAR(m_flEnergyDrinkMeter, float, "CTFPlayer", "m_flEnergyDrinkMeter")
	NETVAR(m_flHypeMeter, float, "CTFPlayer", "m_flHypeMeter")
	NETVAR(m_flChargeMeter, float, "CTFPlayer", "m_flChargeMeter")
	NETVAR(m_flInvisChangeCompleteTime, float, "CTFPlayer", "m_flInvisChangeCompleteTime")
	NETVAR_OFF(m_flInvisibility, float, "CTFPlayer", "m_flInvisChangeCompleteTime", -8)
	NETVAR(m_nDisguiseTeam, int, "CTFPlayer", "m_nDisguiseTeam")
	NETVAR(m_nDisguiseClass, int, "CTFPlayer", "m_nDisguiseClass")
	NETVAR(m_nDisguiseSkinOverride, int, "CTFPlayer", "m_nDisguiseSkinOverride")
	NETVAR(m_nMaskClass, int, "CTFPlayer", "m_nMaskClass")
	NETVAR(m_iDisguiseTargetIndex, int, "CTFPlayer", "m_iDisguiseTargetIndex")
	NETVAR(m_iDisguiseHealth, int, "CTFPlayer", "m_iDisguiseHealth")
	NETVAR(m_bFeignDeathReady, bool, "CTFPlayer", "m_bFeignDeathReady")
	NETVAR(m_hDisguiseWeapon, int /*EHANDLE*/, "CTFPlayer", "m_hDisguiseWeapon")
	NETVAR(m_nTeamTeleporterUsed, int, "CTFPlayer", "m_nTeamTeleporterUsed")
	NETVAR(m_flCloakMeter, float, "CTFPlayer", "m_flCloakMeter")
	NETVAR(m_flSpyTranqBuffDuration, float, "CTFPlayer", "m_flSpyTranqBuffDuration")
	NETVAR(m_nDesiredDisguiseTeam, int, "CTFPlayer", "m_nDesiredDisguiseTeam")
	NETVAR(m_nDesiredDisguiseClass, int, "CTFPlayer", "m_nDesiredDisguiseClass")
	NETVAR(m_flStealthNoAttackExpire, float, "CTFPlayer", "m_flStealthNoAttackExpire")
	NETVAR(m_flStealthNextChangeTime, float, "CTFPlayer", "m_flStealthNextChangeTime")
	NETVAR(m_bLastDisguisedAsOwnTeam, bool, "CTFPlayer", "m_bLastDisguisedAsOwnTeam")
	NETVAR(m_flRageMeter, float, "CTFPlayer", "m_flRageMeter")
	NETVAR(m_bRageDraining, bool, "CTFPlayer", "m_bRageDraining")
	NETVAR(m_flNextRageEarnTime, float, "CTFPlayer", "m_flNextRageEarnTime")
	NETVAR(m_bInUpgradeZone, bool, "CTFPlayer", "m_bInUpgradeZone")
	NETVAR(m_flItemChargeMeter, void*, "CTFPlayer", "m_flItemChargeMeter")
	NETVAR(m_bPlayerDominated, void*, "CTFPlayer", "m_bPlayerDominated")
	NETVAR(m_bPlayerDominatingMe, void*, "CTFPlayer", "m_bPlayerDominatingMe")
	NETVAR(m_ScoreData, void*, "CTFPlayer", "m_ScoreData")
	NETVAR(m_iCaptures, int, "CTFPlayer", "m_iCaptures")
	NETVAR(m_iDefenses, int, "CTFPlayer", "m_iDefenses")
	NETVAR(m_iKills, int, "CTFPlayer", "m_iKills")
	NETVAR(m_iDeaths, int, "CTFPlayer", "m_iDeaths")
	NETVAR(m_iSuicides, int, "CTFPlayer", "m_iSuicides")
	NETVAR(m_iDominations, int, "CTFPlayer", "m_iDominations")
	NETVAR(m_iRevenge, int, "CTFPlayer", "m_iRevenge")
	NETVAR(m_iBuildingsBuilt, int, "CTFPlayer", "m_iBuildingsBuilt")
	NETVAR(m_iBuildingsDestroyed, int, "CTFPlayer", "m_iBuildingsDestroyed")
	NETVAR(m_iHeadshots, int, "CTFPlayer", "m_iHeadshots")
	NETVAR(m_iBackstabs, int, "CTFPlayer", "m_iBackstabs")
	NETVAR(m_iHealPoints, int, "CTFPlayer", "m_iHealPoints")
	NETVAR(m_iInvulns, int, "CTFPlayer", "m_iInvulns")
	NETVAR(m_iTeleports, int, "CTFPlayer", "m_iTeleports")
	NETVAR(m_iResupplyPoints, int, "CTFPlayer", "m_iResupplyPoints")
	NETVAR(m_iKillAssists, int, "CTFPlayer", "m_iKillAssists")
	NETVAR(m_iPoints, int, "CTFPlayer", "m_iPoints")
	NETVAR(m_iBonusPoints, int, "CTFPlayer", "m_iBonusPoints")
	NETVAR(m_iDamageDone, int, "CTFPlayer", "m_iDamageDone")
	NETVAR(m_iCrits, int, "CTFPlayer", "m_iCrits")
	NETVAR(m_RoundScoreData, void*, "CTFPlayer", "m_RoundScoreData")
	NETVAR(m_ConditionList, void*, "CTFPlayer", "m_ConditionList")
	NETVAR(_condition_bits, int, "CTFPlayer", "_condition_bits")
	NETVAR(m_iTauntIndex, int, "CTFPlayer", "m_iTauntIndex")
	NETVAR(m_iTauntConcept, int, "CTFPlayer", "m_iTauntConcept")
	NETVAR(m_nPlayerCondEx, int, "CTFPlayer", "m_nPlayerCondEx")
	NETVAR(m_iStunIndex, int, "CTFPlayer", "m_iStunIndex")
	NETVAR(m_nHalloweenBombHeadStage, int, "CTFPlayer", "m_nHalloweenBombHeadStage")
	NETVAR(m_nPlayerCondEx2, int, "CTFPlayer", "m_nPlayerCondEx2")
	NETVAR(m_nPlayerCondEx3, int, "CTFPlayer", "m_nPlayerCondEx3")
	NETVAR(m_nStreaks, void*, "CTFPlayer", "m_nStreaks")
	NETVAR(m_unTauntSourceItemID_Low, int, "CTFPlayer", "m_unTauntSourceItemID_Low")
	NETVAR(m_unTauntSourceItemID_High, int, "CTFPlayer", "m_unTauntSourceItemID_High")
	NETVAR(m_flRuneCharge, float, "CTFPlayer", "m_flRuneCharge")
	NETVAR(m_bHasPasstimeBall, bool, "CTFPlayer", "m_bHasPasstimeBall")
	NETVAR(m_bIsTargetedForPasstimePass, bool, "CTFPlayer", "m_bIsTargetedForPasstimePass")
	NETVAR(m_hPasstimePassTarget, int /*EHANDLE*/, "CTFPlayer", "m_hPasstimePassTarget")
	NETVAR(m_askForBallTime, float, "CTFPlayer", "m_askForBallTime")
	NETVAR(m_bKingRuneBuffActive, bool, "CTFPlayer", "m_bKingRuneBuffActive")
	NETVAR(m_nPlayerCondEx4, int, "CTFPlayer", "m_nPlayerCondEx4")
	NETVAR(m_flHolsterAnimTime, float, "CTFPlayer", "m_flHolsterAnimTime")
	NETVAR(m_hSwitchTo, int /*EHANDLE*/, "CTFPlayer", "m_hSwitchTo")
	NETVAR(m_hItem, int /*EHANDLE*/, "CTFPlayer", "m_hItem")
	NETVAR(m_bIsCoaching, bool, "CTFPlayer", "m_bIsCoaching")
	NETVAR(m_hCoach, int /*EHANDLE*/, "CTFPlayer", "m_hCoach")
	NETVAR(m_hStudent, int /*EHANDLE*/, "CTFPlayer", "m_hStudent")
	NETVAR(m_nCurrency, int, "CTFPlayer", "m_nCurrency")
	NETVAR(m_nExperienceLevel, int, "CTFPlayer", "m_nExperienceLevel")
	NETVAR(m_nExperienceLevelProgress, int, "CTFPlayer", "m_nExperienceLevelProgress")
	NETVAR(m_bMatchSafeToLeave, bool, "CTFPlayer", "m_bMatchSafeToLeave")
	NETVAR(m_vecTFOrigin, Vec3, "CTFPlayer", "m_vecOrigin")
	NETVAR(m_angEyeAnglesX, float, "CTFPlayer", "m_angEyeAngles[0]")
	NETVAR(m_angEyeAnglesY, float, "CTFPlayer", "m_angEyeAngles[1]")
	NETVAR(m_bAllowMoveDuringTaunt, bool, "CTFPlayer", "m_bAllowMoveDuringTaunt")
	NETVAR(m_bIsReadyToHighFive, bool, "CTFPlayer", "m_bIsReadyToHighFive")
	NETVAR(m_hHighFivePartner, int /*EHANDLE*/, "CTFPlayer", "m_hHighFivePartner")
	NETVAR(m_nForceTauntCam, int, "CTFPlayer", "m_nForceTauntCam")
	NETVAR(m_flTauntYaw, float, "CTFPlayer", "m_flTauntYaw")
	NETVAR_OFF(m_flPrevTauntYaw, float, "CTFPlayer", "m_flTauntYaw", 4)
	NETVAR(m_nActiveTauntSlot, int, "CTFPlayer", "m_nActiveTauntSlot")
	NETVAR(m_iTauntItemDefIndex, int, "CTFPlayer", "m_iTauntItemDefIndex")
	NETVAR(m_flCurrentTauntMoveSpeed, float, "CTFPlayer", "m_flCurrentTauntMoveSpeed")
	NETVAR(m_flVehicleReverseTime, float, "CTFPlayer", "m_flVehicleReverseTime")
	NETVAR(m_flMvMLastDamageTime, float, "CTFPlayer", "m_flMvMLastDamageTime")
	NETVAR(m_flLastDamageTime, float, "CTFPlayer", "m_flLastDamageTime")
	NETVAR(m_bInPowerPlay, bool, "CTFPlayer", "m_bInPowerPlay")
	NETVAR(m_iSpawnCounter, int, "CTFPlayer", "m_iSpawnCounter")
	NETVAR(m_bArenaSpectator, bool, "CTFPlayer", "m_bArenaSpectator")
	NETVAR(m_AttributeManager, void*, "CTFPlayer", "m_AttributeManager")
	NETVAR(m_hOuter, int /*EHANDLE*/, "CTFPlayer", "m_hOuter")
	NETVAR(m_ProviderType, int, "CTFPlayer", "m_ProviderType")
	NETVAR(m_iReapplyProvisionParity, int, "CTFPlayer", "m_iReapplyProvisionParity")
	NETVAR(m_flHeadScale, float, "CTFPlayer", "m_flHeadScale")
	NETVAR(m_flTorsoScale, float, "CTFPlayer", "m_flTorsoScale")
	NETVAR(m_flHandScale, float, "CTFPlayer", "m_flHandScale")
	NETVAR(m_bUseBossHealthBar, bool, "CTFPlayer", "m_bUseBossHealthBar")
	NETVAR(m_bUsingVRHeadset, bool, "CTFPlayer", "m_bUsingVRHeadset")
	NETVAR(m_bForcedSkin, bool, "CTFPlayer", "m_bForcedSkin")
	NETVAR(m_nForcedSkin, int, "CTFPlayer", "m_nForcedSkin")
	NETVAR(m_bTFGlowEnabled, bool, "CTFPlayer", "m_bGlowEnabled")
	NETVAR(m_nActiveWpnClip, int, "CTFPlayer", "m_nActiveWpnClip")
	NETVAR(m_flKartNextAvailableBoost, float, "CTFPlayer", "m_flKartNextAvailableBoost")
	NETVAR(m_iKartHealth, int, "CTFPlayer", "m_iKartHealth")
	NETVAR(m_iKartState, int, "CTFPlayer", "m_iKartState")
	NETVAR(m_hGrapplingHookTarget, int /*EHANDLE*/, "CTFPlayer", "m_hGrapplingHookTarget")
	NETVAR(m_hSecondaryLastWeapon, int /*EHANDLE*/, "CTFPlayer", "m_hSecondaryLastWeapon")
	NETVAR(m_bUsingActionSlot, bool, "CTFPlayer", "m_bUsingActionSlot")
	NETVAR(m_flInspectTime, float, "CTFPlayer", "m_flInspectTime")
	NETVAR(m_flHelpmeButtonPressTime, float, "CTFPlayer", "m_flHelpmeButtonPressTime")
	NETVAR(m_iCampaignMedals, int, "CTFPlayer", "m_iCampaignMedals")
	NETVAR(m_iPlayerSkinOverride, int, "CTFPlayer", "m_iPlayerSkinOverride")
	NETVAR(m_bViewingCYOAPDA, bool, "CTFPlayer", "m_bViewingCYOAPDA")
	CONDGET(OnGround, m_fFlags(), FL_ONGROUND)
	CONDGET(InWater, m_fFlags(), FL_INWATER)
	CONDGET(Ducking, m_fFlags(), FL_DUCKING)
	CONDGET(Charging, m_nPlayerCond(), TFCond_Charging)
	CONDGET(Scoped, m_nPlayerCond(), TFCond_Zoomed)
	CONDGET(UberedCond, m_nPlayerCond(), TFCond_Ubercharged)
	CONDGET(Bonked, m_nPlayerCond(), TFCond_Bonked)
	CONDGET(InMilk, m_nPlayerCond(), TFCond_Milked)
	CONDGET(InJarate, m_nPlayerCond(), TFCond_Jarated)
	CONDGET(Bleeding, m_nPlayerCond(), TFCond_Bleeding)
	CONDGET(Disguised, m_nPlayerCond(), TFCond_Disguised)
	CONDGET(Taunting, m_nPlayerCond(), TFCond_Taunting)
	CONDGET(OnFire, m_nPlayerCond(), TFCond_OnFire)
	CONDGET(Stunned, m_nPlayerCond(), TFCond_Stunned)
	CONDGET(Slowed, m_nPlayerCond(), TFCond_Slowed)
	CONDGET(MegaHealed, m_nPlayerCond(), TFCond_MegaHeal)
	CONDGET(AGhost, m_nPlayerCondEx2(), TFCondEx2_HalloweenGhostMode)
	CONDGET(InBumperKart, m_nPlayerCondEx2(), TFCondEx2_InKart)
	CONDGET(PhlogUbered, m_nPlayerCondEx(), TFCondEx_PyroCrits)
	CONDGET(BlastImmune, m_nPlayerCondEx2(), TFCondEx2_BlastImmune)
	CONDGET(BulletImmune, m_nPlayerCondEx2(), TFCondEx2_BulletImmune)
	CONDGET(FireImmune, m_nPlayerCondEx2(), TFCondEx2_FireImmune)
	CONDGET(StrengthRune, m_nPlayerCondEx2(), TFCondEx2_StrengthRune)
	CONDGET(HasteRune, m_nPlayerCondEx2(), TFCondEx2_HasteRune)
	CONDGET(RegenRune, m_nPlayerCondEx2(), TFCondEx2_RegenRune)
	CONDGET(ResistRune, m_nPlayerCondEx2(), TFCondEx2_ResistRune)
	CONDGET(VampireRune, m_nPlayerCondEx2(), TFCondEx2_VampireRune)
	CONDGET(ReflectRune, m_nPlayerCondEx2(), TFCondEx2_ReflectRune)
	CONDGET(PrecisionRune, m_nPlayerCondEx3(), TFCondEx3_PrecisionRune)
	CONDGET(AgilityRune, m_nPlayerCondEx3(), TFCondEx3_AgilityRune)
	CONDGET(KnockoutRune, m_nPlayerCondEx3(), TFCondEx3_KnockoutRune)
	CONDGET(ImbalanceRune, m_nPlayerCondEx3(), TFCondEx3_ImbalanceRune)
	CONDGET(CritTempRune, m_nPlayerCondEx3(), TFCondEx3_CritboostedTempRune)
	CONDGET(KingRune, m_nPlayerCondEx3(), TFCondEx3_KingRune)
	CONDGET(PlagueRune, m_nPlayerCondEx3(), TFCondEx3_PlagueRune)
	CONDGET(SupernovaRune, m_nPlayerCondEx3(), TFCondEx3_SupernovaRune)
	CONDGET(BuffedByKing, m_nPlayerCondEx3(), TFCondEx3_KingBuff)
	CONDGET(BlastResist, m_nPlayerCondEx(), TFCondEx_ExplosiveCharge)
	CONDGET(BulletResist, m_nPlayerCondEx(), TFCondEx_BulletCharge)
	CONDGET(FireResist, m_nPlayerCondEx(), TFCondEx_FireCharge)
	OFFSET(MoveType, MoveType_t, 0x1A4)
	VIRTUAL(MaxHealth, int, this, int(__thiscall*)(void*), 0x6b)
	__inline Vec3 GetShootPos()
	{
		return m_vecOrigin() + m_vecViewOffset();
	}
	__inline Vec3 GetEyePosition()
	{
		return GetAbsOrigin() + m_vecViewOffset();
	}
	__inline Vec3 GetEyeAngles()
	{
		return { m_angEyeAnglesX(), m_angEyeAnglesY(), 0.f };
	}
	__inline Vec3 GetWorldSpaceCenter()
	{
		return m_vecOrigin() + Vec3(0, 0, (m_vecMins().z + m_vecMaxs().z) / 2);
	}
	__inline Vec3 GetViewOffset() // in the case m_vecViewOffset isn't networked/handled properly
	{
		auto getMainOffset = [this]() -> Vec3
			{
				if (IsDucking())
					return { 0.f, 0.f, 45.f };

				switch (m_iClass())
				{
				case ETFClass::CLASS_SCOUT: return { 0.f, 0.f, 65.f };
				case ETFClass::CLASS_SOLDIER: return { 0.f, 0.f, 68.f };
				case ETFClass::CLASS_PYRO: return { 0.f, 0.f, 68.f };
				case ETFClass::CLASS_DEMOMAN: return { 0.f, 0.f, 68.f };
				case ETFClass::CLASS_HEAVY: return { 0.f, 0.f, 75.f };
				case ETFClass::CLASS_ENGINEER: return { 0.f, 0.f, 68.f };
				case ETFClass::CLASS_MEDIC: return { 0.f, 0.f, 75.f };
				case ETFClass::CLASS_SNIPER: return { 0.f, 0.f, 75.f };
				case ETFClass::CLASS_SPY: return { 0.f, 0.f, 75.f };
				}

				const Vec3 vOffset = m_vecViewOffset();
				if (vOffset.z)
					return vOffset;
				
				return { 0.f, 0.f, 68.f };
			};

		const float flSize = (m_vecMaxs().z - m_vecMins().z) / (IsDucking() ? 62 : 82);
		return getMainOffset() * flSize;
	}
	// credits: KGB
	__inline bool InCond(const ETFCond eCond)
	{
		const int iCond = static_cast<int>(eCond);

		switch (iCond / 32)
		{
		case 0:
		{
			const int bit = (1 << iCond);
			if ((m_nPlayerCond() & bit) == bit)
				return true;
			if ((_condition_bits() & bit) == bit)
				return true;
			break;
		}
		case 1:
		{
			const int bit = 1 << (iCond - 32);
			if ((m_nPlayerCondEx() & bit) == bit)
				return true;
			break;
		}
		case 2:
		{
			const int bit = 1 << (iCond - 64);
			if ((m_nPlayerCondEx2() & bit) == bit)
				return true;
			break;
		}
		case 3:
		{
			const int bit = 1 << (iCond - 96);
			if ((m_nPlayerCondEx3() & bit) == bit)
				return true;
			break;
		}
		case 4:
		{
			const int bit = 1 << (iCond - 128);
			if ((m_nPlayerCondEx4() & bit) == bit)
				return true;
			break;
		}
		}

		return false;
	}
	__inline bool IsInvisible()
	{
		if (InCond(TF_COND_BURNING)
			|| InCond(TF_COND_BURNING_PYRO)
			|| InCond(TF_COND_MAD_MILK)
			|| InCond(TF_COND_URINE))
			return false;

		return m_flInvisibility() >= 1.f;
	}
	__inline bool IsZoomed()
	{
		return InCond(TF_COND_ZOOMED);
	}
	__inline bool IsInvulnerable()
	{
		return InCond(TF_COND_INVULNERABLE)
			|| InCond(TF_COND_INVULNERABLE_CARD_EFFECT)
			|| InCond(TF_COND_INVULNERABLE_HIDE_UNLESS_DAMAGED)
			|| InCond(TF_COND_INVULNERABLE_USER_BUFF)
			|| InCond(TF_COND_PHASE);
	}
	__inline bool IsUbered()
	{
		return InCond(TF_COND_INVULNERABLE)
			|| InCond(TF_COND_INVULNERABLE_CARD_EFFECT)
			|| InCond(TF_COND_INVULNERABLE_HIDE_UNLESS_DAMAGED)
			|| InCond(TF_COND_INVULNERABLE_USER_BUFF);
	}
	__inline bool IsCritBoosted()
	{
		if (InCond(TF_COND_CRITBOOSTED)
			|| InCond(TF_COND_CRITBOOSTED_PUMPKIN)
			|| InCond(TF_COND_CRITBOOSTED_USER_BUFF)
			// || InCond(TF_COND_CRITBOOSTED_DEMO_CHARGE) // only client
			|| InCond(TF_COND_CRITBOOSTED_FIRST_BLOOD)
			|| InCond(TF_COND_CRITBOOSTED_BONUS_TIME)
			|| InCond(TF_COND_CRITBOOSTED_CTF_CAPTURE)
			|| InCond(TF_COND_CRITBOOSTED_ON_KILL)
			|| InCond(TF_COND_CRITBOOSTED_CARD_EFFECT)
			|| InCond(TF_COND_CRITBOOSTED_RUNE_TEMP))
			return true;

		/*
		CBaseCombatWeapon* pWeapon = GetActiveWeapon();
		if (pWeapon)
		{
			auto tfWeaponInfo = pWeapon->GetTFWeaponInfo();
			if (InCond(TF_COND_CRITBOOSTED_RAGE_BUFF) && tfWeaponInfo && tfWeaponInfo->m_iWeaponType == 0)
				// Only primary weapon can be crit boosted by pyro rage
				return true;
			
			float flCritHealthPercent = Utils::AttribHookValue(1.f, "mult_crit_when_health_is_below_percent", pWeapon);
			float flHealthFraction = GetMaxHealth() ? (float)m_iHealth() / GetMaxHealth() : 1.f;
			if (flCritHealthPercent < 1.f && flHealthFraction < flCritHealthPercent)
				return true;
		}
		*/

		return false;
	}
	__inline bool IsMiniCritBoosted()
	{
		return InCond(TF_COND_MINICRITBOOSTED_ON_KILL)
			|| InCond(TF_COND_NOHEALINGDAMAGEBUFF)
			|| InCond(TF_COND_ENERGY_BUFF)
			|| InCond(TF_COND_CRITBOOSTED_DEMO_CHARGE);
	}
	__inline bool IsMarked()
	{
		return InCond(TF_COND_URINE)
			|| InCond(TF_COND_MARKEDFORDEATH)
			|| InCond(TF_COND_MARKEDFORDEATH_SILENT);
	}
	__inline bool IsCloaked() // i dont put flickers in here because they are slightly visible, so it doesnt really count as cloaked to me
	{
		return InCond(TF_COND_STEALTHED); // || InCond(TF_COND_STEALTHED_USER_BUFF);
	}
	__inline const char* GetRune()
	{
		if (IsStrengthRune()) return "Strength Rune";
		if (IsHasteRune()) return "Haste Rune";
		if (IsRegenRune()) return "Regen Rune";
		if (IsResistRune()) return "Resist Rune";
		if (IsVampireRune()) return "Vampire Rune";
		if (IsReflectRune()) return "Reflect Rune";
		if (IsPrecisionRune()) return "Precision Rune";
		if (IsAgilityRune()) return "Agility Rune";
		if (IsKnockoutRune()) return "Knockout Rune";
		if (IsImbalanceRune()) return "Imbalance Rune";
		if (IsKingRune()) return "King";
		if (IsPlagueRune()) return "Plague Rune";
		if (IsSupernovaRune()) return "Supernova Rune";
		return nullptr;
	}
	__inline bool IsClass(const int nClass)
	{
		return m_iClass() == nClass;
	}
	__inline float GetPlayerMaxVelocity()
	{
		switch (m_iClass())
		{
		case ETFClass::CLASS_SCOUT: return 400.f;
		case ETFClass::CLASS_SOLDIER: return 240.f;
		case ETFClass::CLASS_PYRO: return 300.f;
		case ETFClass::CLASS_DEMOMAN: return 280.f;
		case ETFClass::CLASS_HEAVY: return 230.f; // 110 when spinning minigun
		case ETFClass::CLASS_ENGINEER: return 300.f;
		case ETFClass::CLASS_MEDIC: return 320.f;
		case ETFClass::CLASS_SNIPER: return 300.f;
		case ETFClass::CLASS_SPY: return 320.f;
		default: return 1.f;
		}
	}
	__inline bool OnSolid()
	{
		return m_hGroundEntity() >= 0 || IsOnGround();
	}
	__inline bool IsSwimming()
	{
		return m_nTFWaterLevel() > 1;
	}
	__inline void SetEyeAngles(const Vec3& vAngles)
	{
		m_angEyeAnglesX() = vAngles.x;
		m_angEyeAnglesY() = vAngles.y;
	}
	__inline float GetInvisPercentage()
	{
		static auto tf_spy_invis_time = I::Cvar->FindVar("tf_spy_invis_time");
		const float flInvisTime = tf_spy_invis_time ? tf_spy_invis_time->GetFloat() : 1.f;
		const float GetInvisPercent = Math::RemapValClamped(m_flInvisChangeCompleteTime() - I::GlobalVars->curtime, flInvisTime, 0.f, 0.f, 100.f);

		return GetInvisPercent;
	}
	__inline size_t* GetMyWeapons()
	{
		static int nOffset = g_NetVars.GetNetVar("CBaseCombatCharacter", "m_hMyWeapons");
		return reinterpret_cast<size_t*>(reinterpret_cast<DWORD>(this) + nOffset);
	}
	__inline CBaseCombatWeapon* GetWeaponFromSlot(const int nSlot)
	{
		static int nOffset = g_NetVars.GetNetVar("CBaseCombatCharacter", "m_hMyWeapons");
		int hWeapon = *reinterpret_cast<int*>(reinterpret_cast<DWORD>(this) + (nOffset + (nSlot * 0x4)));
		return reinterpret_cast<CBaseCombatWeapon*>(I::ClientEntityList->GetClientEntityFromHandle(hWeapon));
	}
	__inline float TeamFortress_CalculateMaxSpeed(bool bIgnoreSpecialAbility = false)
	{
		return S::TeamFortress_CalculateMaxSpeed.As<float(__thiscall*)(CBaseEntity*, bool)>()(this, bIgnoreSpecialAbility);
	}
	__inline float& m_flLastMovementStunChange()
	{
		return *reinterpret_cast<float*>(reinterpret_cast<DWORD>(this) + 0x1BA0);
	}
	__inline float& m_flStunLerpTarget()
	{
		return *reinterpret_cast<float*>(reinterpret_cast<DWORD>(this) + 0x1B9C);
	}
	__inline bool& m_bStunNeedsFadeOut()
	{
		return *reinterpret_cast<bool*>(reinterpret_cast<DWORD>(this) + 0x1B98);
	}
	__inline CTFPlayerAnimState* GetAnimState()
	{
		return *reinterpret_cast<CTFPlayerAnimState**>(this + 0x1D88);
	}
	__inline void UpdateClientSideAnimation()
	{
		S::CBaseAnimating_UpdateClientSideAnimation.As<void(__thiscall*)(void*)>()(this);
	}
	__inline float GetCritMult()
	{
		return Math::RemapValClamped(static_cast<float>(m_iCritMult()), 0.f, 255.f, 1.f, 4.f);
	}
	__inline void UpdateWearables()
	{
		S::CTFPlayer_UpdateWearables.As<void(__thiscall*)(void*)>()(this);
	}
	__inline void ThirdPersonSwitch()
	{
		static auto address = S::CTFPlayer_ThirdPersonSwitch();
		static auto absolute = *reinterpret_cast<std::uintptr_t*>(address + 1) + address + 5;
		reinterpret_cast<void(__thiscall*)(CBaseEntity*)>(absolute)(this);
	}
	__inline float* GetHeadScale()
	{
		static int nOffset = g_NetVars.GetNetVar("CTFPlayer", "m_flHeadScale");
		return reinterpret_cast<float*>(reinterpret_cast<DWORD>(this) + nOffset);
	}
	__inline float* GetTorsoScale()
	{
		static int nOffset = g_NetVars.GetNetVar("CTFPlayer", "m_flTorsoScale");
		return reinterpret_cast<float*>(reinterpret_cast<DWORD>(this) + nOffset);
	}
	__inline float* GetHandScale()
	{
		static int nOffset = g_NetVars.GetNetVar("CTFPlayer", "m_flHandScale");
		return reinterpret_cast<float*>(reinterpret_cast<DWORD>(this) + nOffset);
	}

public:
	NETVAR(m_nSequence, int, "CBaseAnimating", "m_nSequence")
	NETVAR(m_nForceBone, int, "CBaseAnimating", "m_nForceBone")
	NETVAR(m_vecForce, Vec3, "CBaseAnimating", "m_vecForce")
	NETVAR(m_nSkin, int, "CBaseAnimating", "m_nSkin")
	NETVAR(m_nBody, int, "CBaseAnimating", "m_nBody")
	NETVAR(m_nHitboxSet, int, "CBaseAnimating", "m_nHitboxSet")
	NETVAR(m_flModelScale, float, "CBaseAnimating", "m_flModelScale")
	NETVAR(m_flModelWidthScale, float, "CBaseAnimating", "m_flModelWidthScale")
	NETVAR(m_flPlaybackRate, float, "CBaseAnimating", "m_flPlaybackRate")
	NETVAR(m_flEncodedController, void*, "CBaseAnimating", "m_flEncodedController")
	NETVAR(m_bClientSideAnimation, bool, "CBaseAnimating", "m_bClientSideAnimation")
	NETVAR(m_bClientSideFrameReset, bool, "CBaseAnimating", "m_bClientSideFrameReset")
	NETVAR(m_nNewSequenceParity, int, "CBaseAnimating", "m_nNewSequenceParity")
	NETVAR(m_nResetEventsParity, int, "CBaseAnimating", "m_nResetEventsParity")
	NETVAR(m_nMuzzleFlashParity, int, "CBaseAnimating", "m_nMuzzleFlashParity")
	NETVAR(m_hLightingOrigin, int /*EHANDLE*/, "CBaseAnimating", "m_hLightingOrigin")
	NETVAR(m_hLightingOriginRelative, int /*EHANDLE*/, "CBaseAnimating", "m_hLightingOriginRelative")
	NETVAR(m_flCycle, float, "CBaseAnimating", "m_flCycle")
	NETVAR(m_fadeMinDist, float, "CBaseAnimating", "m_fadeMinDist")
	NETVAR(m_fadeMaxDist, float, "CBaseAnimating", "m_fadeMaxDist")
	NETVAR(m_flFadeScale, float, "CBaseAnimating", "m_flFadeScale")
	__inline int GetHitboxGroup(int nHitbox)
	{
		const auto& pModel = GetModel();
		if (!pModel) return -1;
		const auto& pHDR = I::ModelInfoClient->GetStudioModel(pModel);
		if (!pHDR) return -1;
		const auto& pSet = pHDR->GetHitboxSet(m_nHitboxSet());
		if (!pSet) return -1;

		const auto& pBox = pSet->hitbox(nHitbox);
		if (!pBox) return -1;

		return pBox->group;
	}
	__inline int GetNumOfHitboxes()
	{
		const auto& pModel = GetModel();
		if (!pModel) return 0;
		const auto& pHDR = I::ModelInfoClient->GetStudioModel(pModel);
		if (!pHDR) return 0;
		const auto& pSet = pHDR->GetHitboxSet(m_nHitboxSet());
		if (!pSet) return 0;

		return pSet->numhitboxes;
	}
	__inline Vec3 GetHitboxPos(const int nHitbox, const Vec3 vOffset = {})
	{
		const auto& pModel = GetModel();
		if (!pModel) return Vec3();
		const auto& pHDR = I::ModelInfoClient->GetStudioModel(pModel);
		if (!pHDR) return Vec3();
		const auto& pSet = pHDR->GetHitboxSet(m_nHitboxSet());
		if (!pSet) return Vec3();

		matrix3x4 BoneMatrix[128];
		if (!SetupBones(BoneMatrix, 128, BONE_USED_BY_ANYTHING, m_flSimulationTime()))
			return Vec3();

		const auto& pBox = pSet->hitbox(nHitbox);
		if (!pBox) return Vec3();

		Vec3 vOut;
		Math::VectorTransform(vOffset, BoneMatrix[pBox->bone], vOut);
		return vOut;
	}
	__inline Vec3 GetHitboxPosMatrix(const int nHitbox, matrix3x4 BoneMatrix[128])
	{
		const auto& pModel = GetModel();
		if (!pModel) return Vec3();
		const auto& pHdr = I::ModelInfoClient->GetStudioModel(pModel);
		if (!pHdr) return Vec3();
		const auto& pSet = pHdr->GetHitboxSet(m_nHitboxSet());
		if (!pSet) return Vec3();

		const auto& pBox = pSet->hitbox(nHitbox);
		if (!pBox) return Vec3();

		Vec3 vPos = (pBox->bbmin + pBox->bbmax) * 0.5f, vOut;
		Math::VectorTransform(vPos, BoneMatrix[pBox->bone], vOut);
		return vOut;
	}
	__inline void GetHitboxInfo(int nHitbox, Vec3* pCenter, Vec3* pMins, Vec3* pMaxs, matrix3x4* pMatrix)
	{
		const auto& pModel = GetModel();
		if (!pModel) return;
		const auto& pHDR = I::ModelInfoClient->GetStudioModel(pModel);
		if (!pHDR) return;
		const auto& pSet = pHDR->GetHitboxSet(m_nHitboxSet());
		if (!pSet) return;

		const auto& pBox = pSet->hitbox(nHitbox);
		if (!pBox) return;

		matrix3x4 BoneMatrix[128] = {};
		if (!SetupBones(BoneMatrix, 128, BONE_USED_BY_ANYTHING, I::GlobalVars->curtime))
			return;

		if (pMins)
			*pMins = pBox->bbmin;
		if (pMaxs)
			*pMaxs = pBox->bbmax;
		if (pCenter)
			Math::VectorTransform((pBox->bbmin + pBox->bbmax) * 0.5f, BoneMatrix[pBox->bone], *pCenter);
		if (pMatrix)
			memcpy(*pMatrix, BoneMatrix[pBox->bone], sizeof(matrix3x4));
	}
	__inline bool GetHitboxMinsAndMaxs(const int nHitbox, Vec3& vMins, Vec3& vMaxs, Vec3* vCenter)
	{
		const auto& pModel = GetModel();
		if (!pModel) return false;
		const auto& pHdr = I::ModelInfoClient->GetStudioModel(pModel);
		if (!pHdr) return false;
		const auto& pSet = pHdr->GetHitboxSet(m_nHitboxSet());
		if (!pSet) return false;

		matrix3x4 BoneMatrix[128];
		if (!SetupBones(BoneMatrix, 128, 0x100, I::GlobalVars->curtime))
			return false;

		const auto& pBox = pSet->hitbox(nHitbox);
		if (!pBox) return false;

		vMins = pBox->bbmin; vMaxs = pBox->bbmax;
		if (vCenter) Math::VectorTransform(((pBox->bbmin + pBox->bbmax) * 0.5f), BoneMatrix[pBox->bone], *vCenter);
		return true;
	}
	__inline bool GetHitboxMinsAndMaxsAndMatrix(const int nHitbox, Vec3& vMins, Vec3& vMaxs, matrix3x4& matrix, Vec3* vCenter)
	{
		const auto& pModel = GetModel();
		if (!pModel) return false;
		const auto& pHdr = I::ModelInfoClient->GetStudioModel(pModel);
		if (!pHdr) return false;
		const auto& pSet = pHdr->GetHitboxSet(m_nHitboxSet());
		if (!pSet) return false;

		matrix3x4 BoneMatrix[128];
		if (!SetupBones(BoneMatrix, 128, 0x100, I::GlobalVars->curtime))
			return false;

		const auto& pBox = pSet->hitbox(nHitbox);
		if (!pBox) return false;

		vMins = pBox->bbmin; vMaxs = pBox->bbmax;
		memcpy(matrix, BoneMatrix[pBox->bone], sizeof(matrix3x4));
		if (vCenter) Math::VectorTransform(((pBox->bbmin + pBox->bbmax) * 0.5f), BoneMatrix[pBox->bone], *vCenter);
		return true;
	}
	__inline bool GetHitboxMinsAndMaxsFromMatrix(const int nHitbox, Vec3& vMins, Vec3& vMaxs, matrix3x4* matrix, Vec3* vCenter)
	{
		const auto& pModel = GetModel();
		if (!pModel) return false;
		const auto& pHdr = I::ModelInfoClient->GetStudioModel(pModel);
		if (!pHdr) return false;
		const auto& pSet = pHdr->GetHitboxSet(m_nHitboxSet());
		if (!pSet) return false;

		const auto& pBox = pSet->hitbox(nHitbox);
		if (!pBox) return false;

		vMins = pBox->bbmin; vMaxs = pBox->bbmax;
		if (vCenter) Math::VectorTransform(((pBox->bbmin + pBox->bbmax) * 0.5f), matrix[pBox->bone], *vCenter);
		return true;
	}
	__inline std::array<float, MAXSTUDIOPOSEPARAM>& m_flPoseParameter()
	{
		static int nOffset = g_NetVars.GetNetVar("CBaseAnimating", "m_flPoseParameter");
		return *reinterpret_cast<std::array<float, MAXSTUDIOPOSEPARAM>*>(reinterpret_cast<DWORD>(this) + nOffset);
	}
	__inline CUtlVector<matrix3x4>* GetCachedBoneData()
	{
		return reinterpret_cast<CUtlVector<matrix3x4> *>(reinterpret_cast<DWORD>(this) + 0x848); //0x844?
	}
	__inline CStudioHdr* GetModelPtr()
	{
		static int nOffset = g_NetVars.GetNetVar("CBaseAnimating", "m_nMuzzleFlashParity") + 12;
		return *reinterpret_cast<CStudioHdr**>(reinterpret_cast<DWORD>(this) + nOffset);
	}
	__inline float FrameAdvance(float flInterval)
	{
		return S::CBaseAnimating_FrameAdvance.As<float(__thiscall*)(void*, float)>()(this, flInterval);
	}
	__inline Vec3 GetBonePos(const int nBone)
	{
		matrix3x4 BoneMatrix[128];
		if (SetupBones(BoneMatrix, 128, 0x100, I::GlobalVars->curtime))
			return Vec3(BoneMatrix[nBone][0][3], BoneMatrix[nBone][1][3], BoneMatrix[nBone][2][3]);

		return {};
	}
	__inline void GetBonePosition(int iBone, Vector& origin, QAngle& angles)
	{
		S::CBaseAnimating_GetBonePosition.As<void(__thiscall*)(void*, int, Vector&, QAngle&)>()(this, iBone, origin, angles);
	}
	__inline bool GetAttachment(int number, Vec3& origin)
	{
		return GetVFunc<bool(__thiscall*)(void*, int, Vec3&)>(this, 71)(this, number, origin);
	}
	__inline void* Renderable()
	{
		return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(this) + 0x4);
	}
	VIRTUAL(RenderAngles, const Vec3&, Renderable(), const Vec3& (__thiscall*)(void*), 2)
	VIRTUAL(Model, model_t*, Renderable(), model_t* (__thiscall*)(void*), 9)
	VIRTUAL(RgflCoordinateFrame, matrix3x4&, Renderable(), matrix3x4& (__thiscall*)(void*), 34)
	__inline bool ShouldDraw()
	{
		const auto pRend = Renderable();
		return GetVFunc<bool(__thiscall*)(void*)>(pRend, 3)(pRend);
	}
	__inline int DrawModel(int nFlags)
	{
		const auto pRend = Renderable();
		return GetVFunc<int(__thiscall*)(void*, int)>(pRend, 10)(pRend, nFlags);
	}
	__inline bool SetupBones(matrix3x4* pOut, int nMax, int nMask, float flTime)
	{
		const auto pRend = Renderable();
		return GetVFunc<bool(__thiscall*)(void*, matrix3x4*, int, int, float)>(pRend, 16)(pRend, pOut, nMax, nMask, flTime);
	}
	__inline void GetRenderBounds(Vec3& vMins, Vec3& vMaxs)
	{
		const auto pRend = Renderable();
		GetVFunc<void(__thiscall*)(void*, Vec3&, Vec3&)>(pRend, 20)(pRend, vMins, vMaxs);
	}
	__inline void SetRenderAngles(const Vec3& vAngles)
	{
		Vec3* pRenderAngles = const_cast<Vec3*>(&GetRenderAngles());
		*pRenderAngles = vAngles;
	}
	__inline int LookupAttachment(const char* pAttachmentName)
	{
		const auto pRend = Renderable();
		return GetVFunc<int(__thiscall*)(void*, const char*)>(pRend, 35)(pRend, pAttachmentName);
	}

public:
	NETVAR(m_bBuilding, bool, "CBaseObject", "m_bBuilding")
	NETVAR(m_bMiniBuilding, bool, "CBaseObject", "m_bMiniBuilding")
	NETVAR(m_bWasMapPlaced, bool, "CBaseObject", "m_bWasMapPlaced")
	NETVAR(m_bHasSapper, bool, "CBaseObject", "m_bHasSapper")
	NETVAR(m_bDisabled, bool, "CBaseObject", "m_bDisabled")
	NETVAR(m_bCarried, bool, "CBaseObject", "m_bCarried")
	NETVAR(m_bPlasmaDisable, bool, "CBaseObject", "m_bPlasmaDisable")
	NETVAR(m_hBuilder, int /*EHANDLE*/, "CBaseObject", "m_hBuilder")
	NETVAR(m_iBOHealth, int, "CBaseObject", "m_iHealth")
	NETVAR(m_iMaxHealth, int, "CBaseObject", "m_iMaxHealth")
	NETVAR(m_iUpgradeLevel, int, "CBaseObject", "m_iUpgradeLevel")
	NETVAR(m_iHighestUpgradeLevel, int, "CBaseObject", "m_iHighestUpgradeLevel")
	NETVAR(m_iObjectMode, int, "CBaseObject", "m_iObjectMode")
	NETVAR(m_iObjectType, int, "CBaseObject", "m_iObjectType")
	NETVAR(m_flPercentageConstructed, float, "CBaseObject", "m_flPercentageConstructed")
	NETVAR(m_iAmmoShells, int, "CObjectSentrygun", "m_iAmmoShells")
	NETVAR(m_iAmmoRockets, int, "CObjectSentrygun", "m_iAmmoRockets")
	NETVAR(m_bPlayerControlled, bool, "CObjectSentrygun", "m_bPlayerControlled")
	NETVAR(m_iAmmoMetal, int, "CObjectDispenser", "m_iAmmoMetal")
	OFFSET(RechargeTime, float, 0xFC0)
	OFFSET(CurrentChargeDuration, float, 0xFC4)
	OFFSET(YawToExit, float, 0xFCC)
	__inline int MaxAmmoShells() // Pasted from https://github.com/Lak3/tf2-internal-base
	{
		if (m_iUpgradeLevel() == 1 || m_bMiniBuilding())
			return 150;
		else
			return 200;
	}
	__inline void GetAmmoCount(int& iShells, int& iMaxShells, int& iRockets, int& iMaxRockets)
	{
		const bool bIsMini = m_bMiniBuilding();

		iShells = m_iAmmoShells();
		iMaxShells = MaxAmmoShells();
		iRockets = bIsMini ? 0 : m_iAmmoRockets();
		iMaxRockets = (bIsMini || m_iUpgradeLevel() < 3) ? 0 : 20;
	}
	__inline void* IHasBuildPoints()
	{
		return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(this) + 0xDC8);
	}
	VIRTUAL(NumBuildPoints, int, IHasBuildPoints(), int(__thiscall*)(void*), 0)
	__inline bool GetBuildPoint(int iPoint, Vec3& vecOrigin, Vec3& vecAngles)
	{
		void* pPoints = IHasBuildPoints();
		return GetVFunc<bool(__thiscall*)(void*, int, Vec3&, Vec3&)>(pPoints, 1)(pPoints, iPoint, vecOrigin, vecAngles);
	}
	__inline int GetBuildPointAttachmentIndex(int iPoint)
	{
		void* pPoints = IHasBuildPoints();
		return GetVFunc<int(__thiscall*)(void*, int)>(pPoints, 2)(pPoints, iPoint);
	}

public:
	NETVAR(m_hPlayer, int/*EHANDLE*/, "CTFRagdoll", "m_hPlayer")

public:
	NETVAR(m_hOriginalLauncher, int/*EHANDLE*/, "CBaseProjectile", "m_hOriginalLauncher")

public:
	NETVAR(m_flDamage, float, "CBaseGrenade", "m_flDamage")
	NETVAR(m_DmgRadius, float, "CBaseGrenade", "m_DmgRadius")
	NETVAR(m_bIsLive, bool, "CBaseGrenade", "m_bIsLive")
	NETVAR(m_hThrower, int/*EHANDLE*/, "CBaseGrenade", "m_hThrower")
	NETVAR(m_vecBGVelocity, Vec3, "CBaseGrenade", "m_vecVelocity")
	NETVAR(m_fBGFlags, int, "CBaseGrenade", "m_fFlags")

public:
	NETVAR(m_vBRInitialVelocity, Vec3, "CTFBaseRocket", "m_vInitialVelocity")
	NETVAR(m_vecBROrigin, Vec3, "CTFBaseRocket", "m_vecOrigin")
	NETVAR(m_angBRRotation, Vec3, "CTFBaseRocket", "m_angRotation")
	NETVAR(m_iBRDeflected, int, "CTFBaseRocket", "m_iDeflected")
	NETVAR(m_hBRLauncher, int/*EHANDLE*/, "CTFBaseRocket", "m_hLauncher")

public:
	NETVAR(m_bFCritical, bool, "CTFProjectile_Flare", "m_bCritical")

public:
	NETVAR(m_bRCritical, bool, "CTFProjectile_Rocket", "m_bCritical")

public:
	NETVAR(m_vBGInitialVelocity, Vec3, "CTFWeaponBaseGrenadeProj", "m_vInitialVelocity")
	NETVAR(m_bBGCritical, bool, "CTFWeaponBaseGrenadeProj", "m_bCritical")
	NETVAR(m_iBGDeflected, int, "CTFWeaponBaseGrenadeProj", "m_iDeflected")
	NETVAR(m_vecBGOrigin, Vec3, "CTFWeaponBaseGrenadeProj", "m_vecOrigin")
	NETVAR(m_angBGRotation, Vec3, "CTFWeaponBaseGrenadeProj", "m_angRotation")
	NETVAR(m_hDeflectOwner, int/*EHANDLE*/, "CTFWeaponBaseGrenadeProj", "m_hDeflectOwner")

public:
	NETVAR(m_bTouched, bool, "CTFGrenadePipebombProjectile", "m_bTouched")
	NETVAR(m_iType, int, "CTFGrenadePipebombProjectile", "m_iType")
	NETVAR_OFF(m_flCreationTime, float, "CTFGrenadePipebombProjectile", "m_iType", 4)
	NETVAR(m_hPBLauncher, int/*EHANDLE*/, "CTFGrenadePipebombProjectile", "m_hLauncher")
	NETVAR(m_bDefensiveBomb, int, "CTFGrenadePipebombProjectile", "m_bDefensiveBomb")
	OFFSET(m_bPulsed, bool, 0x908)
	bool HasStickyEffects()
	{
		return m_iType() == TF_GL_MODE_REMOTE_DETONATE || m_iType() == TF_GL_MODE_REMOTE_DETONATE_PRACTICE;
	}

public:
	OFFSET(m_flTankPressure, float, 0x1B40)

public:
	NETVAR(m_hTargetPlayer, int, "CHalloweenGiftPickup", "m_hTargetPlayer")
};