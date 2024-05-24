#include "Radar.h"

#include "../../Vars.h"
#include "../../Color.h"

bool CRadar::GetDrawPosition(CBaseEntity* pLocal, CBaseEntity* pEntity, int& x, int& y, int& z)
{
	const float flRange = Vars::Radar::Main::Range.Value;
	const float flYaw = I::EngineClient->GetViewAngles().y * (PI / 180.f);
	const float flCos = cosf(flYaw), flSin = sinf(flYaw);

	const Vec3 vDelta = pEntity->GetAbsOrigin() - pLocal->GetAbsOrigin();
	auto vPos = Vec2((vDelta.y * (-flCos) + vDelta.x * flSin), (vDelta.x * (-flCos) - vDelta.y * flSin));
	z = vDelta.z;

	switch (Vars::Radar::Main::Style.Value)
	{
	case 0:
	{
		const float flDist = vDelta.Length2D();
		if (flDist > flRange)
		{
			if (!Vars::Radar::Main::AlwaysDraw.Value)
				return false;

			vPos *= flRange / flDist;
		}
		break;
	}
	case 1:
		if (fabs(vPos.x) > flRange || fabs(vPos.y) > flRange)
		{
			if (!Vars::Radar::Main::AlwaysDraw.Value)
				return false;

			if (vPos.y > vPos.x)
			{
				if (vPos.y > -vPos.x)
					vPos.x = flRange * vPos.x / vPos.y, vPos.y = flRange;
				else
					vPos.y = -flRange * vPos.y / vPos.x, vPos.x = -flRange;
			}
			else
			{
				if (vPos.y > -vPos.x)
					vPos.y = flRange * vPos.y / vPos.x, vPos.x = flRange;
				else
					vPos.x = -flRange * vPos.x / vPos.y, vPos.y = -flRange;
			}
		}
	}

	const WindowBox_t& info = Vars::Radar::Main::Window.Value;
	x = info.x + vPos.x / flRange * info.w / 2 + float(info.w) / 2;
	y = info.y + vPos.y / flRange * info.w / 2 + float(info.w) / 2;

	return true;
}

void CRadar::DrawBackground()
{
	const WindowBox_t& info = Vars::Radar::Main::Window.Value;
	const auto& themeBack = Vars::Menu::Theme::Background.Value;
	const auto& themeAccent = Vars::Menu::Theme::Accent.Value;
	const Color_t backgroundColor = { themeBack.r, themeBack.g, themeBack.b, static_cast<byte>(Vars::Radar::Main::BackAlpha.Value) };
	const Color_t accentColor = { themeAccent.r, themeAccent.g, themeAccent.b, static_cast<byte>(Vars::Radar::Main::LineAlpha.Value) };

	switch (Vars::Radar::Main::Style.Value)
	{
	case 0:
	{
		const float flRadius = float(info.w) / 2;
		g_Draw.FillCircle(info.x + flRadius, info.y + flRadius, flRadius, 100, backgroundColor);
		g_Draw.LineCircle(info.x + flRadius, info.y + flRadius, flRadius, 100, accentColor);
		break;
	}
	case 1:
		g_Draw.FillRect(info.x, info.y, info.w, info.w, backgroundColor);
		g_Draw.LineRect(info.x, info.y, info.w, info.w, accentColor);
	}

	g_Draw.Line(info.x + info.w / 2, info.y, info.x + info.w / 2, info.y + info.w, accentColor);
	g_Draw.Line(info.x, info.y + info.w / 2, info.x + info.w, info.y + info.w / 2, accentColor);
}

