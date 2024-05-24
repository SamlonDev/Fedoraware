#include "AutoDetonate.h"

#include "../../Vars.h"

bool CAutoDetonate::CheckDetonation(CBaseEntity* pLocal, EGroupType entityGroup, float flRadiusScale, CUserCmd* pCmd)
{
	for (const auto& pExplosive : g_EntityCache.GetGroup(entityGroup))
	{
		if (pExplosive->m_iType() == TF_GL_MODE_REMOTE_DETONATE_PRACTICE || !pExplosive->m_bPulsed())
			continue;

		const Vec3 vOrigin = pExplosive->GetWorldSpaceCenter();
		if (entityGroup == EGroupType::LOCAL_STICKIES)
		{
			if (pExplosive->m_iType() == TF_GL_MODE_REMOTE_DETONATE && !pExplosive->m_bTouched())
			{
				static auto tf_grenadelauncher_livetime = g_ConVars.FindVar("tf_grenadelauncher_livetime");
				static auto tf_sticky_radius_ramp_time = g_ConVars.FindVar("tf_sticky_radius_ramp_time");
				static auto tf_sticky_airdet_radius = g_ConVars.FindVar("tf_sticky_airdet_radius");
				float flLiveTime = tf_grenadelauncher_livetime ? tf_grenadelauncher_livetime->GetFloat() : 0.8f;
				float flRampTime = tf_sticky_radius_ramp_time ? tf_sticky_radius_ramp_time->GetFloat() : 2.f;
				float flAirdetRadius = tf_sticky_airdet_radius ? tf_sticky_airdet_radius->GetFloat() : 0.85f;
				flRadiusScale *= Math::RemapValClamped(I::GlobalVars->curtime - pExplosive->m_flCreationTime(), flLiveTime, flLiveTime + flRampTime, flAirdetRadius, 1.f);
			}
		}
		float flRadius = (entityGroup == EGroupType::LOCAL_STICKIES ? 146.f : 110.f) * flRadiusScale;

		// Iterate through entities in sphere radius
		CBaseEntity* pTarget;
		for (CEntitySphereQuery sphere(vOrigin, flRadius);
			(pTarget = sphere.GetCurrentEntity()) != nullptr;
			sphere.NextEntity())
		{
			if (!pTarget || pTarget == pLocal || !pTarget->IsAlive() || pTarget->IsPlayer() && pTarget->IsAGhost() || pTarget->m_iTeamNum() == pLocal->m_iTeamNum())
				continue;

			// CEntitySphereQuery actually does a box test so we need to make sure the distance is less than the radius first
			Vec3 vPos = {}; pTarget->GetCollision()->CalcNearestPoint(vOrigin, &vPos);
			if (vOrigin.DistTo(vPos) > flRadius)
				continue;

			const bool isPlayer = pTarget->IsPlayer() && Vars::Aimbot::General::Target.Value & ToAimAt::PLAYER && !F::AimbotGlobal.ShouldIgnore(pTarget);
			const bool isSentry = Vars::Aimbot::General::Target.Value & ToAimAt::SENTRY && pTarget->IsSentrygun();
			const bool isDispenser = Vars::Aimbot::General::Target.Value & ToAimAt::DISPENSER && pTarget->IsDispenser();
			const bool isTeleporter = Vars::Aimbot::General::Target.Value & ToAimAt::TELEPORTER && pTarget->IsTeleporter();
			const bool isSticky = Vars::Aimbot::General::Target.Value & ToAimAt::STICKY && pTarget->GetClassID() == ETFClassID::CTFGrenadePipebombProjectile && pTarget->m_iType() == TF_GL_MODE_REMOTE_DETONATE && (G::CurItemDefIndex == Demoman_s_TheQuickiebombLauncher || G::CurItemDefIndex == Demoman_s_TheScottishResistance);
			const bool isNPC = Vars::Aimbot::General::Target.Value & ToAimAt::NPC && pTarget->IsNPC();
			const bool isBomb = Vars::Aimbot::General::Target.Value & ToAimAt::BOMB && pTarget->IsBomb();
			if (isPlayer || isSentry || isDispenser || isTeleporter || isNPC || isBomb || isSticky)
			{
				if (!Utils::VisPosProjectile(pExplosive, pTarget, vOrigin, isPlayer ? pTarget->m_vecOrigin() + pTarget->GetViewOffset() : pTarget->GetWorldSpaceCenter(), MASK_SHOT))
					continue;

				if (G::CurItemDefIndex == Demoman_s_TheScottishResistance)
				{
					Vec3 vAngleTo = Math::CalcAngle(pLocal->GetShootPos(), vOrigin);
					Utils::FixMovement(pCmd, vAngleTo);
					pCmd->viewangles = vAngleTo;
					G::PSilentAngles = true;
				}
				return true;
			}
		}
	}

	return false;
}

void CAutoDetonate::Run(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd)
{
	if (!Vars::Aimbot::Projectile::AutoDetonate.Value)
		return;

	// Check sticky detonation
	if (Vars::Aimbot::Projectile::AutoDetonate.Value & (1 << 0) && CheckDetonation(pLocal, EGroupType::LOCAL_STICKIES, Vars::Aimbot::Projectile::AutodetRadius.Value / 100, pCmd))
		pCmd->buttons |= IN_ATTACK2;

	// Check flare detonation
	if (Vars::Aimbot::Projectile::AutoDetonate.Value & (1 << 1) && CheckDetonation(pLocal, EGroupType::LOCAL_FLARES, Vars::Aimbot::Projectile::AutodetRadius.Value / 100, pCmd))
		pCmd->buttons |= IN_ATTACK2;
}
