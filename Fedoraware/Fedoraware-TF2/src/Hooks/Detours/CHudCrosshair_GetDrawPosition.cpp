#include "../Hooks.h"

MAKE_HOOK(CHudCrosshair_GetDrawPosition, S::CHudCrosshair_GetDrawPosition(), void, __cdecl,
	float* pX, float* pY, bool* pbBehindCamera, Vec3 angleCrosshairOffset)
{
	if (Vars::Visuals::UI::CleanScreenshots.Value && I::EngineClient->IsTakingScreenshot())
		return Hook.Original<FN>()(pX, pY, pbBehindCamera, angleCrosshairOffset);

	const auto& pLocal = g_EntityCache.GetLocal();
	if (!pLocal)
		return Hook.Original<FN>()(pX, pY, pbBehindCamera, angleCrosshairOffset);

	bool bSet = false;

	if (Vars::Visuals::ThirdPerson::Crosshair.Value &&
		I::Input->CAM_IsThirdPerson())
	{
		const Vec3 viewangles = I::EngineClient->GetViewAngles();
		Vec3 vForward{};
		Math::AngleVectors(viewangles, &vForward);

		const Vec3 vStartPos = pLocal->GetEyePosition();
		const Vec3 vEndPos = (vStartPos + vForward * 8192);

		CGameTrace trace = {};
		CTraceFilterHitscan filter = {};
		filter.pSkip = pLocal;
		Utils::Trace(vStartPos, vEndPos, MASK_SHOT, &filter, &trace);

		Vec3 vScreen;
		if (Utils::W2S(trace.vEndPos, vScreen))
		{
			if (pX) *pX = vScreen.x;
			if (pY) *pY = vScreen.y;
			if (pbBehindCamera) *pbBehindCamera = false;
			bSet = true;
		}
	}

	if (Vars::Visuals::Viewmodel::CrosshairAim.Value &&
		pLocal->IsAlive())
	{
		static Vec3 vPos = {};
		static int iTick = 0;

		if (!G::AimPos.IsZero())
		{
			vPos = G::AimPos;
			iTick = I::GlobalVars->tickcount;
		}

		if (abs(iTick - I::GlobalVars->tickcount) < 32)
		{
			Vec3 vScreen;
			if (Utils::W2S(vPos, vScreen))
			{
				if (pX) *pX = vScreen.x;
				if (pY) *pY = vScreen.y;
				if (pbBehindCamera) *pbBehindCamera = false;
				bSet = true;
			}
		}
	}

	if (!bSet)
		Hook.Original<FN>()(pX, pY, pbBehindCamera, angleCrosshairOffset);
}