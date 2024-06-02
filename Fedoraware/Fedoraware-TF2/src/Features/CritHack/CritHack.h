#ifndef CRIT_HACK_H
#define CRIT_HACK_H

#include "../Feature.h"
#include <deque>
#include <unordered_map>

class CCritHack : public Feature
{
private:
	// Member variables
	float CritDamage = 0;
	float AllDamage = 0;
	bool CritBanned = false;
	int DamageTilUnban = 0;
	float CritChance = 0.f;
	int WishRandomSeed = 0;
	bool ProtectData = false;

	// Helper methods
	void Fill(CBaseEntity* pLocal, const CUserCmd* pCmd, int n = 10);
	int FirstGoodCommand(std::deque<int>& vCommands, const CUserCmd* pCmd);
	bool IsCritCommand(int iSlot, int iIndex, const i32 command_number, const bool bCrit = true, const bool bSafe = true);
	u32 DecryptOrEncryptSeed(int iSlot, int iIndex, const u3
