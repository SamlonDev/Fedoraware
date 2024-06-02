#include "PlayerUtils.h"
#include "../../Vars.h"
#include "../../Color.h"
#include "../../Logs/Logs.h"

// Gets the friends ID for a given player index
uint32_t GetFriendsID(int iIndex)
{
    PlayerInfo_t pi{};
    if (I::EngineClient->GetPlayerInfo(iIndex, &pi))
        return pi.friendsID;
    return 0;
}

// Checks if a tag exists in the map of tags
bool TagExists(const std::map<std::string, PriorityLabel_t>& tags, const std::string& tag)
{
    return tags.find(tag) != tags.end();
}

// Gets the priority label for a given tag
bool GetTag(const std::map<std::string, PriorityLabel_t>& tags, const std::string& tag, PriorityLabel_t* plTag)
{
    if (tag == "")
        return false;

    const auto find = tags.find(tag);
    if (find != tags.end())
    {
        *plTag = find->second;
        return true;
    }

    return false;
}

// Adds a tag to a player's list of tags
void AddTag(uint32_t friendID, const std::string& tag, bool save, const std::string& playerName)
{
    if (!friendID)
        return;

    if (!HasTag(friendID, tag))
    {
        G::PlayerTags[friendID].push_back(tag);
        G::bSavePlayers = save;

        if (playerName != "")
        {
            PriorityLabel_t plTag;
            if (GetTag(tag, &plTag))
                F::Logs.TagsChanged(playerName, "Added", plTag.Color.to_hex_alpha(), tag);
        }
    }
}

// Adds a tag to a player's list of tags, given their index
void AddTag(int iIndex, const std::string& tag, bool save, const std::string& playerName)
{
    if (const uint32_t friendID = GetFriendsID(iIndex))
        AddTag(friendID, tag, save, playerName);
}

// Removes a tag from a player's list of tags
void RemoveTag(uint32_t friendID, const std::string& tag, bool save, const std::string& playerName)
{
    if (!friendID)
        return;

    auto& mTags = G::PlayerTags[friendID];
    for (auto it = mTags.begin(); it != mTags.end(); it++)
    {
        if (*it == tag)
        {
            mTags.erase(it);
            G::bSavePlayers = save;

            if (playerName != "")
            {
                PriorityLabel_t plTag;
                if (GetTag(tag, &plTag))
                    F::Logs.TagsChanged(playerName, "Removed", plTag.Color.to_hex_alpha(), tag);
            }
            break;
        }
    }
}

// Removes a tag from a player's list of tags, given their index
void RemoveTag(int iIndex, const std::string& tag, bool save, const std::string& playerName)
{
    if (const uint32_t friendID = GetFriendsID(iIndex))
        RemoveTag(friendID, tag, save, playerName);
}

// Checks if a player has any tags
bool HasTags(uint32_t friendID)
{
    if (!friendID)
        return false;

    return G::PlayerTags[friendID].size() > 0;
}

// Checks if a player has any tags, given their index
bool HasTags(int iIndex)
{
    if (const uint32_t friendID = GetFriendsID(iIndex))
        return HasTags(friendID);
    return false;
}

// Checks if a player has a specific tag
bool HasTag(uint32_t friendID, const std::string& tag)
{
    if (!friendID)
        return false;

    for (const auto& sTag : G::PlayerTags[friendID])
    {
        if (tag == sTag)
            return true;
    }
    return false;
}

// Checks if a player has a specific tag, given their index
bool HasTag(int iIndex, const std::string& tag)
{
    if (const uint32_t friendID = GetFriendsID(iIndex))
        return HasTag(friendID, tag);
    return false;
}

// Gets the priority for a player based on their tags
int GetPlayerPriority(uint32_t friendID)
{
    const int iDefault = G::mTags["Default"].Priority;
    if (!friendID)
        return iDefault;

    if (HasTag(friendID, "Ignored"))
        return G::mTags["Ignored"].Priority;

    std::vector<int> vPriorities;
    for (const auto& sTag : G::PlayerTags[friendID])
    {
        PriorityLabel_t plTag;
        if (GetTag(sTag, &plTag) && !plTag.Label)
            vPriorities.push_back(plTag.Priority);
    }
    if (Utils::IsSteamFriend(friendID))
    {
        auto& plTag = G::mTags["Friend"];
        if (!plTag.Label)
            vPriorities.push_back(plTag.Priority);
    }

    if (vPriorities.size() > 0)
    {
        std::sort(vPriorities.begin(), vPriorities.end(), std::greater<int>());
        return vPriorities[0];
    }
    return iDefault;
}

