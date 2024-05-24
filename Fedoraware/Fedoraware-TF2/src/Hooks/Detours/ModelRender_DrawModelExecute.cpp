#include "../Hooks.h"

#include "../../Features/Visuals/Chams/Chams.h"
#include "../../Features/Visuals/Glow/Glow.h"
#include "../../Features/Visuals/Materials/Materials.h"

namespace S
{
	MAKE_SIGNATURE(m_hViewmodelAttachment_DrawModel, CLIENT_DLL, "FF 75 0C FF 50 28 8B 8E ? ? ? ? 85 C9 74 28 BA ? ? ? ? 83 F9 FF 74 03 0F B7 D1", 0x6);
}

MAKE_HOOK(ModelRender_DrawModelExecute, Utils::GetVFuncPtr(I::ModelRender, 19), void, __fastcall,
	void* ecx, void* edx, const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4* pBoneToWorld)
{
	/*
	if (!F::Chams.iRendering && !F::Glow.bRendering && !I::EngineVGui->IsGameUIVisible())
	{
		if (const auto& pEntity = I::ClientEntityList->GetClientEntity(pInfo.m_nEntIndex))
			Utils::ConLog("Entity", std::format("{}, {}, {}", pInfo.m_nEntIndex, int(pEntity->GetClassID()), I::ModelInfoClient->GetModelName(pInfo.m_pModel)).c_str());
		else
			Utils::ConLog("Model", std::format("{}, {}", pInfo.m_nEntIndex, I::ModelInfoClient->GetModelName(pInfo.m_pModel)).c_str());
	}
	*/

	if (Vars::Visuals::UI::CleanScreenshots.Value && I::EngineClient->IsTakingScreenshot())
		return Hook.Original<FN>()(ecx, edx, pState, pInfo, pBoneToWorld);

	if (F::Chams.bRendering)
		return F::Chams.RenderHandler(pState, pInfo, pBoneToWorld);
	if (F::Glow.bRendering)
		return F::Glow.RenderHandler(pState, pInfo, pBoneToWorld);

	if (F::Chams.mEntities[pInfo.m_nEntIndex])
		return;

	const auto& pEntity = I::ClientEntityList->GetClientEntity(pInfo.m_nEntIndex);
	if (pEntity && pEntity->GetClassID() == ETFClassID::CTFViewModel)
	{
		F::Glow.RenderViewmodel(pState, pInfo, pBoneToWorld);
		if (F::Chams.RenderViewmodel(pState, pInfo, pBoneToWorld))
			return;
	}

	Hook.Original<FN>()(ecx, edx, pState, pInfo, pBoneToWorld);
}

MAKE_HOOK(C_BaseAnimating_DrawModel, S::CBaseAnimating_DrawModel(), int, __fastcall,
	void* ecx, void* edx, int flags)
{
	static const auto dwDrawModel = S::m_hViewmodelAttachment_DrawModel();
	const auto dwRetAddr = reinterpret_cast<DWORD>(_ReturnAddress());

	if (Vars::Visuals::UI::CleanScreenshots.Value && I::EngineClient->IsTakingScreenshot())
		return Hook.Original<FN>()(ecx, edx, flags);

	if (dwRetAddr == dwDrawModel && flags & STUDIO_RENDER)
	{
		int iReturn;
		F::Glow.RenderViewmodel(ecx, flags);
		if (F::Chams.RenderViewmodel(ecx, flags, &iReturn))
			return iReturn;
	}

	return Hook.Original<FN>()(ecx, edx, flags);
}