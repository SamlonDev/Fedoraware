#include "../Hooks.h"

MAKE_HOOK(CStudioRender_DrawModelStaticProp, Utils::GetVFuncPtr(I::StudioRender, 30), void, __fastcall,
	void* ecx, void* edx, const DrawModelState_t& pState, const matrix3x4& modelToWorld, int flags)
{
	if (Vars::Visuals::World::NearPropFade.Value)
	{
		if (const auto& pLocal = g_EntityCache.GetLocal())
		{
			Vec3 vOrigin = { modelToWorld[0][3], modelToWorld[1][3], modelToWorld[2][3] };

			const float flDistance = pLocal->m_vecOrigin().DistTo(vOrigin);

			float flAlpha = 1.f;
			if (flDistance < 300.0f)
				flAlpha = Math::RemapValClamped(flDistance, 150.0f, 300.0f, 0.15f, 1.f);
			I::StudioRender->SetAlphaModulation(flAlpha);
		}
	}

	Hook.Original<FN>()(ecx, edx, pState, modelToWorld, flags);
}