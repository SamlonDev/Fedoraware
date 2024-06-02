#pragma once
#include "../Feature.h"
#include <deque>
#include <unordered_map>

typedef int i32;
typedef unsigned int u32;

struct WeaponStorage {
	float Bucket;

	float Damage;
	std::pair<int, int> ShotsCrits;
	float Cost;

	int AvailableCrits;
	int PotentialCrits;

	int StreamWait;
	int StreamEnd;

	int EntIndex;
	int DefIndex;

	std::deque<int> CritCommands;
	std::deque<int> SkipCommands;

	WeaponStorage() :
		Bucket(300.f),
		Damage(0.f),
		ShotsCrits({ 1, 0 }),
		Cost(0.f),
		AvailableCrits(0),
		PotentialCrits(0),
		StreamWait(-1),
		StreamEnd(-1),
		EntIndex(-1),
		DefIndex(-1) {}
};

class CCritHack : public Feature {
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

	float CritDamage;
	float AllDamage;

	bool CritBanned;
	int DamageTilUnban;
	float CritChance;
	int WishRandomSeed;

	float BucketDefault;
	float BucketBottom;
	float BucketCap;

public:
	CCritHack() :
		Feature("CritHack"),
		CritDamage(0),
		AllDamage(0),
		CritBanned(false),
		DamageTilUnban(0),
		CritChance(0.f),
		WishRandomSeed(0),
		BucketDefault(300.f),
		BucketBottom(-250.f),
		BucketCap(1000.f) {}

	~CCritHack() {}

	void Run(CUserCmd* pCmd);
	bool CalcIsAttackCriticalHandler(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon);
	void Event(CGameEvent* pEvent, FNV1A_t uNameHash);
	void Draw();

	std::unordered_map<int, WeaponStorage> Storage;
	bool ProtectData;
};

ADD_FEATURE(CCritHack, CritHack)
