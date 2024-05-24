#include "../Hooks.h"

#include "../../Features/Menu/Playerlist/PlayerUtils.h"

//i stole this from mfed :)

namespace S
{
    MAKE_SIGNATURE(CVoiceStatus_IsPlayerBlocked_Call, CLIENT_DLL, "84 C0 BA ? ? ? ? 68", 0x0);
    MAKE_SIGNATURE(VGui_MenuBuilder_AddMenuItem_Call, CLIENT_DLL, "8B 0D ? ? ? ? 8D 55 ? 52 8D 55 ? 52 8B 01 FF 50 ? FF 75 ? 8B 8E ? ? ? ? FF 75 ? E8", 0x0);
}

static int PlayerIndex;
static std::string PlayerName;
static uint32_t FriendsID;

MAKE_HOOK(CVoiceStatus_IsPlayerBlocked, S::CVoiceStatus_IsPlayerBlocked(), bool, __fastcall,
    void* ecx, void* edx, int playerIndex)
{
    static auto CVoiceStatus_IsPlayerBlocked_Call = S::CVoiceStatus_IsPlayerBlocked_Call();
    const auto dwRetAddr = reinterpret_cast<DWORD>(_ReturnAddress());

    if (!Vars::Visuals::UI::ScoreboardPlayerlist.Value)
        return Hook.Original<FN>()(ecx, edx, playerIndex);

    if (dwRetAddr == CVoiceStatus_IsPlayerBlocked_Call)
        PlayerIndex = playerIndex;

    return Hook.Original<FN>()(ecx, edx, playerIndex);
}

MAKE_HOOK(VGui_MenuBuilder_AddMenuItem, S::VGui_MenuBuilder_AddMenuItem(), void*, __fastcall,
    void* ecx, void* edx, const char* pszButtonText, const char* pszCommand, const char* pszCategoryName)
{
    static auto VGui_MenuBuilder_AddMenuItem_Call = S::VGui_MenuBuilder_AddMenuItem_Call();
    const auto dwRetAddr = reinterpret_cast<DWORD>(_ReturnAddress());

    if (!Vars::Visuals::UI::ScoreboardPlayerlist.Value)
        return Hook.Original<FN>()(ecx, edx, pszButtonText, pszCommand, pszCategoryName);

    if (dwRetAddr == VGui_MenuBuilder_AddMenuItem_Call && PlayerIndex != -1)
    {
        auto ret = Hook.Original<FN>()(ecx, edx, pszButtonText, pszCommand, pszCategoryName);

        const auto& pr = g_EntityCache.GetPR(); PlayerInfo_t pi{};
        if (pr && I::EngineClient->GetPlayerInfo(PlayerIndex, &pi))
        {
            PlayerName = pi.name;
            FriendsID = pi.friendsID;

            const bool bIgnored = F::PlayerUtils.HasTag(FriendsID, "Ignored");
            const bool bCheater = F::PlayerUtils.HasTag(FriendsID, "Cheater");

            Hook.Original<FN>()(ecx, edx, std::format("{} {}", bIgnored ? "Unignore" : "Ignore", PlayerName).c_str(), "fedignore", "fed");
            Hook.Original<FN>()(ecx, edx, std::format("{} {}", bCheater ? "Unmark" : "Mark", PlayerName).c_str(), "fedmark", "fed");
        }

        return ret;
    }

    return Hook.Original<FN>()(ecx, edx, pszButtonText, pszCommand, pszCategoryName);
}

MAKE_HOOK(CTFClientScoreBoardDialog_OnCommand, S::CTFClientScoreBoardDialog_OnCommand(), void, __fastcall,
    void* ecx, void* edx, const char* command)
{
    if (!Vars::Visuals::UI::ScoreboardPlayerlist.Value)
        return Hook.Original<FN>()(ecx, edx, command);

    if (FNV1A::Hash(command) == FNV1A::HashConst("fedignore"))
    {
        if (!F::PlayerUtils.HasTag(FriendsID, "Ignored"))
            F::PlayerUtils.AddTag(FriendsID, "Ignored", true, PlayerName);
        else
            F::PlayerUtils.RemoveTag(FriendsID, "Ignored", true, PlayerName);
    }
    else if (FNV1A::Hash(command) == FNV1A::HashConst("fedmark"))
    {
        if (!F::PlayerUtils.HasTag(FriendsID, "Cheater"))
            F::PlayerUtils.AddTag(FriendsID, "Cheater", true, PlayerName);
        else
            F::PlayerUtils.RemoveTag(FriendsID, "Cheater", true, PlayerName);
    }

    Hook.Original<FN>()(ecx, edx, command);
}