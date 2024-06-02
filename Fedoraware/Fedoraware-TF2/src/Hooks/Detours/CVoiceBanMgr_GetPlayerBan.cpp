#include "../Hooks.h"

static const auto CVoiceBanMgr_GetPlayerBan_pattern = g_Pattern.Find(L"client.dll", L"55 8B EC FF 75 ? E8 ? ? ? ? 85 C0 0F 95 C0 5D C2 ? ? CC CC CC CC CC CC CC CC CC CC CC 55 8B EC 83 EC");
if (CVoiceBanMgr_GetPlayerBan_pattern.empty()) {
    // Log error and return
}

MAKE_HOOK(CVoiceBanMgr_GetPlayerBan, CVoiceBanMgr_GetPlayerBan_pattern, bool, __fastcall,
    void* ecx, void* edx, std::string_view playerID)
{
    const bool bReturn = Hook.Original<FN>()(ecx, edx, playerID.data());
    //Utils::ConLog("CVoiceBanMgr_GetPlayerBan", "Player " + std::to_string(bReturn) + " is voice-banned: " + playerID);
    return bReturn;
}
