#pragma once

#include "../BaseEntity/BaseEntity.h"

#ifndef TICKS_TO_TIME
#define TICKS_TO_TIME(t) (I::GlobalVars->interval_per_tick * (t))
#endif

namespace S
{
	MAKE_SIGNATURE(CBaseCombatWeapon_HasAmmo, CLIENT_DLL, "56 8B F1 83 BE ? ? ? ? ? 75 0D 83 BE ? ? ? ? ? 75 04", 0x0);

	MAKE_SIGNATURE(CBaseCombatWeapon_CanFireCriticalShot, CLIENT_DLL, "6A ? 68 ? ? ? ? 68 ? ? ? ? 6A ? E8 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? C3 CC CC CC 55", 0x0);
	MAKE_SIGNATURE(CBaseCombatWeapon_GetTFWeaponInfo, CLIENT_DLL, "55 8B EC FF 75 ? E8 ? ? ? ? 83 C4 ? 85 C0 75 ? 5D C3", 0x0);
	MAKE_SIGNATURE(CBaseCombatWeapon_GetProjectileFireSetup, CLIENT_DLL, "53 8B DC 83 EC ? 83 E4 ? 83 C4 ? 55 8B 6B ? 89 6C 24 ? 8B EC 81 EC ? ? ? ? 56 8B F1 57 8B 06 8B 80 ? ? ? ? FF D0 84 C0", 0x0);
	MAKE_SIGNATURE(CBaseCombatWeapon_GetSpreadAngles, CLIENT_DLL, "55 8B EC 83 EC ? 56 57 6A ? 68 ? ? ? ? 68 ? ? ? ? 6A ? 8B F9 E8 ? ? ? ? 50 E8 ? ? ? ? 8B F0 83 C4 ? 85 F6", 0x0);
	MAKE_SIGNATURE(CBaseCombatWeapon_GetWeaponSpread, CLIENT_DLL, "55 8B EC 83 EC ? 56 8B F1 57 6A ? 6A", 0x0);
	MAKE_SIGNATURE(CBaseCombatWeapon_CalcIsAttackCritical, CLIENT_DLL, "53 57 6A ? 68 ? ? ? ? 68 ? ? ? ? 6A ? 8B F9 E8 ? ? ? ? 50 E8 ? ? ? ? 8B D8 83 C4 ? 85 DB 0F 84", 0x0);
	MAKE_SIGNATURE(CBaseCombatWeapon_CalcIsAttackCriticalHelper, CLIENT_DLL, "55 8B EC 83 EC ? 56 57 6A ? 68 ? ? ? ? 68 ? ? ? ? 6A ? 8B F9 E8 ? ? ? ? 50 E8 ? ? ? ? 8B F0 83 C4 ? 89 75", 0x0);
	MAKE_SIGNATURE(CBaseCombatWeapon_CalcIsAttackCriticalHelperMelee, CLIENT_DLL, "55 8B EC A1 ? ? ? ? 83 EC ? 83 78 ? ? 57 8B F9 75", 0x0);
	MAKE_SIGNATURE(CTFWeaponBaseMelee_DoSwingTraceInternal, CLIENT_DLL, "53 8B DC 83 EC ? 83 E4 ? 83 C4 ? 55 8B 6B ? 89 6C 24 ? 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 56 8B F1", 0x0);
	MAKE_SIGNATURE(CTFWeaponBase_GetAppropriateWorldOrViewModel, CLIENT_DLL, "56 57 6A 00 68 ? ? ? ? 68 ? ? ? ? 6A 00 8B F1 E8 ? ? ? ? 50 E8 ? ? ? ? 8B F8 83 C4 14 85 FF 74 ? 8B CE", 0x0);
	MAKE_SIGNATURE(CTFWeaponBase_UpdateAllViewmodelAddons, CLIENT_DLL, "55 8B EC 83 EC 08 56 57 8B F1 E8 ? ? ? ? 8B F8 85 FF 0F 84 ? ? ? ? 8B 17", 0x0);

