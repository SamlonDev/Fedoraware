#pragma once

#include "../../SDK/SDK.h"
#include "../../SDK/Interfaces/GameEvent/GameEvent.h"

class CEventListener : public CGameEventListener
{
public:
	CEventListener();
	~CEventListener();

	void Setup(const std::vector<const char*>& arrEvents);
	void Destroy();

	virtual void FireGameEvent(CGameEvent* pEvent) override;
	virtual int GetEventDebugID() const override { return 42; }

private:
	std::vector<const char*> arrEvents_;
};

inline CEventListener g_Events;

CEventListener::CEventListener()
{
}

CEventListener::~CEventListener()
{
	Destroy();
}

void CEventListener::Setup(const std::vector<const char*>& arrEvents)
{
	arrEvents_ = arrEvents;
}

void CEventListener::Destroy()
{
	arrEvents_.clear();
}

void CEventListener::FireGameEvent(CGameEvent* pEvent)
{
	// Handle game event here
}
