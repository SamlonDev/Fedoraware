#pragma once
#include "../Feature.h"
#include <assert.h>

class CEnginePrediction : public IFeature
{
public:
	CEnginePrediction();
	~CEnginePrediction();

	void Start(CUserCmd* pCmd);
	void End(CUserCmd* pCmd);

private:
	// Gets the tickbase of the specified local player.
	int GetTickbase(CUserCmd* pCmd, CBaseEntity* pLocal);

	// Simulates the specified user command for the local player.
	void Simulate(CUserCmd* pCmd);

	// The previous move data.
	CMoveData m_MoveData;

	// The previous tick count.
	int m_nOldTickCount;

	// The previous current time.
	float m_fOldCurrentTime;

	// The previous frame time.
	float m_fOldFrameTime;

	// Whether the engine has been simulated this frame.
	bool m_bSimulated;

	// Prevents the engine from being simulated multiple times.
	bool m_bSimulatedThisFrame;

private:
	// IFeature implementation.
	virtual const char* GetName() const override;
};

ADD_FEATURE(CEnginePrediction, EnginePrediction)

CEnginePrediction::CEnginePrediction()
	: m_nOldTickCount(0),
	m_fOldCurrentTime(0.f),
	m_fOldFrameTime(0.f),
	m_bSimulated(false),
	m_bSimulatedThisFrame(false)
{
}

CEnginePrediction::~CEnginePrediction()
{
	m_bSimulated = false;
}

int CEnginePrediction::GetTickbase(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	assert(pCmd != nullptr);
	assert(pLocal != nullptr);

	// Implementation here...
}

void CEnginePrediction::Simulate(CUserCmd* pCmd)
{
	if (m_nOldTickCount == 0)
	{
		return;
	}

	// Implementation here...
}

void CEnginePrediction::Start(CUserCmd* pCmd)
{
	if (m_bSimulatedThisFrame)
	{
		return;
	}

	m_nOldTickCount = pCmd->tick_count;
	m_fOldCurrentTime = g_pGlobals->curtime;
	m_fOldFrameTime = g_pGlobals->frametime;

	m_bSimulated = true;
	m_bSimulatedThisFrame = true;

	Simulate(pCmd);
}

void CEnginePrediction::End(CUserCmd* pCmd)
{
	if (!m_bSimulated)
	{
		return;
	}

	m_bSimulatedThisFrame = false;
}

const char* CEnginePrediction::GetName() const
{
	return "EnginePrediction";
}
