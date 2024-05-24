#include "AutoUber.h"

#include "../../Vars.h"
#include "../../Menu/Playerlist/PlayerUtils.h"

// This code is terrible and unoptimized

constexpr static int CHANGE_TIMER = 5; // i am lazy to change code, this should be fine.

int vaccChangeState = 0;
int vaccChangeTicks = 0;
int vaccIdealResist = 0;
int vaccChangeTimer = 0;

int BulletDangerValue(CBaseEntity* pPatient)
{
	bool anyZoomedSnipers = false;
	bool anyEnemies = false;

	// Find dangerous playes in other team
	for (const auto& pPlayer : g_EntityCache.GetGroup(EGroupType::PLAYERS_ENEMIES))
	{
		if (!pPlayer->IsAlive() || pPlayer->IsAGhost())
			continue;

		if (pPlayer->GetDormant())
			continue;

		switch (pPlayer->m_iClass())
		{
			case 1: if (!(Vars::Auto::Uber::ReactClasses.Value & 1 << 0)) continue;
				  break; //	scout
			case 2: if (!(Vars::Auto::Uber::ReactClasses.Value & 1 << 7)) continue;
				  break; //	sniper
			case 3: if (!(Vars::Auto::Uber::ReactClasses.Value & 1 << 1)) continue;
				  break; //	soldier
			case 6: if (!(Vars::Auto::Uber::ReactClasses.Value & 1 << 4)) continue;
				  break; //	heavy
			case 7: if (!(Vars::Auto::Uber::ReactClasses.Value & 1 << 2)) continue;
				  break; //	pyro
			case 8: if (!(Vars::Auto::Uber::ReactClasses.Value & 1 << 8)) continue;
				  break; //	spy
			case 9: if (!(Vars::Auto::Uber::ReactClasses.Value & 1 << 5)) continue;
				  break; //	engineer
			default: { continue; }
		}

		if (pPlayer->InCond(TF_COND_PHASE))
			return false;

		const auto& pWeapon = pPlayer->GetActiveWeapon();

		if (!pWeapon)
			return 0;

		if (pWeapon->GetSlot() == SLOT_MELEE)
			return false;

		if (pWeapon->GetClassID() == ETFClassID::CTFLunchBox || pWeapon->GetClassID() == ETFClassID::CTFLunchBox_Drink || pWeapon->GetClassID() == ETFClassID::CTFWeaponPDA)
			return false;

		// Ignore ignored players
		if (F::AimbotGlobal.ShouldIgnore(pPlayer))
			continue;

		const Vec3 vAngleTo = Math::CalcAngle(pPlayer->GetEyePosition(), pPatient->GetWorldSpaceCenter());
		const float flFOVTo = Math::CalcFov(pPlayer->GetEyeAngles(), vAngleTo);

		if (Vars::Auto::Uber::ReactFOV.Value && !F::PlayerUtils.HasTag(pPlayer->GetIndex(), "Cheater"))
		{
			if (flFOVTo - (3.f * G::ChokeMap[pPlayer->GetIndex()]) > static_cast<float>(Vars::Auto::Uber::ReactFOV.Value))
				continue; // account for choking :D
		}

		if (pPlayer->InCond(TF_COND_AIMING))
		{
			anyZoomedSnipers = true;
			if (Utils::VisPos(pPatient, pPlayer, pPatient->GetHitboxPos(HITBOX_HEAD), pPlayer->GetEyePosition()))
				return 2;
		}


		if (Utils::VisPos(pPatient, pPlayer, pPatient->GetHitboxPos(HITBOX_PELVIS),
			pPlayer->GetEyePosition()))
		{
			if (const auto& pWeapon = pPlayer->GetActiveWeapon())
			{
				if (pPlayer->m_iClass() == CLASS_SPY && pWeapon->GetSlot() == SLOT_PRIMARY || pPlayer->m_iClass() == CLASS_SCOUT || pPlayer->m_iClass() == CLASS_HEAVY || pPlayer->m_iClass() ==
					CLASS_MEDIC || pPlayer->m_iClass() == CLASS_SNIPER || pPlayer->m_iClass() == CLASS_ENGINEER)
				{
					if (pPatient->m_vecOrigin().DistTo(pPlayer->m_vecOrigin()) < 350.f ||
						(pPatient->m_vecOrigin().DistTo(pPlayer->m_vecOrigin()) < 600.f &&
						(pPlayer->m_iClass() == CLASS_SPY || pPlayer->m_iClass() == CLASS_SCOUT || pPlayer->m_iClass() == CLASS_HEAVY || pPlayer->m_iClass() == CLASS_MEDIC || pPlayer->
						m_iClass() == CLASS_SNIPER || pPlayer->m_iClass() == CLASS_ENGINEER)))
					{
						return 2;
					}
				}

				if (pWeapon->GetClassID() == ETFClassID::CTFShotgun_Pyro || pWeapon->GetClassID() == ETFClassID::CTFShotgun_Soldier)
				{
					{
						if (pPatient->m_vecOrigin().DistTo(pPlayer->m_vecOrigin()) < 50.f ||
							(pPatient->m_vecOrigin().DistTo(pPlayer->m_vecOrigin()) < 250.f && (
							(pPlayer->m_iClass() == CLASS_PYRO))))
						{
							return 2;
						}

						if (pPatient->m_vecOrigin().DistTo(pPlayer->m_vecOrigin()) < 50.f ||
							(pPatient->m_vecOrigin().DistTo(pPlayer->m_vecOrigin()) < 250.f && (
							(pPlayer->m_iClass() == CLASS_SOLDIER))))
						{
							return 2;
						}
					}
				}
			}

			anyEnemies = true;
		}
	}

	bool hasHitscan = false;

	for (const auto& pProjectile : g_EntityCache.GetGroup(EGroupType::WORLD_PROJECTILES))
	{
		if (pProjectile->GetVelocity().IsZero())
			continue;

		if (pProjectile->m_iTeamNum() == pPatient->m_iTeamNum())
			continue;

		if (pProjectile->GetClassID() != ETFClassID::CTFProjectile_Arrow &&
			pProjectile->GetClassID() != ETFClassID::CTFProjectile_EnergyBall &&
			pProjectile->GetClassID() != ETFClassID::CTFProjectile_EnergyRing &&
			pProjectile->GetClassID() != ETFClassID::CTFProjectile_Cleaver &&
			pProjectile->GetClassID() != ETFClassID::CTFProjectile_HealingBolt)
		{
			continue;
		}

		const Vec3 vPredicted = pProjectile->m_vecOrigin() + pProjectile->GetVelocity();
		const float flHypPred = sqrtf(pPatient->m_vecOrigin().DistToSqr(vPredicted));
		const float flHyp = sqrtf(pPatient->m_vecOrigin().DistToSqr(pProjectile->m_vecOrigin()));
		if (flHypPred < flHyp && pPatient->m_vecOrigin().DistTo(vPredicted) < pProjectile->GetVelocity().Length())
		{
			if (pProjectile->IsCritBoosted())
				return 2;
			hasHitscan = true;
		}
	}

	if (hasHitscan)
	{
		if (pPatient->m_iHealth() < 449)
			return 2;
	}

	return (anyZoomedSnipers || anyEnemies) ? 1 : 0;
}

