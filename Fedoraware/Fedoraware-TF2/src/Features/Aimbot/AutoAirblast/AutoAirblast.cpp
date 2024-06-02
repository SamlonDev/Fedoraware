#include "AutoAirblast.h"

#include "../../Vars.h"

bool CAutoAirblast::CanAirblastEntity(CBaseEntity* pLocal, CBaseEntity* pEntity, Vec3& vAngle, Vec3& vPos)
{
    if (!pLocal || !pEntity || !pLocal->IsAlive() || !pEntity->IsAlive())
    {
        return false;
    }

    Vec3 vForward = {};
    Math::AngleVectors(vAngle, &vForward);
    const Vec3 vOrigin = pLocal->GetShootPos() + (vForward * 128.f);

    CBaseEntity* pTarget = nullptr;
    for (CEntitySphereQuery sphere(vOrigin, 128.f);
        (pTarget = sphere.GetCurrentEntity()) != nullptr;
        sphere.NextEntity())
    {
        if (pTarget == pEntity)
        {
            break;
        }
    }

    if (!pTarget || pTarget != pEntity)
    {
        return false;
    }

    return Utils::VisPos(pLocal, pEntity, pLocal->GetShootPos(), vPos);
}

void CAutoAirblast::Run(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd)
{
    if (!pLocal || !pWeapon || !pCmd)
    {
        return;
    }

    if (!Vars::Aimbot::Projectile::AutoAirblast.Value || !G::CanSecondaryAttack)
    {
        return;
    }

    const int iWeaponID = pWeapon->GetWeaponID();
    if (iWeaponID != TF_WEAPON_FLAMETHROWER && iWeaponID != TF_WEAPON_FLAME_BALL || G::CurItemDefIndex == Pyro_m_ThePhlogistinator)
    {
        return;
    }

    const auto* pNet = I::EngineClient->GetNetChannelInfo();
    const float flLatency = pNet ? pNet->GetLatency(FLOW_INCOMING) + pNet->GetLatency(FLOW_OUTGOING) : 0.f;

    const Vec3 vEyePos = pLocal->GetShootPos();
    bool bShouldBlast = false;

    for (const auto& pProjectile : g_EntityCache.GetGroup(EGroupType::WORLD_PROJECTILES))
    {
        if (pProjectile->m_iTeamNum() == pLocal->m_iTeamNum())
        {
            continue;
        }

        if (pProjectile->m_bTouched() && (pProjectile->GetClassID() == ETFClassID::CTFGrenadePipebombProjectile || pProjectile->GetClassID() == ETFClassID::CTFStunBall))
        {
            continue;
        }

        if (pProjectile->GetClassID() == ETFClassID::CTFProjectile_Arrow && pProjectile->GetVelocity().IsZero())
        {
            continue;
        }

        Vec3 vPos = pProjectile->m_vecOrigin();
        const float flFov = Math::GetFov(I::EngineClient->GetViewAngles(), vEyePos, vPos);
        if (flFov > Vars::Aimbot::General::AimFOV.Value)
        {
            continue;
        }

        if (CanAirblastEntity(pLocal, pProjectile, pCmd->viewangles, vPos))
        {
            bShouldBlast = true;
            break;
        }

        if (!bShouldBlast && Vars::Aimbot::Projectile::AutoAirblast.Value == 2)
        {
            Vec3 vAngle = Math::CalcAngle(vEyePos, vPos);
            if (CanAirblastEntity(pLocal, pProjectile, vAngle, vPos))
            {
                Utils::FixMovement(pCmd, vAngle);
                pCmd->viewangles = vAngle;
                G::PSilentAngles = true;
                bShouldBlast = true;
                break;
            }
        }
    }

    if (bShouldBlast)
    {
        G::IsAttacking = true;
        pCmd->buttons |= IN_ATTACK2;
    }
}
