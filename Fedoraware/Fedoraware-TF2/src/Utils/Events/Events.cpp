#include "Events.h"
#include "../../Features/Logs/Logs.h"
#include "../../Features/Resolver/Resolver.h"
#include "../../Features/PacketManip/AntiAim/AntiAim.h"
#include "../../Features/Visuals/Visuals.h"
#include "../../Features/CritHack/CritHack.h"
#include "../../Features/Backtrack/Backtrack.h"
#include "../../Features/Misc/Misc.h"

void CEventListener::Setup(const std::deque<const char*>& deqEvents)
{
	if (deqEvents.empty())
		return;

	for (auto szEvent : deqEvents)
	{
		I::GameEventManager->AddListener(this, szEvent, false);

		if (!I::GameEventManager->FindListener(this, szEvent))
			throw std::runtime_error(std::format("failed to add listener: {}", szEvent));
	}
}

void CEventListener::Destroy()
{
	I::GameEventManager->RemoveListener(this);
}

void CEventListener::FireGameEvent(CGameEvent* pEvent)
{
	if (!pEvent || I::EngineClient->IsPlayingTimeDemo())
		return;

	const FNV1A_t uNameHash = FNV1A::Hash(pEvent->GetName());
	F::Logs.Event(pEvent, uNameHash);
	F::CritHack.Event(pEvent, uNameHash);
	F::Misc.Event(pEvent, uNameHash);

	if (uNameHash == FNV1A::HashConst("player_hurt"))
	{
		F::Resolver.OnPlayerHurt(pEvent);
	}

	if (uNameHash == FNV1A::HashConst("player_spawn"))
		F::Backtrack.SetLerp(pEvent);

	if (Vars::Visuals::UI::PickupTimers.Value && uNameHash == FNV1A::HashConst("item_pickup"))
	{
		const auto itemName = pEvent->GetString("item");
		if (const auto& pEntity = I::ClientEntityList->GetClientEntity(I::EngineClient->GetPlayerForUserID(pEvent->GetInt("userid"))))
		{
			if (std::strstr(itemName, "medkit"))
				F::Visuals.PickupDatas.push_back({ 1, I::EngineClient->Time(), pEntity->GetAbsOrigin() });
			else if (std::strstr(itemName, "ammopack"))
				F::Visuals.PickupDatas.push_back({ 0, I::EngineClient->Time(), pEntity->GetAbsOrigin() });
		}
	}

	if (Vars::Misc::MannVsMachine::InstantRevive.Value && uNameHash == FNV1A::HashConst("revive_player_notify"))
	{
		if (pEvent->GetInt("entindex") == I::EngineClient->GetLocalPlayer())
		{
			auto kv = new KeyValues("MVM_Revive_Response");
			kv->SetInt("accepted", 1);
			I::EngineClient->ServerCmdKeyValues(kv);
		}
	}
}