int FireDangerValue(CBaseEntity* pPatient)
{
	int shouldSwitch = 0;

	for (const auto& pPlayer : g_EntityCache.GetGroup(EGroupType::PLAYERS_ENEMIES))
	{
		if (!pPlayer->IsAlive())
			continue;

		if (pPlayer->m_iClass() != CLASS_PYRO) // Pyro only
			continue;

		if (pPatient->m_vecOrigin().DistTo(pPlayer->m_vecOrigin()) > 450.f)
			continue;

		const auto& pPlayerWeapon = pPlayer->GetActiveWeapon();

		if (!pPlayerWeapon)
			return 0;

		if (pPlayerWeapon->GetClassID() == ETFClassID::CTFFlameThrower)
		{
			if (pPatient->InCond(TF_COND_BURNING) && pPatient->m_iHealth() < 250)
			{
				if (pPatient->m_iClass() == CLASS_PYRO)
					return 1;
				return 2;
			}

			if (pPlayer->IsPhlogUbered())
				return 2;
			shouldSwitch = 1;
		}
	}

	for (const auto& pProjectile : g_EntityCache.GetGroup(EGroupType::WORLD_PROJECTILES))
	{
		if (pProjectile->m_iTeamNum() == pPatient->m_iTeamNum() || pProjectile->GetVelocity().IsZero())
			continue;

		if (pProjectile->GetClassID() != ETFClassID::CTFProjectile_Flare &&
			pProjectile->GetClassID() != ETFClassID::CTFProjectile_BallOfFire &&
			pProjectile->GetClassID() != ETFClassID::CTFProjectile_SpellFireball)
		{
			continue;
		}

		const Vec3 vPredicted = (pProjectile->m_vecOrigin() + pProjectile->GetVelocity());
		const float flHypPred = sqrtf(pPatient->m_vecOrigin().DistToSqr(vPredicted));
		const float flHyp = sqrtf(pPatient->m_vecOrigin().DistToSqr(pProjectile->m_vecOrigin()));
		if (flHypPred < flHyp && pPatient->m_vecOrigin().DistTo(vPredicted) < pProjectile->GetVelocity().Length())
		{
			if (pProjectile->IsCritBoosted() || pPatient->InCond(TF_COND_BURNING))
				return 2;
			shouldSwitch = 1;
		}
	}

	return shouldSwitch;
}