	MAKE_SIGNATURE(GetLocalizedBaseItemName, CLIENT_DLL, "55 8B EC 56 8B 75 ? 8B 4E ? 85 C9 74 ? 57", 0x0);
	MAKE_SIGNATURE(GLocalizationProvider, CLIENT_DLL, "A1 ? ? ? ? A8 ? 75 ? 83 C8 ? C7 05 ? ? ? ? ? ? ? ? A3 ? ? ? ? B8 ? ? ? ? C3", 0x0);
	MAKE_SIGNATURE(C_EconItemView_GetStaticData, CLIENT_DLL, "0F B7 41 ? 50 E8 ? ? ? ? 8B C8 E8 ? ? ? ? 6A ? 68 ? ? ? ? 68 ? ? ? ? 6A ? 50 E8 ? ? ? ? 83 C4 ? C3 CC CC CC CC CC CC CC 83 C1", 0x0);
}

class CBaseCombatWeapon : public CBaseEntity
{
public: //Netvars
	NETVAR(m_iClip1, int, "CBaseCombatWeapon", "m_iClip1")
	NETVAR(m_iClip2, int, "CBaseCombatWeapon", "m_iClip2")
	NETVAR(m_iPrimaryAmmoType, int, "CBaseCombatWeapon", "m_iPrimaryAmmoType")
	NETVAR(m_iSecondaryAmmoType, int, "CBaseCombatWeapon", "m_iSecondaryAmmoType")
	NETVAR(m_nViewModelIndex, int, "CBaseCombatWeapon", "m_nViewModelIndex")
	NETVAR(m_bFlipViewModel, bool, "CBaseCombatWeapon", "m_bFlipViewModel")
	NETVAR(m_flNextPrimaryAttack, float, "CBaseCombatWeapon", "m_flNextPrimaryAttack")
	NETVAR(m_flNextSecondaryAttack, float, "CBaseCombatWeapon", "m_flNextSecondaryAttack")
	NETVAR(m_nNextThinkTick, int, "CBaseCombatWeapon", "m_nNextThinkTick")
	NETVAR(m_flTimeWeaponIdle, float, "CBaseCombatWeapon", "m_flTimeWeaponIdle")
	NETVAR(m_iViewModelIndex, int, "CBaseCombatWeapon", "m_iViewModelIndex")
	NETVAR(m_iWorldModelIndex, int, "CBaseCombatWeapon", "m_iWorldModelIndex")
	NETVAR(m_iState, int, "CBaseCombatWeapon", "m_iState")
	NETVAR(m_hOwner, int /*EHANDLE*/, "CBaseCombatWeapon", "m_hOwner")

	bool HasAmmo()
	{
		return S::CBaseCombatWeapon_HasAmmo.As<bool(__thiscall*)(void*)>()(this);
	}

	NETVAR(m_bLowered, bool, "CTFWeaponBase", "m_bLowered")
	NETVAR(m_iReloadMode, int, "CTFWeaponBase", "m_iReloadMode")
	NETVAR(m_bResetParity, bool, "CTFWeaponBase", "m_bResetParity")
	NETVAR(m_bReloadedThroughAnimEvent, bool, "CTFWeaponBase", "m_bReloadedThroughAnimEvent")
	NETVAR(m_bDisguiseWeapon, bool, "CTFWeaponBase", "m_bDisguiseWeapon")
	NETVAR(m_flLastCritCheckTime, float, "CTFWeaponBase", "m_flLastCritCheckTime")
	NETVAR(m_flReloadPriorNextFire, float, "CTFWeaponBase", "m_flReloadPriorNextFire")
	NETVAR(m_flLastFireTime, float, "CTFWeaponBase", "m_flLastFireTime")
	NETVAR(m_flEffectBarRegenTime, float, "CTFWeaponBase", "m_flEffectBarRegenTime")
	NETVAR(m_flObservedCritChance, float, "CTFWeaponBase", "m_flObservedCritChance")
	NETVAR(m_flEnergy, float, "CTFWeaponBase", "m_flEnergy")
	NETVAR(m_hExtraWearable, int /*EHANDLE*/, "CTFWeaponBase", "m_hExtraWearable")
	NETVAR(m_hExtraWearableViewModel, int /*EHANDLE*/, "CTFWeaponBase", "m_hExtraWearableViewModel")
	NETVAR(m_bBeingRepurposedForTaunt, bool, "CTFWeaponBase", "m_bBeingRepurposedForTaunt")
	NETVAR(m_nKillComboClass, int, "CTFWeaponBase", "m_nKillComboClass")
	NETVAR(m_nKillComboCount, int, "CTFWeaponBase", "m_nKillComboCount")
	NETVAR(m_flInspectAnimEndTime, float, "CTFWeaponBase", "m_flInspectAnimEndTime")
	NETVAR(m_nInspectStage, int, "CTFWeaponBase", "m_nInspectStage")
	NETVAR_OFF(m_flSmackTime, float, "CTFWeaponBase", "m_nInspectStage", 0x1C)
	NETVAR(m_iConsecutiveShots, int, "CTFWeaponBase", "m_iConsecutiveShots")

