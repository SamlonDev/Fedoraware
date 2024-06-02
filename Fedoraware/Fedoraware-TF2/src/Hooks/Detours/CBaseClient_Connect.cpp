#include "../Hooks.h"
#include "../../Features/Visuals/Materials/Materials.h"

// Use a typedef to make the hook declaration easier to read and write
typedef void(__fastcall * OriginalFn)(void* ecx, void* edx, const char* szName, int nUserID, INetChannel* pNetChannel, bool bFakePlayer, int clientChallenge);

// Use a more descriptive name for the hook
HOOK_FN(CBaseClient_OnConnect, CBaseClient_Connect, OriginalFn,
    void* ecx, void* edx, const char* szName, int nUserID, INetChannel* pNetChannel, bool bFakePlayer, int clientChallenge)
{
    // Call the original function first
    OriginalFn(ecx, edx, szName, nUserID, pNetChannel, bFakePlayer, clientChallenge);

    // Then perform any additional actions
    F::Materials.ReloadMaterials();
}