int BlastDangerValue(CBaseEntity* pPatient)
{
	bool hasRockets = false;

	for (const auto& pProjectile : g_EntityCache.GetGroup(EGroupType::WORLD_PROJECTILES))
	{
		if (hasRockets && !pProjectile->IsCritBoosted())
			continue;

		if (pProjectile->GetVelocity().IsZero())
			continue;

		if (pProjectile->m_bTouched()) // Ignore landed Stickies
			continue;

		if (pProjectile->m_iTeamNum() == pPatient->m_iTeamNum())
			continue;

		if (pProjectile->GetClassID() != ETFClassID::CTFProjectile_Rocket &&
			pProjectile->GetClassID() != ETFClassID::CTFProjectile_SentryRocket &&
			pProjectile->GetClassID() != ETFClassID::CTFGrenadePipebombProjectile)
		{
			continue;
		}

		// Projectile is getting closer
		if (pPatient->m_vecOrigin().DistTo(pProjectile->m_vecOrigin()) <= 275.f)
			hasRockets = true;
	}

	if (hasRockets)
	{
		if (pPatient->m_iHealth() < 235)
			return 2;
		return 1;
	}

	return 0;
}

int CurrentResistance()
{
	if (const auto& pWeapon = g_EntityCache.GetWeapon())
		return pWeapon->m_nChargeResistType();
	return 0;
}

int ChargeCount()
{
	if (const auto& pWeapon = g_EntityCache.GetWeapon())
	{
		if (G::CurItemDefIndex == Medic_s_TheVaccinator)
			return pWeapon->m_flChargeLevel() / 0.25f;
		return pWeapon->m_flChargeLevel() / 1.f;
	}
	return 1;
}

int OptimalResistance(CBaseEntity* pPatient, bool* pShouldPop)
{
	const int bulletDanger = BulletDangerValue(pPatient);
	const int fireDanger = FireDangerValue(pPatient);
	const int blastDanger = BlastDangerValue(pPatient);
	if (pShouldPop)
	{
		int charges = ChargeCount();
		if (bulletDanger > 1 && Vars::Auto::Uber::AutoVaccinator.Value & (1 << 0))
			*pShouldPop = true;
		if (blastDanger > 1 && Vars::Auto::Uber::AutoVaccinator.Value & (1 << 1))
			*pShouldPop = true;
		if (fireDanger > 1 && Vars::Auto::Uber::AutoVaccinator.Value & (1 << 2))
			*pShouldPop = true;
	}

	if (!(bulletDanger || fireDanger || blastDanger))
	{
		return -1;
	}

	vaccChangeTimer = CHANGE_TIMER;

	// vaccinator_change_timer = (int) change_timer;
	if (bulletDanger >= fireDanger && bulletDanger >= blastDanger)
		return 0;
	if (blastDanger >= fireDanger && blastDanger >= bulletDanger)
		return 1;
	if (fireDanger >= bulletDanger && fireDanger >= blastDanger)
		return 2;
	return -1;
}

void SetResistance(int pResistance)
{
	Math::Clamp(pResistance, 0, 2);
	vaccChangeTimer = CHANGE_TIMER;
	vaccIdealResist = pResistance;

	const int curResistance = CurrentResistance();
	if (pResistance == curResistance)
		return;
	if (pResistance > curResistance)
		vaccChangeState = pResistance - curResistance;
	else
		vaccChangeState = 3 - curResistance + pResistance;
}

