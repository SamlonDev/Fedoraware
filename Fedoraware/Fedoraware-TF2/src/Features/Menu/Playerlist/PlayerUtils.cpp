#include "PlayerUtils.h"
#include "../../Vars.h"
#include "../../Color.h"
#include "../../Logs/Logs.h"

uint32_t GetFriendsID(int iIndex)
{
	PlayerInfo_t pi{};
	if (I::EngineClient->GetPlayerInfo(iIndex, &pi))
		return pi.friendsID;
	return 0;
}

bool CPlayerlistUtils::GetTag(std::string sTag, PriorityLabel_t* plTag)
{
	if (sTag == "")
		return false;

	const auto find = mTags.find(sTag);
	if (find != mTags.end())
	{
		*plTag = find->second;
		return true;
	}

	return false;
}



void CPlayerlistUtils::AddTag(uint32_t friendsID, std::string sTag, bool bSave, std::string sName)
{
	if (!friendsID)
		return;

	if (!HasTag(friendsID, sTag))
	{
		G::PlayerTags[friendsID].push_back(sTag);
		bSavePlayers = bSave;
		if (sName != "")
		{
			PriorityLabel_t plTag;
			if (GetTag(sTag, &plTag))
				F::Logs.TagsChanged(sName, "Added", plTag.Color.to_hex_alpha(), sTag);
		}
	}
}
void CPlayerlistUtils::AddTag(int iIndex, std::string sTag, bool bSave, std::string sName)
{
	if (const uint32_t friendsID = GetFriendsID(iIndex))
		AddTag(friendsID, sTag, bSave, sName);
}

void CPlayerlistUtils::RemoveTag(uint32_t friendsID, std::string sTag, bool bSave, std::string sName)
{
	if (!friendsID)
		return;

	auto& mTags = G::PlayerTags[friendsID];
	for (auto it = mTags.begin(); it != mTags.end(); it++)
	{
		if (*it == sTag)
		{
			mTags.erase(it);
			bSavePlayers = bSave;
			if (sName != "")
			{
				PriorityLabel_t plTag;
				if (GetTag(sTag, &plTag))
					F::Logs.TagsChanged(sName, "Removed", plTag.Color.to_hex_alpha(), sTag);
			}
			break;
		}
	}
}
void CPlayerlistUtils::RemoveTag(int iIndex, std::string sTag, bool bSave, std::string sName)
{
	if (const uint32_t friendsID = GetFriendsID(iIndex))
		RemoveTag(friendsID, sTag, bSave, sName);
}

bool CPlayerlistUtils::HasTags(uint32_t friendsID)
{
	if (!friendsID)
		return false;

	return G::PlayerTags[friendsID].size();
}
bool CPlayerlistUtils::HasTags(int iIndex)
{
	if (const uint32_t friendsID = GetFriendsID(iIndex))
		return HasTags(friendsID);
	return false;
}

bool CPlayerlistUtils::HasTag(uint32_t friendsID, std::string sTag)
{
	if (!friendsID)
		return false;

	for (const auto& _sTag : G::PlayerTags[friendsID])
	{
		if (sTag == _sTag)
			return true;
	}
	return false;
}
bool CPlayerlistUtils::HasTag(int iIndex, std::string sTag)
{
	if (const uint32_t friendsID = GetFriendsID(iIndex))
		return HasTag(friendsID, sTag);
	return false;
}



int CPlayerlistUtils::GetPriority(uint32_t friendsID)
{
	const int iDefault = mTags["Default"].Priority;
	if (!friendsID)
		return iDefault;

	if (HasTag(friendsID, "Ignored"))
		return mTags["Ignored"].Priority;

	std::vector<int> vPriorities;
	for (const auto& sTag : G::PlayerTags[friendsID])
	{
		PriorityLabel_t plTag;
		if (F::PlayerUtils.GetTag(sTag, &plTag) && !plTag.Label)
			vPriorities.push_back(plTag.Priority);
	}
	if (Utils::IsSteamFriend(friendsID))
	{
		auto& plTag = mTags["Friend"];
		if (!plTag.Label)
			vPriorities.push_back(plTag.Priority);
	}

	if (vPriorities.size())
	{
		std::sort(vPriorities.begin(), vPriorities.end(), std::greater<int>());
		return *vPriorities.begin();
	}
	return iDefault;
}
int CPlayerlistUtils::GetPriority(int iIndex)
{
	if (const uint32_t friendsID = GetFriendsID(iIndex))
		return GetPriority(friendsID);
	return mTags["Default"].Priority;
}

bool CPlayerlistUtils::GetSignificantTag(uint32_t friendsID, std::string* sTag, PriorityLabel_t* plTag, int iMode)
{
	if (!friendsID)
		return false;

	std::vector<std::pair<std::string, PriorityLabel_t>> vLabels;
	if (!iMode || iMode == 1)
	{
		if (HasTag(friendsID, "Ignored"))
		{
			*sTag = "Ignored";
			*plTag = mTags["Ignored"];
			return true;
		}

		for (const auto& _sTag : G::PlayerTags[friendsID])
		{
			PriorityLabel_t _plTag;
			if (F::PlayerUtils.GetTag(_sTag, &_plTag) && !_plTag.Label)
				vLabels.push_back({ _sTag, _plTag });
		}
		if (Utils::IsSteamFriend(friendsID))
		{
			auto& _plTag = mTags["Friend"];
			if (!_plTag.Label)
				vLabels.push_back({ "Friend", _plTag });
		}
	}
	if ((!iMode || iMode == 2) && !vLabels.size())
	{
		for (const auto& _sTag : G::PlayerTags[friendsID])
		{
			PriorityLabel_t _plTag;
			if (F::PlayerUtils.GetTag(_sTag, &_plTag) && _plTag.Label)
				vLabels.push_back({ _sTag, _plTag });
		}
		if (Utils::IsSteamFriend(friendsID))
		{
			auto& _plTag = mTags["Friend"];
			if (_plTag.Label)
				vLabels.push_back({ "Friend", _plTag });
		}
	}
	if (!vLabels.size())
		return false;

	std::sort(vLabels.begin(), vLabels.end(), [&](const auto& a, const auto& b) -> bool
		{
			// sort by priority if unequal
			if (a.second.Priority != b.second.Priority)
				return a.second.Priority > b.second.Priority;

			return a.first < b.first;
		});
	*sTag = vLabels.begin()->first;
	*plTag = vLabels.begin()->second;
	return true;
}
bool CPlayerlistUtils::GetSignificantTag(int iIndex, std::string* sTag, PriorityLabel_t* plTag, int iMode)
{
	if (const uint32_t friendsID = GetFriendsID(iIndex))
		return GetSignificantTag(friendsID, sTag, plTag, iMode);
	return false;
}

bool CPlayerlistUtils::IsIgnored(uint32_t friendsID)
{
	if (!friendsID)
		return false;

	const int iPriority = GetPriority(friendsID);
	const int iIgnored = mTags["Ignored"].Priority;
	return iPriority <= iIgnored;
}
bool CPlayerlistUtils::IsIgnored(int iIndex)
{
	if (const uint32_t friendsID = GetFriendsID(iIndex))
		return IsIgnored(friendsID);
	return false;
}

bool CPlayerlistUtils::IsFriend(int iIndex)
{
	if (const uint32_t friendsID = GetFriendsID(iIndex))
		return Utils::IsSteamFriend(friendsID);
	return false;
}



void CPlayerlistUtils::UpdatePlayers()
{
	static Timer updateTimer{};
	if (updateTimer.Run(1000))
	{
		std::lock_guard lock(mutex);
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