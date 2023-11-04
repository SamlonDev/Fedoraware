#pragma once

#include "../BaseEntity/BaseEntity.h"

#ifndef TICKS_TO_TIME
#define TICKS_TO_TIME( t )	( I::GlobalVars->interval_per_tick * ( t ) )
#endif

namespace S
{
	MAKE_SIGNATURE(GetLocalizedBaseItemName, CLIENT_DLL, "55 8B EC 56 8B 75 ? 8B 4E ? 85 C9 74 ? 57", 0x0);
	MAKE_SIGNATURE(GLocalizationProvider, CLIENT_DLL, "A1 ? ? ? ? A8 ? 75 ? 83 C8 ? C7 05 ? ? ? ? ? ? ? ? A3 ? ? ? ? B8 ? ? ? ? C3", 0x0);
	MAKE_SIGNATURE(C_EconItemView_GetStaticData, CLIENT_DLL, "0F B7 41 ? 50 E8 ? ? ? ? 8B C8 E8 ? ? ? ? 6A ? 68 ? ? ? ? 68 ? ? ? ? 6A ? 50 E8 ? ? ? ? 83 C4 ? C3 CC CC CC CC CC CC CC 83 C1", 0x0);
	MAKE_SIGNATURE(C_BaseCombatWeapon_GetName, CLIENT_DLL, "0F B7 81 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? 83 C0 ? C3 CC CC CC CC CC CC CC CC CC CC CC CC D9 05", 0x0);

	MAKE_SIGNATURE(CBaseCombatWeapon_GetWeaponData, CLIENT_DLL, "55 8B EC 66 8B 45 ? 66 3B 05 ? ? ? ? 73", 0x0);
	MAKE_SIGNATURE(CBaseCombatWeapon_CanFireCriticalShot, CLIENT_DLL, "6A ? 68 ? ? ? ? 68 ? ? ? ? 6A ? E8 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? C3 CC CC CC 55", 0x0);
	MAKE_SIGNATURE(CBaseCombatWeapon_GetTFWeaponInfo, CLIENT_DLL, "55 8B EC FF 75 ? E8 ? ? ? ? 83 C4 ? 85 C0 75 ? 5D C3", 0x0);
	MAKE_SIGNATURE(CBaseCombatWeapon_GetWeaponSpread, CLIENT_DLL, "55 8B EC 83 EC ? 56 8B F1 57 6A ? 6A", 0x0);

	MAKE_SIGNATURE(CTFWeaponBaseMelee_DoSwingTraceInternal, CLIENT_DLL, "53 8B DC 83 EC ? 83 E4 ? 83 C4 ? 55 8B 6B ? 89 6C 24 ? 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 56 8B F1", 0x0);

	MAKE_SIGNATURE(CBaseCombatWeapon_GetSpreadAngles, CLIENT_DLL, "55 8B EC 83 EC ? 56 57 6A ? 68 ? ? ? ? 68 ? ? ? ? 6A ? 8B F9 E8 ? ? ? ? 50 E8 ? ? ? ? 8B F0 83 C4 ? 85 F6", 0x0);
	MAKE_SIGNATURE(CBaseCombatWeapon_GetProjectileFireSetup, CLIENT_DLL, "53 8B DC 83 EC ? 83 E4 ? 83 C4 ? 55 8B 6B ? 89 6C 24 ? 8B EC 81 EC ? ? ? ? 56 8B F1 57 8B 06 8B 80 ? ? ? ? FF D0 84 C0", 0x0);

