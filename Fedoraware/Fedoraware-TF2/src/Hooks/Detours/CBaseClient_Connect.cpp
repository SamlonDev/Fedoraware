#include "../Hooks.h"
#include "../../Features/Visuals/Materials/Materials.h"

MAKE_HOOK(CBaseClient_Connect, S::CBaseClient_Connect(), void, __fastcall,
	void* ecx, void* edx, const char* szName, int nUserID, INetChannel* pNetChannel, bool bFakePlayer, int clientChallenge)
{
	F::Materials.ReloadMaterials();
	return Hook.Original<FN>()(ecx, edx, szName, nUserID, pNetChannel, bFakePlayer, clientChallenge);
}