	OFFSET(CritTokenBucket, float, 0xA54)
	OFFSET(CritChecks, int, 0xA58)
	OFFSET(CritSeedRequests, int, 0xA5C)
	OFFSET(CritTime, float, 0xB50)
	OFFSET(LastCritCheckTime, float, 0xB54)
	OFFSET(LastCritCheckFrame, int, 0xB58)
	OFFSET(LastRapidfireCritCheckTime, float, 0xB60)
	OFFSET(CritShot, bool, 0xB36)
	OFFSET(RandomSeed, int, 0xB5C)

	VIRTUAL(Slot, int, this, int(__thiscall*)(void*), 330)
	VIRTUAL(WeaponID, int, this, int(__thiscall*)(void*), 381)
	VIRTUAL(DamageType, int, this, int(__thiscall*)(void*), 340)
	VIRTUAL(FinishReload, void, this, void(__thiscall*)(void*), 275)
	VIRTUAL(BulletSpread, Vec3&, this, Vec3& (__thiscall*)(void*), 286)

	int GetBulletAmount();
	bool IsStreamingWeapon();
	CHudTexture* GetWeaponIcon();
	__inline bool CanAttack(CBaseEntity* pOwner)
	{
		if (!pOwner->IsAlive() || pOwner->IsTaunting() || pOwner->IsBonked() || pOwner->IsAGhost() || pOwner->IsInBumperKart() || pOwner->m_fFlags() & FL_FROZEN)
			return false;

		if (pOwner->m_iClass() == CLASS_SPY)
		{
			if (pOwner->m_bFeignDeathReady() && !pOwner->IsCloaked())
				return false;

			//Invis
			static float flTimer = 0.f;
			if (pOwner->IsCloaked())
			{
				flTimer = 0.f;
				return false;
			}
			else
			{
				if (!flTimer)
					flTimer = I::GlobalVars->curtime;

				if (flTimer > I::GlobalVars->curtime)
					flTimer = 0.f;

				if ((I::GlobalVars->curtime - flTimer) < 2.f)
					return false;
			}
		}

		return true;
	}
	__inline bool CanPrimaryAttack(CBaseEntity* pOwner)
	{
		bool bOut = false;

		if (pOwner)
		{
			float flCurTime = static_cast<float>(pOwner->m_nTickBase()) * I::GlobalVars->interval_per_tick;
			bOut = m_flNextPrimaryAttack() <= flCurTime && pOwner->m_flNextAttack() <= flCurTime;
		}

		return bOut;
	}
	__inline bool CanSecondaryAttack(CBaseEntity* pOwner)
	{
		bool bOut = false;

		if (pOwner)
		{
			float flCurTime = static_cast<float>(pOwner->m_nTickBase()) * I::GlobalVars->interval_per_tick;
			bOut = m_flNextSecondaryAttack() <= flCurTime && pOwner->m_flNextAttack() <= flCurTime;
		}

		return bOut;
	}