	MAKE_SIGNATURE(CBaseCombatWeapon_CalcIsAttackCritical, CLIENT_DLL, "53 57 6A ? 68 ? ? ? ? 68 ? ? ? ? 6A ? 8B F9 E8 ? ? ? ? 50 E8 ? ? ? ? 8B D8 83 C4 ? 85 DB 0F 84", 0x0);
	MAKE_SIGNATURE(CBaseCombatWeapon_CalcIsAttackCriticalHelper, CLIENT_DLL, "55 8B EC 83 EC ? 56 57 6A ? 68 ? ? ? ? 68 ? ? ? ? 6A ? 8B F9 E8 ? ? ? ? 50 E8 ? ? ? ? 8B F0 83 C4 ? 89 75", 0x0);
	MAKE_SIGNATURE(CBaseCombatWeapon_CalcIsAttackCriticalHelperMelee, CLIENT_DLL, "55 8B EC A1 ? ? ? ? 83 EC ? 83 78 ? ? 57 8B F9 75", 0x0);
}

class CBaseCombatWeapon : public CBaseEntity
{
public: //Netvars
	M_DYNVARGET(Clip1, int, this, "DT_BaseCombatWeapon", "LocalWeaponData", "m_iClip1")
	M_DYNVARGET(Clip2, int, this, "DT_BaseCombatWeapon", "LocalWeaponData", "m_iClip2")
	M_DYNVARGET(Energy, float, this, "DT_TFWeaponBase", "m_flEnergy")
	M_DYNVARGET(nViewModelIndex, int, this, "DT_BaseCombatWeapon", "LocalWeaponData", "m_nViewModelIndex")
	M_DYNVARGET(iViewModelIndex, int, this, "DT_BaseCombatWeapon", "m_iViewModelIndex")
	M_DYNVARGET(ItemDefIndex, int, this, "DT_EconEntity", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex")
	M_DYNVARGET(ChargeBeginTime, float, this, "DT_WeaponPipebombLauncher", "PipebombLauncherLocalData", "m_flChargeBeginTime")
	M_DYNVARGET(ChargeDamage, float, this, "DT_TFSniperRifle", "SniperRifleLocalData", "m_flChargedDamage")
	M_DYNVARGET(LastFireTime, float, this, "DT_TFWeaponBase", "LocalActiveTFWeaponData", "m_flLastFireTime")
	M_DYNVARGET(NextPrimaryAttack, float, this, "DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack")
	M_DYNVARGET(NextSecondaryAttack, float, this, "DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextSecondaryAttack")
	M_DYNVARGET(ChargeResistType, int, this, "DT_WeaponMedigun", "m_nChargeResistType")
	M_DYNVARGET(ReloadMode, int, this, "DT_TFWeaponBase", "m_iReloadMode")
	M_DYNVARGET(DetonateTime, float, this, "DT_WeaponGrenadeLauncher", "m_flDetonateTime")
	//M_DYNVARGET(ObservedCritChance, float, this, "DT_LocalTFWeaponData", "m_flObservedCritChance")
	M_DYNVARGET(LastCritCheckTime, float, this, "DT_TFWeaponBase", "LocalActiveTFWeaponData", "m_flLastCritCheckTime")
	M_DYNVARGET(ObservedCritChance, float, this, "DT_TFWeaponBase", "LocalActiveTFWeaponData", "m_flObservedCritChance")
	M_DYNVARGET(RechargeScale, float, this, "DT_WeaponFlameBall", "m_flRechargeScale")
	inline void SetObservedCritChance(float crit_chance)
	{
		static auto offset = GetNetVar("CTFWeaponBase", "m_flObservedCritChance");
		*reinterpret_cast<float*>(reinterpret_cast<DWORD>(this) + offset) = crit_chance;
	}

	M_OFFSETGET(UberCharge, float, 0xC6C) //DT_WeaponMedigun -> NonLocalTFWeaponMedigundata -> m_flChargeLevel
	//M_OFFSETGET(HealingTarget, int, 0xC48) //DT_WeaponMedigun -> m_hHealingTarget
	M_OFFSETGET(Healing, int, 0xC51) //DT_WeaponMedigun -> m_bHealing
	M_OFFSETGET(CurrentSeed, int, 0x2D7)
	M_OFFSETGET(CritTokenBucket, float, 0xA54)
	M_OFFSETGET(CritChecks, int, 0xA58)
	M_OFFSETGET(CritSeedRequests, int, 0xA5C)
	M_OFFSETGET(CritTime, float, 0xB50)
	M_OFFSETGET(LastRapidFireCritCheckTime, float, 0xB60)

	NETVAR(m_iPrimaryAmmoType, int, "CBaseCombatWeapon", "m_iPrimaryAmmoType");
	NETVAR(m_flNextPrimaryAttack, float, "CBaseCombatWeapon", "m_flNextPrimaryAttack")

public: //Virtuals
	M_VIRTUALGET(WeaponID, int, this, int(__thiscall*)(void*), 381)
	M_VIRTUALGET(Slot, int, this, int(__thiscall*)(void*), 330)
	M_VIRTUALGET(DamageType, int, this, int(__thiscall*)(void*), 340)
	M_VIRTUALGET(FinishReload, void, this, void(__thiscall*)(void*), 275)
	M_VIRTUALGET(BulletSpread, Vec3&, this, Vec3& (__thiscall*)(void*), 286)

public: //Everything else, lol
	__inline float& GetSmackTime()
	{
//credits to KGB
		static auto dwOffset = g_NetVars.get_offset("DT_TFWeaponBase", "m_nInspectStage") + 0x1C;
		return *reinterpret_cast<float*>(this + dwOffset);
	}

	int GetBulletAmount();

	bool IsStreamingWeapon();

	inline int& m_iWeaponMode()
	{
		static int offset = 716;
		return *reinterpret_cast<int*>(reinterpret_cast<DWORD>(this) + offset);
	}

	inline int& m_iCurrentSeed()
	{
		static int offset = 727;
		return *reinterpret_cast<int*>(reinterpret_cast<DWORD>(this) + offset);
	}

	inline bool GetLocalizedBaseItemName(wchar_t(&szItemName)[128])
	{
		static auto fnGetLocalizedBaseItemName = S::GetLocalizedBaseItemName.As<bool(__cdecl*)(wchar_t(&)[128], const void*, const void*)>();
		static auto fnGLocalizationProvider = S::GLocalizationProvider.As<void* (__cdecl*)()>();
		static auto fnGetStaticData = S::C_EconItemView_GetStaticData.As<void* (__thiscall*)(void*)>();

		void* pItem = m_Item();
		const void* pItemStaticData = fnGetStaticData(pItem);

		return fnGetLocalizedBaseItemName(szItemName, fnGLocalizationProvider(), pItemStaticData);
	}

	NETVAR(m_Item, void*, "CEconEntity", "m_Item");

	/*
	55 8B EC 56 8B 75 10 8B 4E 34 85 C9 74 44 57 8B 7D 0C 51 8B CF 8B 07 FF 10  - GetLocalizedBaseItemName
	A1 ? ? ? ? A8 01 75 12 83 C8 01 C7 05 ? ? ? ? ? ? ? ? A3 ? ? ? ? B8 ? ? ? ? C3 - GLocalizationProvider

	CEconItemDefinition comes from C_EconItemView::GetStaticData so i need to get a C_EconItemView from C_TFWeaponBase
	NETVAR(m_Item, void *, "CEconEntity", "m_Item");
	*/

	inline const char* GetName()
	{
		//static auto C_BaseCombatWeapon_GetName = S::C_BaseCombatWeapon_GetName.As<const char* (__thiscall*)(void*)>();
		return GetVFunc<const char* (__thiscall*)(void*)>(this, 333)(this);
		//return C_BaseCombatWeapon_GetName(this);
	}

	//str8 outta cathook
	__inline bool AmbassadorCanHeadshot()
	{
		if (GetItemDefIndex() == Spy_m_TheAmbassador || GetItemDefIndex() == Spy_m_FestiveAmbassador)
		{
			if ((I::GlobalVars->curtime - GetLastFireTime()) <= 1.0)
			{
				return false;
			}
		}
		return true;
	}

	__inline CAttributeList* GetAttributeList()
	{
		static auto dwOff = g_NetVars.get_offset("DT_EconEntity", "m_AttributeManager", "m_AttributeList");
		return reinterpret_cast<CAttributeList*>(this + dwOff);
	}

	__inline void SetItemDefIndex(const int nIndex)
	{
		static auto dwOff = g_NetVars.get_offset("DT_EconEntity", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
		*reinterpret_cast<int*>(this + dwOff) = nIndex;
	}

	__inline CBaseEntity* GetHealingTarget()
	{
		return I::ClientEntityList->GetClientEntityFromHandle(GetHealingTargetHandle());
	}

	__inline int GetHealingTargetHandle()
	{
		return *reinterpret_cast<int*>(this + 0xC48);
	}

	__inline WeaponData_t GetWeaponData()
	{
		static auto fnGetWeaponData = S::CBaseCombatWeapon_GetWeaponData.As<CTFWeaponInfo * (__cdecl*)(int)>();
		return fnGetWeaponData(GetWeaponID())->m_WeaponData[0];
	}

	__inline bool CanFireCriticalShot()
	{
		static auto fnCanFireCriticalShot = S::CBaseCombatWeapon_CanFireCriticalShot.As<bool* (__cdecl*)(CBaseCombatWeapon*)>();
		return fnCanFireCriticalShot(this);
	}

	__inline CTFWeaponInfo* GetTFWeaponInfo()
	{
		static auto fnGetTFWeaponInfo = S::CBaseCombatWeapon_GetTFWeaponInfo.As<CTFWeaponInfo * (__cdecl*)(int)>();
		return fnGetTFWeaponInfo(GetWeaponID());
	}

	__inline float GetSwingRange(CBaseEntity* pLocal)
	{
		return static_cast<float>(GetVFunc<int(__thiscall*)(CBaseEntity*)>(this, 455)(pLocal));
	}

	__inline float GetWeaponSpread()
	{
		static auto fnGetWeaponSpread = S::CBaseCombatWeapon_GetWeaponSpread.As<float(__thiscall*)(decltype(this))>();
		return fnGetWeaponSpread(this);
	}

	/*__inline bool WillCrit() {
		static auto dwCalcIsAttackCritical = g_Pattern.Find(L"client.dll", L"55 8B EC 83 EC 18 56 57 6A 00 68 ? ? ? ? 68 ? ? ? ? 6A 00 8B F9 E8 ? ? ? ? 50 E8 ? ? ? ? 8B F0 83 C4 14 89 75 EC");
		return reinterpret_cast<bool(__thiscall*)(decltype(this))>(dwCalcIsAttackCritical);
	}*/

	/*__inline bool CalcIsAttackCritical() {
		typedef bool(__thiscall* OriginalFn)(CBaseCombatWeapon*);
		static DWORD dwFunc = g_Pattern.Find(L"client.dll", L"55 8B EC 83 EC 18 56 57 6A 00 68 ? ? ? ? 68 ? ? ? ? 6A 00 8B F9 E8 ? ? ? ? 50 E8 ? ? ? ? 8B F0 83 C4 14 89 75 EC");
		return ((OriginalFn)dwFunc)(this);
	}*/

	__inline bool DoSwingTrace(CGameTrace& Trace)
	{
		return GetVFunc<int(__thiscall*)(CGameTrace&)>(this, 454)(Trace);
	}

	__inline bool DoSwingTraceInternal(CGameTrace& Trace)
	{
		static auto fnDoSwingTraceInternal = S::CTFWeaponBaseMelee_DoSwingTraceInternal.As<bool(__thiscall*)(decltype(this), CGameTrace&, bool, void*)>();
		return fnDoSwingTraceInternal(this, Trace, false, nullptr);
	}

	__inline int LookupAttachment(const char* pAttachmentName)
	{
		const auto pRend = Renderable();
		return GetVFunc<int(__thiscall*)(void*, const char*)>(pRend, 35)(pRend, pAttachmentName);
	}

	__inline bool GetAttachment(int number, Vec3& origin)
	{
		return GetVFunc<bool(__thiscall*)(void*, int, Vec3&)>(this, 71)(this, number, origin);
	}


	__inline bool CanFireCriticalShot(const bool bHeadShot)
	{
		bool bResult = false;
		if (const auto& pOwner = I::ClientEntityList->GetClientEntityFromHandle(GethOwner()))
		{
			const int nOldFov = pOwner->GetFov(); pOwner->SetFov(-1);
			bResult = GetVFunc<bool(__thiscall*)(decltype(this), bool, CBaseEntity*)>(this, 425)(this, bHeadShot, nullptr);
			pOwner->SetFov(nOldFov);
		} return bResult;
	}

	__inline bool CanFireRandomCriticalShot(const float flCritChance)
	{
		return GetVFunc<bool(__thiscall*)(decltype(this), float)>(this, 424)(this, flCritChance);
	}

	__inline bool CanWeaponHeadShot()
	{
		return ((GetDamageType() & DMG_USE_HITLOCATIONS) && CanFireCriticalShot(true)); //credits to bertti
	}

	__inline bool CanShoot(CBaseEntity* pLocal)
	{
		if (!pLocal->IsAlive() || pLocal->IsTaunting() || pLocal->IsBonked() || pLocal->IsAGhost() || pLocal->IsInBumperKart() || pLocal->m_fFlags() & FL_FROZEN)
			return false;

		// there might be a better way of doing this (whatever the model uses ?)
		if (GetWeaponID() == TF_WEAPON_FLAME_BALL)
		{
			static bool bCooldown = false;
			if (bCooldown && GetRechargeScale() == 1.f)
			{
				return pLocal->GetTickBase() - GetLastFireTime() / I::GlobalVars->interval_per_tick > 53;
			}
			else
			{
				if (GetRechargeScale() == 1.f)
				{
					bCooldown = true;
					return true;
				}
				bCooldown = false;
				return false;
			}
		}

		if (pLocal->GetClassNum() == CLASS_SPY)
		{
			if (pLocal->GetFeignDeathReady() && !pLocal->IsCloaked())
				return false;

			{ //Invis
				static float flTimer = 0.0f;

				if (pLocal->IsCloaked())
				{
					flTimer = 0.0f;
					return false;
				}
				else
				{
					if (!flTimer)
						flTimer = I::GlobalVars->curtime;

					if (flTimer > I::GlobalVars->curtime)
						flTimer = 0.0f;

					if ((I::GlobalVars->curtime - flTimer) < 2.0f)
						return false;
				}
			}
		}

		float flCurTime = static_cast<float>(pLocal->GetTickBase()) * I::GlobalVars->interval_per_tick;

		return GetNextPrimaryAttack() <= flCurTime && pLocal->GetNextAttack() <= flCurTime;
	}

	__inline bool CanSecondaryAttack(CBaseEntity* pLocal)
	{
		if (!pLocal->IsAlive() || pLocal->IsTaunting() || pLocal->IsBonked() || pLocal->IsAGhost() || pLocal->IsInBumperKart())
			return false;

		float flCurTime = static_cast<float>(pLocal->GetTickBase()) * I::GlobalVars->interval_per_tick;

		return GetNextSecondaryAttack() <= flCurTime && pLocal->GetNextAttack() <= flCurTime;
	}

	__inline bool IsInReload()
	{
		static DWORD dwNextPrimaryAttack = g_NetVars.get_offset("DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack");
		bool m_bInReload = *reinterpret_cast<bool*>(this + (dwNextPrimaryAttack + 0xC));
		int m_iReloadMode = *reinterpret_cast<int*>(this + 0xB28);
		return (m_bInReload || m_iReloadMode != 0);
	}

	__inline void GetSpreadAngles(Vec3& vOut)
	{
		static auto fnGetSpreadAngles = S::CBaseCombatWeapon_GetSpreadAngles.As<void(__thiscall*)(decltype(this), Vec3&)>();
		fnGetSpreadAngles(this, vOut);
	}

	__inline void GetProjectileFireSetup(CBaseEntity* pPlayer, Vec3 vOffset, Vec3* vSrc, Vec3* vForward, bool bHitTeam, float flEndDist)
	{
		static auto fnGetProjectileFireSetu = S::CBaseCombatWeapon_GetProjectileFireSetup.As<void(__thiscall*)(CBaseEntity*, CBaseEntity*, Vec3, Vec3*, Vec3*, bool, float)>();
		fnGetProjectileFireSetu(this, pPlayer, vOffset, vSrc, vForward, bHitTeam, flEndDist);
	}

	__inline bool IsRapidFire()
	{
		const bool ret = GetWeaponData().m_bUseRapidFireCrits;
		return ret || this->GetClientClass()->m_ClassID == static_cast<int>(ETFClassID::CTFMinigun);
	}



	__inline bool WillCrit()
	{
		return this->GetSlot() == SLOT_MELEE ? this->CalcIsAttackCriticalHelperMelee() : this->CalcIsAttackCriticalHelper();
	}

	__inline bool CalcIsAttackCritical()
	{
		using FN = bool(__thiscall*)(void*);
		static auto fnCalcIsAttackCritical = S::CBaseCombatWeapon_CalcIsAttackCritical.As<FN>();

		return fnCalcIsAttackCritical(this);
	}

	__inline bool CalcIsAttackCriticalHelper()
	{
		using FN = bool(__thiscall*)(CBaseCombatWeapon*);
		static FN fnCalcIsAttackCriticalHelper = S::CBaseCombatWeapon_CalcIsAttackCriticalHelper.As<FN>();
		return fnCalcIsAttackCriticalHelper(this);
	}

	__inline bool CalcIsAttackCriticalHelperMelee()
	{
		using FN = bool(__thiscall*)(CBaseCombatWeapon*);
		static FN fnCalcIsAttackCriticalHelper = S::CBaseCombatWeapon_CalcIsAttackCriticalHelperMelee.As<FN>();
		return fnCalcIsAttackCriticalHelper(this);
	}

	__inline bool CalcIsAttackCriticalHelperNoCrits(CBaseEntity* pWeapon)
	{
		typedef bool (*fn_t)(CBaseEntity*);
		return GetVFunc<fn_t>(pWeapon, 464, 0)(pWeapon);
	}

	//__inline bool CanFireCriticalShot(CBaseEntity* pWeapon)		// this does not fucking work no matter what i do and i have no idea why :DDD
	//{
	//	typedef bool (*fn_t)(CBaseEntity*, bool, CBaseEntity*);
	//	return GetVFunc<fn_t>(this, 492)(pWeapon, false, nullptr);
	//}

	__inline Vec3 GetSpreadAngles()
	{
		Vec3 vOut; GetSpreadAngles(vOut); return vOut;
	}

	__inline int GetMinigunState()
	{
		return *reinterpret_cast<int*>(this + 0xC48);
	}

	__inline bool IsReadyToFire()
	{
		static float lastFire = 0, nextAttack = 0;

		if (lastFire != GetLastFireTime())
		{
			lastFire = GetLastFireTime();
			nextAttack = GetNextPrimaryAttack();
		}

		if (GetClip1() == 0)
			return false;
		return (nextAttack <= (TICKS_TO_TIME(I::ClientEntityList->GetClientEntity(I::EngineClient->GetLocalPlayer())->GetTickBase())));
	}

	__inline bool IsFlipped()
	{
		static auto cl_flipviewmodels = I::Cvar->FindVar("cl_flipviewmodels");
		return cl_flipviewmodels->GetBool();
	}

	__inline float GetFireRate()
	{
		typedef float(__thiscall* FN)(PVOID);
		return GetVFunc<FN>(this, 359)(this);
	}

	CHudTexture* GetWeaponIcon();
};

class CTFWeaponInvis : public CBaseCombatWeapon
{
public:
	__inline bool HasFeignDeath()
	{
		return static_cast<bool>(GetVFunc<bool(__thiscall*)(CTFWeaponInvis*)>(this, 523));
	}
};