#ifndef BACKTRACK_H
#define BACKTRACK_H

#pragma once
#include "../Feature.h"

#pragma warning ( disable : 4091 )

class CIncomingSequence
{
public:
	int InReliableState;
	int SequenceNr;
	float CurTime;

	CIncomingSequence(int inState, int seqNr, float time) noexcept
		: InReliableState(inState), SequenceNr(seqNr), CurTime(time) {}
};

using BoneMatrixes = std::array<std::array<float, 3U[4U]>, 128U>;

class CBacktrack : public Feature<CBacktrack>
{
public:
	bool WithinRewind(const TickRecord& record) const noexcept override;

	void SendLerp() noexcept;
	void UpdateDatagram() noexcept;
	void StoreNolerp() noexcept;
	void MakeRecords() noexcept;
	void CleanRecords() noexcept;
	std::optional<TickRecord> GetHitRecord(CUserCmd* pCmd, CBaseEntity* pEntity, Vec3 vAngles, Vec3 vPos) const noexcept;

private:
	std::unordered_map<int, bool> mDidShoot;
	int iLastCreationTick = 0;

	std::deque<CIncomingSequence> dSequences;
	int iLastInSequence = 0;

	bool bLastTickHeld = false;

public:
	float GetLerp() const noexcept;
	float GetFake() const noexcept;
	float GetReal(int iFlow = 0) const noexcept;

	std::deque<TickRecord>* GetRecords(CBaseEntity* pEntity) const noexcept;
	std::deque<TickRecord> GetValidRecords(std::deque<TickRecord>* pRecords, CBaseEntity* pLocal = nullptr, bool bDistance = false) const noexcept;

	void Restart() noexcept;
	void FrameStageNotify() noexcept;
	void Run(CUserCmd* pCmd) noexcept;
	void SetLerp(CGameEvent* pEvent) noexcept;
	void ResolverUpdate(CBaseEntity* pEntity) noexcept;
	void ReportShot(int iIndex) noexcept;
	void AdjustPing(INetChannel* netChannel) noexcept;

	bool bFakeLatency = false;
	constexpr float flWishInterp = G::LerpTime;
	constexpr float flFakeInterp = G::LerpTime;
	std::unordered_map<CBaseEntity*, std::deque<TickRecord>> mRecords;
	std::unordered_map<CBaseEntity*, std::pair<int, BoneMatrixes>> mBones;
	std::unordered_map<CBaseEntity*, Vec3> mEyeAngles;
	std::unordered_map<CBaseEntity*, bool> mLagCompensation;

	bool bSettingUpBones = false;

	int iTickCount = 0;
	float flMaxUnlag = 1.f;
};

ADD_FEATURE(CBacktrack, Backtrack)

#endif // BACKTRACK_H
