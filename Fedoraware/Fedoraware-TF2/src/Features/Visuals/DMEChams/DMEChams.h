#pragma once
#include "../../Feature.h"

class CChams
{
private:
	void SetupBegin(IMatRenderContext* pRenderContext, bool bTwoModels = false);
	void SetupVisible(IMatRenderContext* pRenderContext, bool bTwoModels = false);
	void SetupOccluded(IMatRenderContext* pRenderContext);
	void SetupEnd(IMatRenderContext* pRenderContext, bool bTwoModels = false);

	void DrawModel(Chams_t chams, const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4* pBoneToWorld, bool bTwoModels = true, bool bViewmodel = false);

	bool GetChams(int iIndex, std::string_view szModelName, Chams_t* pChams, bool* bViewmodel);

	void RenderFakeAng(const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4* pBoneToWorld);
	void RenderBacktrack(const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4* pBoneToWorld);

public:
	bool Render(const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4* pBoneToWorld);

	bool m_bRendering;
};

ADD_FEATURE(CChams, Chams)