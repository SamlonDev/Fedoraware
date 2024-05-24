#include "EntityCache.h"

#include "../GlobalInfo/GlobalInfo.h"
#include "../../../Features/Visuals/ESP/ESP.h"

void CEntityCache::Fill()
{
	UpdateFriends();

	for (int n = I::EngineClient->GetMaxClients() + 1; n < I::ClientEntityList->GetHighestEntityIndex(); n++)
	{
		CBaseEntity* pEntity = I::ClientEntityList->GetClientEntity(n);
		if (!pEntity || pEntity->GetClassID() != ETFClassID::CTFPlayerResource)
			continue;

		m_pPlayerResource = reinterpret_cast<CTFPlayerResource*>(pEntity);
	}

	CBaseEntity* pLocal = I::ClientEntityList->GetClientEntity(I::EngineClient->GetLocalPlayer());
	if (!pLocal /*|| !pLocal->IsInValidTeam()*/)
		return;

	m_pLocal = pLocal;
	m_pLocalWeapon = m_pLocal->GetActiveWeapon();

	switch (m_pLocal->m_iObserverMode())
	{
		case OBS_MODE_FIRSTPERSON:
		case OBS_MODE_THIRDPERSON:
		{
			m_pObservedTarget = I::ClientEntityList->GetClientEntityFromHandle(m_pLocal->m_hObserverTarget());
			break;
		}
		default: break;
	}

	for (int n = 1; n < I::ClientEntityList->GetHighestEntityIndex(); n++)
	{
		CBaseEntity* pEntity = I::ClientEntityList->GetClientEntity(n);
		if (!pEntity)
			continue;

		const auto nClassID = pEntity->GetClassID();

		if (pEntity->GetDormant())
		{
			if (nClassID != ETFClassID::CTFPlayer)
				continue;

			// Is any dormant data available?
			if (!G::DormantPlayerESP.count(n))
				continue;

			const auto& dormantData = G::DormantPlayerESP[n];
			const float lastUpdate = dormantData.LastUpdate;

			if (I::EngineClient->Time() - lastUpdate > Vars::ESP::DormantTime.Value)
				continue;

			pEntity->SetAbsOrigin(dormantData.Location);
			pEntity->m_vecOrigin() = dormantData.Location;

			pEntity->m_lifeState() = LIFE_ALIVE;
			const auto& playerResource = GetPR();
			if (playerResource && playerResource->GetValid(n))
				pEntity->m_iHealth() = playerResource->GetHealth(n);
		}

		switch (nClassID)
		{
			case ETFClassID::CTFPlayer:
			{
				m_vecGroups[EGroupType::PLAYERS_ALL].push_back(pEntity);
				m_vecGroups[pEntity->m_iTeamNum() != m_pLocal->m_iTeamNum() ? EGroupType::PLAYERS_ENEMIES : EGroupType::PLAYERS_TEAMMATES].push_back(pEntity);
				break;
			}
			case ETFClassID::CObjectSentrygun:
			case ETFClassID::CObjectDispenser:
			case ETFClassID::CObjectTeleporter:
			{
				m_vecGroups[EGroupType::BUILDINGS_ALL].push_back(pEntity);
				m_vecGroups[pEntity->m_iTeamNum() != m_pLocal->m_iTeamNum() ? EGroupType::BUILDINGS_ENEMIES : EGroupType::BUILDINGS_TEAMMATES].push_back(pEntity);
				break;
			}
			case ETFClassID::CBaseAnimating:
			{
				const auto szName = pEntity->GetModelName();
				if (Hash::IsAmmo(szName))
				{
					m_vecGroups[EGroupType::WORLD_AMMO].push_back(pEntity);
					break;
				}
				if (Hash::IsHealth(szName))
				{
					m_vecGroups[EGroupType::WORLD_HEALTH].push_back(pEntity);
					break;
				}
				if (Hash::IsSpell(szName))
				{
					m_vecGroups[EGroupType::WORLD_SPELLBOOK].push_back(pEntity);
					break;
				}
				break;
			}
			case ETFClassID::CTFAmmoPack:
			{
				m_vecGroups[EGroupType::WORLD_AMMO].push_back(pEntity);
				break;
			}
			case ETFClassID::CTFProjectile_Rocket:
			case ETFClassID::CTFGrenadePipebombProjectile:
			case ETFClassID::CTFProjectile_Jar:
			case ETFClassID::CTFProjectile_JarGas:
			case ETFClassID::CTFProjectile_JarMilk:
			case ETFClassID::CTFProjectile_Arrow:
			case ETFClassID::CTFProjectile_SentryRocket:
			case ETFClassID::CTFProjectile_Flare:
			case ETFClassID::CTFProjectile_GrapplingHook:
			case ETFClassID::CTFProjectile_Cleaver:
			case ETFClassID::CTFProjectile_EnergyBall:
			case ETFClassID::CTFProjectile_EnergyRing:
			case ETFClassID::CTFProjectile_HealingBolt:
			case ETFClassID::CTFProjectile_ThrowableBreadMonster:
			case ETFClassID::CTFStunBall:
			case ETFClassID::CTFBall_Ornament:
			{
				m_vecGroups[EGroupType::WORLD_PROJECTILES].push_back(pEntity);

				if (nClassID == ETFClassID::CTFGrenadePipebombProjectile && (pEntity->m_iType() == TF_GL_MODE_REMOTE_DETONATE_PRACTICE || pEntity->m_bPulsed()))
				{
					CBaseEntity* pThrower = I::ClientEntityList->GetClientEntityFromHandle(pEntity->m_hThrower());
					CBaseEntity* pOwner = I::ClientEntityList->GetClientEntityFromHandle(pEntity->m_hOwnerEntity());
					if (pThrower == m_pLocal || pOwner == m_pLocal)
						m_vecGroups[EGroupType::LOCAL_STICKIES].push_back(pEntity);
#ifdef DEBUG
					if (Vars::Debug::DebugInfo.Value)
					{
						Utils::ConLog("EntityCache", std::format("\npEntity : {}\npLocal : {}\n\n", pEntity, m_pLocal).c_str(), { 104, 235, 255, 255 });
						if (!pOwner || !pThrower) { break; }
						if (pThrower == m_pLocal || pOwner == m_pLocal) { break; }
						Utils::ConLog("EntityCache", std::format("    \npLocal : {}\npLocalWeapon : {}\npThrower : {}\npOwner : {}\n\n", m_pLocal, m_pLocalWeapon, pThrower, pOwner).c_str(), { 104, 235, 255, 255 });
					}
#endif
					break;
				}

				if (nClassID == ETFClassID::CTFProjectile_Flare)
				{
					if (const auto& pSecondary = m_pLocal->GetWeaponFromSlot(EWeaponSlots::SLOT_SECONDARY))
					{
						if (pSecondary->m_iItemDefinitionIndex() == ETFWeapons::Pyro_s_TheDetonator)
						{
							if (I::ClientEntityList->GetClientEntityFromHandle(pEntity->m_hOwnerEntity()) == m_pLocal)
								m_vecGroups[EGroupType::LOCAL_FLARES].push_back(pEntity);
						}
					}

					break;
				}

				break;
			}
			case ETFClassID::CHeadlessHatman:
			case ETFClassID::CTFTankBoss:
			case ETFClassID::CMerasmus:
			case ETFClassID::CZombie:
			case ETFClassID::CEyeballBoss:
			{
				m_vecGroups[EGroupType::WORLD_NPC].push_back(pEntity);
				break;
			}
			case ETFClassID::CTFPumpkinBomb:
			case ETFClassID::CTFGenericBomb:
			{
				m_vecGroups[EGroupType::WORLD_BOMBS].push_back(pEntity);
				break;
			}
			case ETFClassID::CCurrencyPack:
			{
				m_vecGroups[EGroupType::WORLD_MONEY].push_back(pEntity);
				break;
			}
			case ETFClassID::CHalloweenGiftPickup:
			{
				if (I::ClientEntityList->GetClientEntityFromHandle(pEntity->m_hTargetPlayer()) == m_pLocal)
					m_vecGroups[EGroupType::WORLD_GARGOYLE].push_back(pEntity);
				break;
			}
		}
	}
}

void CEntityCache::UpdateFriends()
{
	// Check friendship for every player
	m_Friends.reset();
	for (int n = 1; n <= I::EngineClient->GetMaxClients(); n++)
	{
		PlayerInfo_t pi{};
		if (!I::EngineClient->GetPlayerInfo(n, &pi))
			continue;

		m_Friends[n] = Utils::IsSteamFriend(pi.friendsID);
	}
}

void CEntityCache::Clear()
{
	m_pLocal = nullptr;
	m_pLocalWeapon = nullptr;
	m_pObservedTarget = nullptr;
	m_pPlayerResource = nullptr;

	for (auto& Group : m_vecGroups)
		Group.second.clear();
}

const std::vector<CBaseEntity*>& CEntityCache::GetGroup(const EGroupType& Group)
{
	return m_vecGroups[Group];
}
