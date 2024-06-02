#include "FakeAngle.h"
#include "../../PacketManip/AntiAim/AntiAim.h"

void CFakeAngle::Run()
{
    const auto pLocal = g_EntityCache.GetLocal();
    if (!pLocal || !pLocal->IsAlive() || pLocal->IsAGhost())
    {
        // Skip processing if there's no local player or they're not alive or they're a ghost
        return;
    }

    auto pAnimState = pLocal->GetAnimState();
    if (!pAnimState)
    {
        // Skip processing if there's no animation state
        return;
    }

    // Clamp the pitch value to -89 to 89 degrees
    Math::Clamp(F::AntiAim.vFakeAngles.x, -89.f, 89.f);

    // Save the old variable values
    const float flOldFrameTime = I::GlobalVars->frametime;
    const int nOldSequence = pLocal->m_nSequence();
    const float flOldCycle = pLocal->m_flCycle();
    CTFPlayerAnimState pOldAnimState = *pAnimState;
    std::array<float, MAX_STUDIO_BONE> pOldPoseParams = pLocal->m_flPoseParameter();

    // Set the new variable values
    I::GlobalVars->frametime = 0.f;
    pAnimState->m_flCurrentFeetYaw = F::AntiAim.vFakeAngles.y;
    pAnimState->m_flEyeYaw = F::AntiAim.vFakeAngles.y;
    pAnimState->Update(F::AntiAim.vFakeAngles.y, F::AntiAim.vFakeAngles.x);

    // Set up the bones
    auto BonesSetup = pLocal->SetupBones(BoneMatrix, 128, BONE_USED_BY_ANYTHING, I::GlobalVars->curtime);

    // Restore the old variable values
    I::GlobalVars->frametime = flOldFrameTime;
    pLocal->m_nSequence() = nOldSequence;
    pLocal->m_flCycle() = flOldCycle;
    pLocal->m_flPoseParameter() = pOldPoseParams;
    *pAnimState = pOldAnimState;
}