void CRadar::DrawPoints()
{
	const auto& pLocal = g_EntityCache.GetLocal();
	if (!pLocal)
		return;

	// Ammo & Health
	if (Vars::Radar::World::Active.Value)
	{
		const int iSize = Vars::Radar::World::IconSize.Value;

		if (Vars::Radar::World::Draw.Value & 1 << 4)
		{
			for (const auto& pBook : g_EntityCache.GetGroup(EGroupType::WORLD_GARGOYLE))
			{
				int x, y, z;
				if (GetDrawPosition(pLocal, pBook, x, y, z))
				{
					if (Vars::Radar::World::Background.Value)
					{
						const float flRadius = sqrtf(pow(iSize, 2) * 2) / 2;
						g_Draw.FillCircle(x, y, flRadius, 20, Vars::Colors::Halloween.Value);
					}

					g_Draw.Texture(x, y, iSize, iSize, 38);
				}
			}
			for (const auto& pBook : g_EntityCache.GetGroup(EGroupType::WORLD_SPELLBOOK))
			{
				int x, y, z;
				if (GetDrawPosition(pLocal, pBook, x, y, z))
				{
					if (Vars::Radar::World::Background.Value)
					{
						const float flRadius = sqrtf(pow(iSize, 2) * 2) / 2;
						g_Draw.FillCircle(x, y, flRadius, 20, Vars::Colors::Halloween.Value);
					}

					g_Draw.Texture(x, y, iSize, iSize, 37);
				}
			}
		}

		if (Vars::Radar::World::Draw.Value & 1 << 3)
		{
			for (const auto& bBomb : g_EntityCache.GetGroup(EGroupType::WORLD_BOMBS))
			{
				int x, y, z;
				if (GetDrawPosition(pLocal, bBomb, x, y, z))
				{
					if (Vars::Radar::World::Background.Value)
					{
						const float flRadius = sqrtf(pow(iSize, 2) * 2) / 2;
						g_Draw.FillCircle(x, y, flRadius, 20, Vars::Colors::Bomb.Value);
					}

					g_Draw.Texture(x, y, iSize, iSize, 36);
				}
			}
		}

		if (Vars::Radar::World::Draw.Value & 1 << 2)
		{
			for (const auto& pBook : g_EntityCache.GetGroup(EGroupType::WORLD_MONEY))
			{
				int x, y, z;
				if (GetDrawPosition(pLocal, pBook, x, y, z))
				{
					if (Vars::Radar::World::Background.Value)
					{
						const float flRadius = sqrtf(pow(iSize, 2) * 2) / 2;
						g_Draw.FillCircle(x, y, flRadius, 20, Vars::Colors::Money.Value);
					}

					g_Draw.Texture(x, y, iSize, iSize, 35);
				}
			}
		}

		if (Vars::Radar::World::Draw.Value & 1 << 1)
		{
			for (const auto& pAmmo : g_EntityCache.GetGroup(EGroupType::WORLD_AMMO))
			{
				int x, y, z;
				if (GetDrawPosition(pLocal, pAmmo, x, y, z))
				{
					if (Vars::Radar::World::Background.Value)
					{
						const float flRadius = sqrtf(pow(iSize, 2) * 2) / 2;
						g_Draw.FillCircle(x, y, flRadius, 20, Vars::Colors::Ammo.Value);
					}

					g_Draw.Texture(x, y, iSize, iSize, 34);
				}
			}
		}

		if (Vars::Radar::World::Draw.Value & 1 << 0)
		{
			for (const auto& pHealth : g_EntityCache.GetGroup(EGroupType::WORLD_HEALTH))
			{
				int x, y, z;
				if (GetDrawPosition(pLocal, pHealth, x, y, z))
				{
					if (Vars::Radar::World::Background.Value)
					{
						const float flRadius = sqrtf(pow(iSize, 2) * 2) / 2;
						g_Draw.FillCircle(x, y, flRadius, 20, Vars::Colors::Health.Value);
					}

					g_Draw.Texture(x, y, iSize, iSize, 33);
				}
			}
		}
	}

	// Draw buildings
	if (Vars::Radar::Buildings::Active.Value)
	{
		const int iSize = Vars::Radar::Buildings::IconSize.Value;

		for (const auto& pBuilding : g_EntityCache.GetGroup(EGroupType::BUILDINGS_ALL))
		{
			if (!pBuilding->IsAlive())
				continue;

			if (!pBuilding->m_bWasMapPlaced())
			{
				const auto& pOwner = I::ClientEntityList->GetClientEntityFromHandle(pBuilding->m_hBuilder());
				if (pOwner)
				{
					const int nIndex = pOwner->GetIndex();
					if (nIndex != I::EngineClient->GetLocalPlayer() && pOwner != g_EntityCache.GetObservedTarget())
					{
						if (!(Vars::Radar::Buildings::Draw.Value & 1 << 3 && g_EntityCache.IsFriend(nIndex)))
						{
							if (!(Vars::Radar::Buildings::Draw.Value & 1 << 1) && pOwner->m_iTeamNum() != pLocal->m_iTeamNum())
								continue;
							if (!(Vars::Radar::Buildings::Draw.Value & 1 << 2) && pOwner->m_iTeamNum() == pLocal->m_iTeamNum())
								continue;
						}
					}
					else if (!(Vars::Radar::Buildings::Draw.Value & 1 << 0))
						continue;
				}
			}

			int x, y, z;
			if (GetDrawPosition(pLocal, pBuilding, x, y, z))
			{
				const Color_t drawColor = GetEntityDrawColor(pBuilding, Vars::Colors::Relative.Value);

				int iBounds = iSize;
				if (Vars::Radar::Buildings::Background.Value)
				{
					const float flRadius = sqrtf(pow(iSize, 2) * 2) / 2;
					g_Draw.FillCircle(x, y, flRadius, 20, drawColor);
					iBounds = flRadius * 2;
				}

				switch (pBuilding->GetClassID())
				{
				case ETFClassID::CObjectSentrygun:
					g_Draw.Texture(x, y, iSize, iSize, 26 + pBuilding->m_iUpgradeLevel());
					break;
				case ETFClassID::CObjectDispenser:
					g_Draw.Texture(x, y, iSize, iSize, 30);
					break;
				case ETFClassID::CObjectTeleporter:
					g_Draw.Texture(x, y, iSize, iSize, pBuilding->m_iObjectMode() ? 32 : 31);
					break;
				}

				if (Vars::Radar::Buildings::Health.Value)
				{
					const int iMaxHealth = pBuilding->m_iMaxHealth(), iHealth = pBuilding->m_iBOHealth();

					float flRatio = std::clamp(float(iHealth) / iMaxHealth, 0.f, 1.f);
					Color_t cColor = Vars::Colors::HealthBar.Value.StartColor.lerp(Vars::Colors::HealthBar.Value.EndColor, flRatio);
					g_Draw.FillRectPercent(x - iBounds / 2, y - iBounds / 2, 2, iBounds, flRatio, cColor, { 0, 0, 0, 255 }, ALIGN_BOTTOM, true);

					if (iHealth > iMaxHealth)
					{
						const float flMaxOverheal = floorf(iMaxHealth / 10.f) * 5;
						flRatio = std::clamp((iHealth - iMaxHealth) / flMaxOverheal, 0.f, 1.f);
						cColor = Vars::Colors::Overheal.Value;
						g_Draw.FillRectPercent(x - iBounds / 2, y - iBounds / 2, 2, iBounds, flRatio, cColor, { 0, 0, 0, 0 }, ALIGN_BOTTOM, true);
					}
				}
			}
		}
	}

	// Draw Players
	if (Vars::Radar::Players::Active.Value)
	{
		const int iSize = Vars::Radar::Players::IconSize.Value;

		for (const auto& pPlayer : g_EntityCache.GetGroup(EGroupType::PLAYERS_ALL))
		{
			if (!pPlayer->IsAlive() || pPlayer->IsAGhost())
				continue;

			const int nIndex = pPlayer->GetIndex();
			if (nIndex != I::EngineClient->GetLocalPlayer() && pPlayer != g_EntityCache.GetObservedTarget())
			{
				if (!(Vars::Radar::Players::Draw.Value & 1 << 3 && g_EntityCache.IsFriend(nIndex)))
				{
					if (!(Vars::Radar::Players::Draw.Value & 1 << 1) && pPlayer->m_iTeamNum() != pLocal->m_iTeamNum())
						continue;
					if (!(Vars::Radar::Players::Draw.Value & 1 << 2) && pPlayer->m_iTeamNum() == pLocal->m_iTeamNum())
						continue;
				}
			}
			else if (!(Vars::Radar::Players::Draw.Value & 1 << 0))
				continue;
			if (!(Vars::Radar::Players::Draw.Value & 1 << 4) && pPlayer->m_flInvisibility() >= 1.f)
				continue;

			int x, y, z;
			if (GetDrawPosition(pLocal, pPlayer, x, y, z))
			{
				const Color_t drawColor = GetEntityDrawColor(pPlayer, Vars::Colors::Relative.Value);

				int iBounds = iSize;
				if (Vars::Radar::Players::Background.Value)
				{
					const float flRadius = sqrtf(pow(iSize, 2) * 2) / 2;
					g_Draw.FillCircle(x, y, flRadius, 20, drawColor);
					iBounds = flRadius * 2;
				}

				switch (Vars::Radar::Players::IconType.Value)
				{
				case 2:
				{
					PlayerInfo_t pi{};
					if (I::EngineClient->GetPlayerInfo(pPlayer->GetIndex(), &pi) && !pi.fakeplayer)
					{
						g_Draw.Avatar(x, y, iSize, iSize, pi.friendsID);
						break;
					}
					[[fallthrough]];
				}
				case 1:
					if (pPlayer->IsInValidTeam())
					{
						g_Draw.Texture(x, y, iSize, iSize, pPlayer->m_iClass() + (pPlayer->m_iTeamNum() == TEAM_RED ? 9 : 18) - 1);
						break;
					}
					[[fallthrough]];
				case 0:
					g_Draw.Texture(x, y, iSize, iSize, pPlayer->m_iClass() - 1);
					break;
				}

				if (Vars::Radar::Players::Health.Value)
				{
					const int iMaxHealth = pPlayer->GetMaxHealth(), iHealth = pPlayer->m_iHealth();

					float flRatio = std::clamp(float(iHealth) / iMaxHealth, 0.f, 1.f);
					Color_t cColor = Vars::Colors::HealthBar.Value.StartColor.lerp(Vars::Colors::HealthBar.Value.EndColor, flRatio);
					g_Draw.FillRectPercent(x - iBounds / 2, y - iBounds / 2, 2, iBounds, flRatio, cColor, { 0, 0, 0, 255 }, ALIGN_BOTTOM, true);

					if (iHealth > iMaxHealth)
					{
						const float flMaxOverheal = floorf(iMaxHealth / 10.f) * 5;
						flRatio = std::clamp((iHealth - iMaxHealth) / flMaxOverheal, 0.f, 1.f);
						cColor = Vars::Colors::Overheal.Value;
						g_Draw.FillRectPercent(x - iBounds / 2, y - iBounds / 2, 2, iBounds, flRatio, cColor, { 0, 0, 0, 0 }, ALIGN_BOTTOM, true);
					}
				}

				if (Vars::Radar::Players::Height.Value && std::abs(z) > 80.f)
				{
					const int m = x - iSize / 2;
					const int iOffset = z > 0 ? -5 : 5;
					const int yPos = z > 0 ? y - iBounds / 2 - 2 : y + iBounds / 2 + 2;

					g_Draw.DrawFillTriangle({ Vec2(m, yPos), Vec2(m + iSize * 0.5f, yPos + iOffset), Vec2(m + iSize, yPos) }, drawColor);
				}
			}
		}
	}
}

void CRadar::Run()
{
	if (!Vars::Radar::Main::Active.Value)
		return;

	DrawBackground();
	DrawPoints();
}