	__inline bool CanPrimary(CBaseEntity* pLocal)
	{
		if (!CanAttack(pLocal))
			return false;

		if (GetWeaponID() == TF_WEAPON_FLAME_BALL)
			return pLocal->m_flTankPressure() >= 100.f;

		return CanPrimaryAttack(pLocal);
	}
	__inline bool CanSecondary(CBaseEntity* pLocal)
	{
		if (!CanAttack(pLocal))
			return false;

		if (GetWeaponID() == TF_WEAPON_FLAME_BALL)
			return pLocal->m_flTankPressure() >= 100.f;

		return CanSecondaryAttack(pLocal);
	}
	__inline bool IsInReload()
	{
		static int nOffset = g_NetVars.GetNetVar("CBaseCombatWeapon", "m_flNextPrimaryAttack");
		bool m_bInReload = *reinterpret_cast<bool*>(reinterpret_cast<DWORD>(this) + nOffset + 0xC);
		return (m_bInReload || m_iReloadMode() != 0);
	}
	__inline float GetFireRate()
	{
		typedef float(__thiscall* FN)(PVOID);
		return GetVFunc<FN>(this, 359)(this);
	}
	__inline bool CanFireCriticalShot(const bool bHeadShot)
	{
		bool bResult = false;
		if (const auto& pOwner = I::ClientEntityList->GetClientEntityFromHandle(m_hOwnerEntity()))
		{
			const int nOldFov = pOwner->m_iFOV(); pOwner->m_iFOV() = -1;
			bResult = GetVFunc<bool(__thiscall*)(decltype(this), bool, CBaseEntity*)>(this, 425)(this, bHeadShot, nullptr);
			pOwner->m_iFOV() = nOldFov;
		} return bResult;
	}
	__inline bool CanFireCriticalShot()
	{
		return S::CBaseCombatWeapon_CanFireCriticalShot.As<bool*(__cdecl*)(CBaseCombatWeapon*)>()(this);
	}
	__inline bool CanFireRandomCriticalShot(const float flCritChance)
	{
		return GetVFunc<bool(__thiscall*)(decltype(this), float)>(this, 424)(this, flCritChance);
	}
	//__inline bool CanFireCriticalShot(CBaseEntity* pWeapon)		// this does not fucking work no matter what i do and i have no idea why :DDD
	//{
	//	typedef bool (*fn_t)(CBaseEntity*, bool, CBaseEntity*);
	//	return GetVFunc<fn_t>(this, 492)(pWeapon, false, nullptr);
	//}
	__inline bool CanWeaponHeadShot()
	{
		return GetDamageType() & DMG_USE_HITLOCATIONS && CanFireCriticalShot(true); //credits to bertti
	}
	//str8 outta cathook
	__inline bool AmbassadorCanHeadshot()
	{
		if (m_iItemDefinitionIndex() == Spy_m_TheAmbassador || m_iItemDefinitionIndex() == Spy_m_FestiveAmbassador)
		{
			if ((I::GlobalVars->curtime - m_flLastFireTime()) <= 1.0)
				return false;
		}
		return true;
	}
	__inline float GetSwingRange(CBaseEntity* pLocal)
	{
		return static_cast<float>(GetVFunc<int(__thiscall*)(CBaseEntity*)>(this, 455)(pLocal));
	}
	__inline float GetSwingRangeSimple()
	{
		return GetWeaponID() == TF_WEAPON_SWORD ? 72.f : 48.f;
	}
	__inline CTFWeaponInfo* GetTFWeaponInfo()
	{
		return S::CBaseCombatWeapon_GetTFWeaponInfo.As<CTFWeaponInfo * (__cdecl*)(int)>()(GetWeaponID());
	}
	__inline WeaponData_t GetWeaponData()
	{
		return GetTFWeaponInfo()->m_WeaponData[0];
	}
	__inline CTFWeaponInfo* GetWeaponInfo()
	{
		static int nOffset = g_NetVars.GetNetVar("CTFWeaponBase", "m_flEffectBarRegenTime") + 0x10;
		return *reinterpret_cast<CTFWeaponInfo**>(reinterpret_cast<DWORD>(this) + nOffset);
	}
	__inline bool IsRapidFire()
	{
		auto tfWeaponInfo = GetTFWeaponInfo();
		if (!tfWeaponInfo)
			return false;

		return tfWeaponInfo->GetWeaponData(0).m_bUseRapidFireCrits;
	}
	__inline bool IsEnergyWeapon()
	{
		return GetVFunc<bool(__thiscall*)(void*)>(this, 432)(this);
	}
	__inline bool HasPrimaryAmmoForShot()
	{
		if (IsEnergyWeapon())
			return m_flEnergy() > 0.f;

		int nClip1 = m_iClip1();

		if (nClip1 == -1)
		{
			if (auto pOwner = I::ClientEntityList->GetClientEntityFromHandle(m_hOwnerEntity()))
			{
				int nAmmoCount = pOwner->GetAmmoCount(m_iPrimaryAmmoType());

				if (m_iItemDefinitionIndex() == Engi_m_TheWidowmaker)
					return nAmmoCount > 29;

				return nAmmoCount > 0;
			}
		}

		return nClip1 > 0;
	}
	__inline int& m_iWeaponMode()
	{
		static int offset = 716;
		return *reinterpret_cast<int*>(reinterpret_cast<DWORD>(this) + offset);
	}
	__inline int& m_iCurrentSeed()
	{
		return *reinterpret_cast<int*>(reinterpret_cast<DWORD>(this) + 0xB5C);
	}
	__inline void GetProjectileFireSetup(CBaseEntity* pPlayer, Vec3 vOffset, Vec3* vSrc, Vec3* vForward, bool bHitTeam = true, float flEndDist = 2000.f)
	{
		S::CBaseCombatWeapon_GetProjectileFireSetup.As<void(__thiscall*)(CBaseEntity*, CBaseEntity*, Vec3, Vec3*, Vec3*, bool, float)>()(this, pPlayer, vOffset, vSrc, vForward, bHitTeam, flEndDist);
	}
	__inline void GetSpreadAngles(Vec3& vOut)
	{
		S::CBaseCombatWeapon_GetSpreadAngles.As<void(__thiscall*)(decltype(this), Vec3&)>()(this, vOut);
	}
	__inline Vec3 GetSpreadAngles()
	{
		Vec3 vOut; GetSpreadAngles(vOut); return vOut;
	}
	__inline void UpdateAllViewmodelAddons()
	{
		return S::CTFWeaponBase_UpdateAllViewmodelAddons.As<void(__thiscall*)(void*)>()(this);
	}
	__inline float ApplyFireDelay(float flDelay)
	{
		return GetVFunc<float(__thiscall*)(void*, float)>(this, 407)(this, flDelay);
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
		return S::CBaseCombatWeapon_CalcIsAttackCriticalHelper.As<bool(__thiscall*)(CBaseCombatWeapon*)>()(this);
	}
	__inline bool CalcIsAttackCriticalHelperMelee()
	{
		return S::CBaseCombatWeapon_CalcIsAttackCriticalHelperMelee.As<bool(__thiscall*)(CBaseCombatWeapon*)>()(this);
	}
	__inline bool CalcIsAttackCriticalHelperNoCrits(CBaseEntity* pWeapon)
	{
		typedef bool (*fn_t)(CBaseEntity*);
		return GetVFunc<fn_t>(pWeapon, 464, 0)(pWeapon);
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
	__inline void SetObservedCritChance(float crit_chance)
	{
		static auto nOffset = g_NetVars.GetNetVar("CTFWeaponBase", "m_flObservedCritChance");
		*reinterpret_cast<float*>(reinterpret_cast<DWORD>(this) + nOffset) = crit_chance;
	}
	__inline CBaseEntity* GetAppropriateWorldOrViewModel()
	{
		return S::CTFWeaponBase_GetAppropriateWorldOrViewModel.As<CBaseEntity*(__thiscall*)(void*)>()(this);
	}
	__inline float GetWeaponSpread()
	{
		return S::CBaseCombatWeapon_GetWeaponSpread.As<float(__thiscall*)(decltype(this))>()(this);
	}
	__inline bool DoSwingTrace(CGameTrace& Trace)
	{
		return GetVFunc<int(__thiscall*)(CGameTrace&)>(this, 454)(Trace);
	}
	__inline bool DoSwingTraceInternal(CGameTrace& Trace)
	{
		return S::CTFWeaponBaseMelee_DoSwingTraceInternal.As<bool(__thiscall*)(decltype(this), CGameTrace&, bool, void*)>()(this, Trace, false, nullptr);
	}
	__inline int GetMinigunState()
	{
		return *reinterpret_cast<int*>(this + 0xC48);
	}

	NETVAR(m_bReadyToBackstab, bool, "CTFKnife", "m_bReadyToBackstab")
	NETVAR(m_bKnifeExists, bool, "CTFKnife", "m_bKnifeExists")
	NETVAR(m_flKnifeRegenerateDuration, float, "CTFKnife", "m_flKnifeRegenerateDuration")
	NETVAR(m_flKnifeMeltTimestamp, float, "CTFKnife", "m_flKnifeMeltTimestamp")

	NETVAR(m_iWeaponState, int, "CTFMinigun", "m_iWeaponState")
	NETVAR(m_bCritShot, bool, "CTFMinigun", "m_bCritShot")

	NETVAR(m_hHealingTarget, int /*EHANDLE*/, "CWeaponMedigun", "m_hHealingTarget")
	NETVAR(m_bHealing, bool, "CWeaponMedigun", "m_bHealing")
	NETVAR(m_bAttacking, bool, "CWeaponMedigun", "m_bAttacking")
	NETVAR(m_bChargeRelease, bool, "CWeaponMedigun", "m_bChargeRelease")
	NETVAR(m_bHolstered, bool, "CWeaponMedigun", "m_bHolstered")
	NETVAR(m_nChargeResistType, int, "CWeaponMedigun", "m_nChargeResistType")
	NETVAR(m_hLastHealingTarget, int /*EHANDLE*/, "CWeaponMedigun", "m_hLastHealingTarget")
	NETVAR(m_flChargeLevel, float, "CWeaponMedigun", "m_flChargeLevel")
	
	int GetMedigunType();
	MedigunChargeTypes GetChargeType();
	__inline MedigunChargeTypes GetResistType()
	{
		int nCurrentActiveResist = (GetChargeType() - MEDIGUN_CHARGE_BULLET_RESIST);
		nCurrentActiveResist = nCurrentActiveResist % MEDIGUN_NUM_RESISTS;
		return MedigunChargeTypes(nCurrentActiveResist);
	}
	__inline CBaseEntity* GetHealingTarget()
	{
		return I::ClientEntityList->GetClientEntityFromHandle(GetHealingTargetHandle());
	}
	__inline int GetHealingTargetHandle()
	{
		return *reinterpret_cast<int*>(this + 0xC48);
	}

	NETVAR(m_flChargeBeginTime, float, "CTFPipebombLauncher", "m_flChargeBeginTime")
	NETVAR(m_iPipebombCount, int, "CTFPipebombLauncher", "m_iPipebombCount")

	NETVAR(m_flChargedDamage, float, "CTFSniperRifle", "m_flChargedDamage")

	NETVAR(m_flDetonateTime, float, "CTFGrenadeLauncher", "m_flDetonateTime")
	NETVAR(m_iCurrentTube, int, "CTFGrenadeLauncher", "m_iCurrentTube")
	NETVAR(m_iGoalTube, int, "CTFGrenadeLauncher", "m_iGoalTube")

	NETVAR(m_bCharging, bool, "CTFSniperRifleClassic", "m_bCharging")

	NETVAR(m_iItemDefinitionIndex, int, "CEconEntity", "m_iItemDefinitionIndex")
	NETVAR(m_Item, void*, "CEconEntity", "m_Item")

	__inline CAttributeList* GetAttributeList()
	{
		static int nOffset = g_NetVars.GetNetVar("CEconEntity", "m_AttributeList");
		return reinterpret_cast<CAttributeList*>(reinterpret_cast<DWORD>(this) + nOffset);
	}

	__inline void SetItemDefIndex(const int nIndex)
	{
		static auto nOffset = g_NetVars.GetNetVar("CEconEntity", "m_iItemDefinitionIndex");
		*reinterpret_cast<int*>(reinterpret_cast<DWORD>(this) + nOffset) = nIndex;
	}
	__inline bool GetLocalizedBaseItemName(wchar_t(&szItemName)[128])
	{
		static auto fnGetLocalizedBaseItemName = S::GetLocalizedBaseItemName.As<bool(__cdecl*)(wchar_t(&)[128], const void*, const void*)>();
		static auto fnGLocalizationProvider = S::GLocalizationProvider.As<void* (__cdecl*)()>();
		static auto fnGetStaticData = S::C_EconItemView_GetStaticData.As<void* (__thiscall*)(void*)>();

		void* pItem = m_Item();
		const void* pItemStaticData = fnGetStaticData(pItem);

		return fnGetLocalizedBaseItemName(szItemName, fnGLocalizationProvider(), pItemStaticData);
	}
	__inline const char* GetName()
	{
		return GetVFunc<const char* (__thiscall*)(void*)>(this, 333)(this);
	}
};

class CTFWeaponInvis : public CBaseCombatWeapon
{
public:
	__inline bool HasFeignDeath()
	{
		return static_cast<bool>(GetVFunc<bool(__thiscall*)(CTFWeaponInvis*)>(this, 523));
	}
};