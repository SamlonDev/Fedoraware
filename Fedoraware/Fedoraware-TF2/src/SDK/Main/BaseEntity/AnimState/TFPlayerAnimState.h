#pragma once
#include "MultiPlayerAnimState.h"

class ICTFPlayerAnimStateListener
{
public:
    virtual void OnClearAnimationState() = 0;
    // Add other event handlers as needed
};

class CTFPlayerAnimState : private CMultiPlayerAnimState
{
public:
    CTFPlayerAnimState(CBaseEntity* pTFPlayer);
    virtual ~CTFPlayerAnimState();

    CBaseEntity* GetTFPlayer() const { return m_pTFPlayer; }

    virtual void ClearAnimationState() override;
    virtual Activity TranslateActivity(Activity actDesired) override;
    virtual void Update(float eyeYaw, float eyePitch) override;
    virtual void CheckStunAnimation() override;
    virtual Activity CalcMainActivity() override;
    virtual void ComputePoseParam_AimYaw(CStudioHdr* pStudioHdr) override;
    virtual float GetCurrentMaxGroundSpeed() override;
    virtual float GetGesturePlaybackRate() override;
    virtual bool ShouldUpdateAnimState() override;
    virtual void GetOuterAbsVelocity(Vec3& vel) override;
    virtual void RestartGesture(int iGestureSlot, Activity iGestureActivity, bool bAutoKill = true) override;

    void SetRenderAngles(const Vec3& angles);

    void AddListener(ICTFPlayerAnimStateListener* pListener);
    void RemoveListener(ICTFPlayerAnimStateListener* pListener);

private:
    CBaseEntity* m_pTFPlayer;
    bool m_bInAirWalk;
    float m_flHoldDeployedPoseUntilTime;
    float m_flTauntMoveX;
    float m_flTauntMoveY;
    float m_flVehicleLeanVel;
    float m_flVehicleLeanPos;
    Vec3 m_vecSmoothedUp;

    std::vector<ICTFPlayerAnimStateListener*> m_Listeners;

    void NotifyClearAnimationState();
    // Add other helper methods as needed
};

#define MAXSTUDIOPOSEPARAM 24
