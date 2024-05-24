#pragma once
#include "../../Feature.h"

#include "../AimbotGlobal/AimbotGlobal.h"

class CAimbotMelee
{
	std::vector<Target_t> GetTargets(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon);
	bool AimFriendlyBuilding(CBaseEntity* pBuilding);
	std::vector<Target_t> SortTargets(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon);

	int GetSwingTime(CBaseCombatWeapon* pWeapon);
	void SimulatePlayers(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, std::vector<Target_t> targets, Vec3& vEyePos,
						 std::unordered_map<CBaseEntity*, std::deque<TickRecord>>& pRecordMap,
						 std::unordered_map<CBaseEntity*, std::deque<std::pair<Vec3, Vec3>>>& simLines);
	bool CanBackstab(CBaseEntity* pTarget, CBaseEntity* pLocal, Vec3 eyeAngles);
	int CanHit(Target_t& target, CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, Vec3 vEyePos, std::deque<TickRecord> newRecords);
	
	bool IsAttacking(const CUserCmd* pCmd, CBaseCombatWeapon* pWeapon);
	void Aim(CUserCmd* pCmd, Vec3& vAngle);
	Vec3 Aim(Vec3 vCurAngle, Vec3 vToAngle, int iMethod = Vars::Aimbot::General::AimType.Value);

	bool FindNearestBuildPoint(CBaseEntity* pBuilding, CBaseEntity* pLocal, Vec3& vPoint);
	bool RunSapper(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd);

	Target_t lockedTarget;
	int iAimType = 0;
	int iDoubletapTicks = 0;
	int iDoubletapMax = 0;
public:
	void Run(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd);
};

ADD_FEATURE(CAimbotMelee, AimbotMelee)