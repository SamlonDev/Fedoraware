#pragma once
#include "Vars.h"
#include "Menu/Playerlist/PlayerUtils.h"

__inline Color_t GetTeamColor(int iTeam, bool bOther)
{
	if (bOther)
	{
		if (const auto& pLocal = g_EntityCache.GetLocal())
			return pLocal->m_iTeamNum() == iTeam ? Vars::Colors::Team.Value : Vars::Colors::Enemy.Value;
	}
	else {
		switch (iTeam)
		{
		case 2: return Vars::Colors::TeamRed.Value;
		case 3: return Vars::Colors::TeamBlu.Value;
		}
	}

	return { 255, 255, 255, 255 };
}

__inline Color_t GetEntityDrawColor(CBaseEntity* pEntity, bool enableOtherColors)
{
	Color_t out = GetTeamColor(pEntity->m_iTeamNum(), enableOtherColors);

	if (pEntity->IsPlayer())
	{
		PlayerInfo_t pi{}; bool bTagColor = false; Color_t cTagColor;
		if (I::EngineClient->GetPlayerInfo(pEntity->GetIndex(), &pi))
		{
			std::string _; PriorityLabel_t plTag;
			if (bTagColor = F::PlayerUtils.GetSignificantTag(pi.friendsID, &_, &plTag))
				cTagColor = plTag.Color;
		}

		if (g_EntityCache.GetLocal()->GetIndex() == pEntity->GetIndex())
			out = Vars::Colors::Local.Value;
		else if (g_EntityCache.IsFriend(pEntity->GetIndex()))
			out = F::PlayerUtils.mTags["Friend"].Color;
		else if (bTagColor)
			out = cTagColor;
		else if (pEntity->IsCloaked())
			out = Vars::Colors::Cloak.Value;
		else if (pEntity->IsInvulnerable())
			out = Vars::Colors::Invulnerable.Value;
	}

	if (pEntity->GetIndex() == G::CurrentTarget.first && abs(G::CurrentTarget.second - I::GlobalVars->tickcount) < 32)
		out = Vars::Colors::Target.Value;

	return out;
}