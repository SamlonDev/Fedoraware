#pragma once
#include "../Feature.h"

#pragma warning ( disable : 4091 )

class CIncomingSequence
{
public:
	int InReliableState;
	int SequenceNr;
	float CurTime;

	CIncomingSequence(int inState, int seqNr, float time)
	{
		InReliableState = inState;
		SequenceNr = seqNr;
		CurTime = time;
	}
};

using BoneMatrixes = struct
{
	float BoneMatrix[128][3][4];
};

struct TickRecord
{
	float flSimTime = 0.f;
	float flCreateTime = 0.f;
	int iTickCount = 0;
	bool bOnShot = false;
	BoneMatrixes BoneMatrix{};
	Vec3 vOrigin = {};
	Vec3 vCenter = {}; // cham / glow optimization
	bool bInvalid = false;
};

class CBacktrack
{
	// logic
	bool WithinRewind(const TickRecord& record);

	// utils
	void SendLerp();
	void UpdateDatagram();
	void StoreNolerp();
	void MakeRecords();
	void CleanRecords();
	std::optional<TickRecord> GetHitRecord(CUserCmd* pCmd, CBaseEntity* pEntity, Vec3 vAngles, Vec3 vPos);

	// data
	std::unordered_map<int, bool> mDidShoot;
	int iLastCreationTick = 0;

	// data - fake latency
	std::deque<CIncomingSequence> dSequences;
	int iLastInSequence = 0;

	bool bLastTickHeld = false;

public:
	float GetLerp();
	float GetFake();
	float GetReal(int iFlow = 0);

	std::deque<TickRecord>* GetRecords(CBaseEntity* pEntity);
	std::deque<TickRecord> GetValidRecords(std::deque<TickRecord>* pRecords, CBaseEntity* pLocal = nullptr, bool bDistance = false);

	void Restart();
	void FrameStageNotify();
	void Run(CUserCmd* pCmd);
	void SetLerp(CGameEvent* pEvent);
	void ResolverUpdate(CBaseEntity* pEntity);
	void ReportShot(int iIndex);
	void AdjustPing(INetChannel* netChannel);

	bool bFakeLatency = false;
	float flWishInterp = G::LerpTime;
	float flFakeInterp = G::LerpTime;
	std::unordered_map<CBaseEntity*, std::deque<TickRecord>> mRecords;
	std::unordered_map<CBaseEntity*, std::pair<int, matrix3x4[128]>> mBones;
	std::unordered_map<CBaseEntity*, Vec3> mEyeAngles;
	std::unordered_map<CBaseEntity*, bool> mLagCompensation;

	bool bSettingUpBones = false;

	int iTickCount = 0;
	float flMaxUnlag = 1.f;
};

ADD_FEATURE(CBacktrack, Backtrack)