// Gets the priority for a player based on their tags, given their index
int GetPlayerPriority(int iIndex)
{
    if (const uint32_t friendID = GetFriendsID(iIndex))
        return GetPlayerPriority(friendID);
    return G::mTags["Default"].Priority;
}

// Gets the most significant tag for a player
bool GetSignificantTag(uint32_t friendID, std::string* sTag, PriorityLabel_t* plTag, int iMode)
{
    if (!friendID)
        return false;

    std::vector<std::pair<std::string, PriorityLabel_t>> vLabels;
    if (!iMode || iMode == 1)
    {
        if (HasTag(friendID, "Ignored"))
        {
            *sTag = "Ignored";
            *plTag = G::mTags["Ignored"];
            return true;
        }

        for (const auto& sTag : G::PlayerTags[friendID])
        {
            PriorityLabel_t plTag;
            if (GetTag(sTag, &plTag) && !plTag.Label)
                vLabels.push_back({ sTag, plTag });
        }
        if (Utils::IsSteamFriend(friendID))
        {
            auto& plTag = G::mTags["Friend"];
            if (!plTag.Label)
                vLabels.push_back({ "Friend", plTag });
        }
    }
    if ((!iMode || iMode == 2) && vLabels.size() == 0)
    {
        for (const auto& sTag : G::PlayerTags[friendID])
        {
            PriorityLabel_t plTag;
            if (GetTag(sTag, &plTag) && plTag.Label)
                vLabels.push_back({ sTag, plTag });
        }
        if (Utils::IsSteamFriend(friendID))
        {
            auto& plTag = G::mTags["Friend"];
            if (plTag.Label)
                vLabels.push_back({ "Friend", plTag });
        }
    }
    if (vLabels.size() == 0)
        return false;

    std::sort(vLabels.begin(), vLabels.end(), [&](const auto& a, const auto& b) -> bool
        {
            // sort by priority if unequal
            if (a.second.Priority != b.second.Priority)
                return a.second.Priority > b.second.Priority;

            return a.first < b.first;
        });
    *sTag = vLabels[0].first;
    *plTag = vLabels[0].second;
    return true;
}

// Gets the most significant tag for a player, given their index
bool GetSignificantTag(int iIndex, std::string* sTag, PriorityLabel_t* plTag, int iMode)
{
    if (const uint32_t friendID = GetFriendsID(iIndex))
        return GetSignificantTag(friendID, sTag, plTag, iMode);
    return false;
}

// Checks if a player is ignored
bool IsIgnored(uint32_t friendID)
{
    if (!friendID)
        return false;

    const int iPriority = GetPlayerPriority(friendID);
    const int iIgnored = G::mTags["Ignored"].Priority;
    return iPriority <= iIgnored;
}

// Checks if a player is ignored, given their index
bool IsIgnored(int iIndex)
{
    if (const uint32_t friendID = GetFriendsID(iIndex))
        return IsIgnored(friendID);
    return false;
}

// Checks if a player is a friend
bool IsFriend(int iIndex)
{
    if (const uint32_t friendID = GetFriendsID(iIndex))
        return Utils::IsSteamFriend(friendID);
    return false;
}

// Updates the player cache
void UpdatePlayers()
{
    static Timer updateTimer{};
    if (updateTimer.Run(1000))
    {
        std::lock_guard<std::mutex> lock(mutex);
        vPlayerCache.clear();

        const auto& pr = g_EntityCache.GetPR();
        if (!pr)
            return;

        for (int n = 1; n <= I::EngineClient->GetMaxClients(); n++)
        {
            if (!pr->GetValid(n) || !pr->GetConnected(n))
                continue;

            bool bFake = true, bFriend = false;
            PlayerInfo_t pi{};
            if (I::EngineClient->GetPlayerInfo(n, &pi))
            {
                bFake = pi.fakeplayer;
                bFriend = Utils::IsSteamFriend(pi.friendsID);
            }

            vPlayerCache.push_back({
                pr->GetPlayerName(n),
                pr->GetAccountID(n),
                pr->GetUserID(n),
                pr->GetTeam(n),
                pr->GetClass(n),
                pr->IsAlive(n),
                n == I::EngineClient->GetLocalPlayer(),
                bFriend,
                bFake
            });
        }
    }
}
