#include "SpectatorList.h"

#include "../../Vars.h"

bool CSpectatorList::GetSpectators(CBaseEntity* pLocal)
{
	G::FirstSpectated = G::ThirdSpectated = false;

	Spectators.clear();

	for (const auto& pTeammate : g_EntityCache.GetGroup(EGroupType::PLAYERS_TEAMMATES))
	{
		CBaseEntity* pObservedPlayer = I::ClientEntityList->GetClientEntityFromHandle(pTeammate->m_hObserverTarget());

		if (pTeammate && !pTeammate->IsAlive() && pObservedPlayer == pLocal)
		{
			std::wstring szMode;
			switch (pTeammate->m_iObserverMode())
			{
				case OBS_MODE_FIRSTPERSON:
				{
					szMode = L"1st";
					G::FirstSpectated = true;
					break;
				}
				case OBS_MODE_THIRDPERSON:
				{
					szMode = L"3rd";
					G::ThirdSpectated = true;
					break;
				}
				default: 
					continue;
			}

			int respawnIn = 0; float respawnTime = 0;
			if (CTFPlayerResource* pResource = g_EntityCache.GetPR())
			{
				respawnTime = pResource->GetNextRespawnTime(pTeammate->GetIndex());
				respawnIn = std::max(int(respawnTime - I::GlobalVars->curtime), 0);
			}
			bool respawnTimeIncreased = false; // theoretically the respawn times could be changed by the map but oh well
			if (!RespawnCache.contains(pTeammate->GetIndex()))
				RespawnCache[pTeammate->GetIndex()] = respawnTime;
			if (RespawnCache[pTeammate->GetIndex()] + 0.9f < respawnTime)
			{
				respawnTimeIncreased = true;
				RespawnCache[pTeammate->GetIndex()] = -1.f;
			}

			PlayerInfo_t pi{};
			if (I::EngineClient->GetPlayerInfo(pTeammate->GetIndex(), &pi))
			{
				Spectators.push_back({
					Utils::ConvertUtf8ToWide(pi.name), szMode, respawnIn, respawnTimeIncreased, g_EntityCache.IsFriend(pTeammate->GetIndex()),
					pTeammate->m_iTeamNum(), pTeammate->GetIndex()
				});
			}
		}
		else
		{
			auto iter = RespawnCache.find(pTeammate->GetIndex());
			if (iter != RespawnCache.end())
				RespawnCache.erase(iter);
		}
	}

	return !Spectators.empty();
}

void CSpectatorList::Run()
{
	if (!(Vars::Menu::Indicators.Value & (1 << 2)))
	{
		RespawnCache.clear();
		return;
	}

	if (const auto& pLocal = g_EntityCache.GetLocal())
	{
		if (!pLocal->IsAlive() || !GetSpectators(pLocal))
			return;

		int x = Vars::Menu::SpectatorsDisplay.Value.x;
		int iconOffset = 0;
		int y = Vars::Menu::SpectatorsDisplay.Value.y + 8;

		EAlign align = ALIGN_TOP;
		if (x <= (100 + 50 * Vars::Menu::DPI.Value))
		{
		//	iconOffset = 36;
			x -= 42 * Vars::Menu::DPI.Value;
			align = ALIGN_TOPLEFT;
		}
		else if (x >= g_ScreenSize.w - (100 + 50 * Vars::Menu::DPI.Value))
		{
			x += 42 * Vars::Menu::DPI.Value;
			align = ALIGN_TOPRIGHT;
		}
		//else
		//	iconOffset = 16;

		//if (!Vars::Menu::SpectatorAvatars.Value)
		//	iconOffset = 0;

		const auto& fFont = g_Draw.GetFont(FONT_INDICATORS);

		g_Draw.String(fFont, x, y, Vars::Menu::Theme::Accent.Value, align, L"Spectating You:");
		for (const auto& Spectator : Spectators)
		{
			y += fFont.nTall + 3;

			/*
			if (Vars::Visuals::SpectatorAvatars.Value)
			{
				int w, h;

				I::MatSystemSurface->GetTextSize(g_Draw.GetFont(FONT_MENU).dwFont,
					(Spectator.Name + Spectator.Mode + std::to_wstring(Spectator.RespawnIn) + std::wstring{L" -  (respawn s)"}).c_str(), w, h);
				switch (align)
				{
				case ALIGN_DEFAULT: w = 0; break;
				case ALIGN_CENTERHORIZONTAL: w /= 2; break;
				}

				PlayerInfo_t pi{};
				if (!I::EngineClient->GetPlayerInfo(Spectator.Index, &pi))
					continue;

				g_Draw.Avatar(x - w - (36 - iconOffset), y, 24, 24, pi.friendsID);
				// center - half the width of the string
				y += 6;
			}
			*/

			Color_t color = Vars::Menu::Theme::Active.Value;
			if (Spectator.Mode == std::wstring{L"1st"})
				color = { 255, 200, 127, 255 };
			if (Spectator.RespawnTimeIncreased)
				color = { 255, 100, 100, 255 };
			if (Spectator.IsFriend)
				color = { 200, 255, 200, 255 };
			g_Draw.String(fFont, x + iconOffset, y, color, align,
				L"%ls - %ls (respawn %ds)", Spectator.Name.data(), Spectator.Mode.data(), Spectator.RespawnIn);
		}
	}
}
