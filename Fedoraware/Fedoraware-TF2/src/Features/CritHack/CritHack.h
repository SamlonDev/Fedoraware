#pragma once
#include "../Feature.h"

typedef int                i32;
typedef unsigned int       u32;

struct WeaponStorage
{
	float Damage = 0.f;
	float Cost = 0.f;
	int AvailableCrits = 0;
	int PotentialCrits = 0;
	int StreamWait = -1;

	int EntIndex = -1;
	int DefIndex = -1;

	std::deque<int> CritCommands = {};
	std::deque<int> SkipCommands = {};
};

class CCritHack
{
private:
	void Fill(CBaseEntity* pLocal, const CUserCmd* pCmd, int n = 10);
	int FirstGoodCommand(std::deque<int>& vCommands, const CUserCmd* pCmd);

	bool IsCritCommand(int iSlot, int iIndex, const i32 command_number, const bool bCrit = true, const bool bSafe = true);
	u32 DecryptOrEncryptSeed(int iSlot, int iIndex, const u32 uSeed);

	void GetTotalCrits(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon);
	void CanFireCritical(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon);
	void GetDamageTilUnban(CBaseEntity* pLocal);
	bool WeaponCanCrit(CBaseCombatWeapon* weapon);

	void ResetWeapons(CBaseEntity* pLocal);
	void Reset();

	float CritDamage = 0;
	float AllDamage = 0;

	bool CritBanned = false;
	int DamageTilUnban = 0;
	float CritChance = 0.f;
	int WishRandomSeed = 0;

public:
	void Run(CUserCmd* pCmd);
	bool CalcIsAttackCriticalHandler(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon);
	void Event(CGameEvent* pEvent, FNV1A_t uNameHash);
	void Draw();

	std::unordered_map<int, WeaponStorage> Storage = {};
	bool ProtectData = false;
};

ADD_FEATURE(CCritHack, CritHack)