void DoResistSwitching(CUserCmd* pCmd)
{
	if (vaccChangeTimer > 0)
		vaccChangeTimer--;
	else
		vaccChangeTimer = CHANGE_TIMER;

	if (!vaccChangeState)
		return;
	if (CurrentResistance() == vaccIdealResist)
	{
		vaccChangeTicks = 0;
		vaccChangeState = 0;
		return;
	}
	if (pCmd->buttons & IN_RELOAD)
	{
		vaccChangeTicks = 8;
		return;
	}
	if (vaccChangeTicks <= 0)
	{
		pCmd->buttons |= IN_RELOAD;
		vaccChangeState--;
		vaccChangeTicks = 8;
	}
	else
		vaccChangeTicks--;
}

void CAutoUber::Run(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd)
{
		if (!Vars::Auto::Global::Active.Value) // change me
			return;

	if (!Vars::Auto::Uber::Active.Value
		|| pWeapon->GetWeaponID() != TF_WEAPON_MEDIGUN // not medigun, return
		|| G::CurItemDefIndex == Medic_s_TheKritzkrieg // kritzkrieg,  return
		|| ChargeCount() < 1) // not charged
	{
		return;
	}

	//Check local status, if enabled. Don't pop if local already is not vulnerable
	if (Vars::Auto::Uber::PopLocal.Value && !pLocal->IsInvulnerable())
	{
		m_flHealth = static_cast<float>(pLocal->m_iHealth());
		m_flMaxHealth = static_cast<float>(pLocal->GetMaxHealth());

		if (Vars::Auto::Uber::AutoVaccinator.Value && G::CurItemDefIndex == Medic_s_TheVaccinator)
		{
			// Auto vaccinator
			bool shouldPop = false;

			DoResistSwitching(pCmd);

			const int optResistance = OptimalResistance(pLocal, &shouldPop);
			if (optResistance >= 0 && optResistance != CurrentResistance())
				SetResistance(optResistance);

			if (shouldPop && CurrentResistance() == optResistance)
				pCmd->buttons |= IN_ATTACK2;
		}
		else
		{
			// Default medigun
			if (((m_flHealth / m_flMaxHealth) * 100.0f) <= Vars::Auto::Uber::HealthLeft.Value)
			{
				pCmd->buttons |= IN_ATTACK2; //We under the wanted health percentage, pop
				return; //Popped, no point checking our target's status
			}
		}
	}

	//Will be null as long as we aren't healing anyone
	if (const auto& pTarget = pWeapon->GetHealingTarget())
	{
		//Ignore if target is somehow dead, or already not vulnerable
		if (!pTarget->IsAlive() || pTarget->IsInvulnerable() || pTarget->GetDormant())
			return;

		//Dont waste if not a friend, fuck off scrub
		if (Vars::Auto::Uber::OnlyFriends.Value && !g_EntityCache.IsFriend(pTarget->GetIndex()))
			return;

		//Check target's status
		m_flHealth = static_cast<float>(pTarget->m_iHealth());
		m_flMaxHealth = static_cast<float>(pTarget->GetMaxHealth());

		if (Vars::Auto::Uber::VoiceCommand.Value)
		{
			const int iTargetIndex = pTarget->GetIndex();
			for (const auto& iEntity : G::MedicCallers)
			{
				if (iEntity == iTargetIndex)
				{
					pCmd->buttons |= IN_ATTACK2;
					break;
				}
			}
		}
		G::MedicCallers.clear();


		if (Vars::Auto::Uber::AutoVaccinator.Value && G::CurItemDefIndex == Medic_s_TheVaccinator)
		{
			// Auto vaccinator
			bool shouldPop = false;
			DoResistSwitching(pCmd);

			const int optResistance = OptimalResistance(pTarget, &shouldPop);
			if (optResistance >= 0 && optResistance != CurrentResistance())
				SetResistance(optResistance);

			if (shouldPop && CurrentResistance() == optResistance)
				pCmd->buttons |= IN_ATTACK2;
		}
		else
		{
			// Default mediguns
			if (((m_flHealth / m_flMaxHealth) * 100.0f) <= Vars::Auto::Uber::HealthLeft.Value)
				pCmd->buttons |= IN_ATTACK2; //Target under wanted health percentage, pop
		}
	}
}
