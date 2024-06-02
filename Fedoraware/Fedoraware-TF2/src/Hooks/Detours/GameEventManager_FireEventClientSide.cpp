#include "../Hooks.h"

#include "../../Features/Killstreak/Killstreak.h"
#include "../../Features/Fedworking/Fedworking.h"
#include <string>

MAKE_HOOK(GameEventManager_FireEventClientSide, Utils::GetVFuncPtr(I::GameEventManager, 8), bool, __fastcall,
    void* ecx, void* edx, CGameEvent* pEvent)
{
    if (!ecx || !edx || !pEvent) { return false; }

    const FNV1A_t uNameHash = FNV1A::Hash(pEvent->GetName());
    F::Killstreaker.FireEvents(pEvent, uNameHash);

    if (uNameHash == FNV1A::HashConst("party_chat"))
    {
        std::string msg = pEvent->GetString("text");

        // Handle networking
        if (msg.find("FED@") == 0)
        {
            if (Vars::Misc::PartyNetworking.Value)
            {
                F::Fedworking.HandleMessage(msg);
            }
            return false;
        }
    }

    return Hook.Original<FN>()(ecx, edx, pEvent);
}
