#ifndef CPLAYERLISTUTILS_H
#define CPLAYERLISTUTILS_H

#include "../../Feature.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <string_view>

struct ListPlayer
{
	const char* Name{};
	uint32_t FriendsID{};
	int UserID{};
	int Team{};
	int Class{};
	bool Alive{};
	bool Local{};
	bool Friend{};
	bool Fake{};
};

struct PriorityLabel_t
{
	Color_t Color = { 255, 255, 255, 255 };
	int Priority = 0;

	bool Label = false;
	bool Assignable = true;
	bool Locked = false; // don't allow it to be removed
};

class CPlayerlistUtils : public Feature
{
public:
	bool GetTag(std::string_view sTag, PriorityLabel_t* plTag) const noexcept;

	void AddTag(uint32_t friendsID, std::string_view sTag, bool bSave = true, std::string_view sName = "");
	void AddTag(int iIndex, std::string_view sTag, bool bSave = true, std::string_view sName = "");
	void RemoveTag(uint32_t friendsID, std::string_view sTag, bool bSave = true, std::string_view sName = "");
	void RemoveTag(int iIndex, std::string_view sTag, bool bSave = true, std::string_view sName = "");
	bool HasTags(uint32_t friendsID) const noexcept;
	bool HasTags(int iIndex) const noexcept;
	bool HasTag(uint32_t friendsID, std::string_view sTag) const noexcept;
	bool HasTag(int iIndex, std::string_view sTag) const noexcept;

	int GetPriority(uint32_t friendsID) const noexcept;
	int GetPriority(int iIndex) const noexcept;
	bool GetSignificantTag(uint32_t friendsID, std::string_view* sTag, PriorityLabel_t* plTag, int iMode = 1) const noexcept;
	bool GetSignificantTag(int iIndex, std::string_view* sTag, PriorityLabel_t* plTag, int iMode = 1) const noexcept;
	bool IsIgnored(uint32_t friendsID) const noexcept;
	bool IsIgnored(int iIndex) const noexcept;
	bool IsFriend(int iIndex) const noexcept;

	void UpdatePlayers() override;

	const std::vector<const char*> vListPitch = { "None", "Up", "Down", "Zero", "Auto" };
	const std::vector<const char*> vListYaw = { "None", "Forward", "Backward", "Left", "Right", "Invert", "Edge", "Auto" };

	std::unordered_map<std::string, PriorityLabel_t> mTags = {
		{ "Default", { { 200, 200, 200, 255 }, 0, false, false, true } },
		{ "Ignored", { { 200, 200, 200, 255 }, -1, false, true, true } },
		{ "Cheater", { { 255, 100, 100, 255 }, 1, false, true, true } },
		{ "Friend", { { 100, 255, 100, 255 }, 0, true, false, true } }
	};

	std::vector<ListPlayer> vPlayerCache = {};
	std::unordered_map<uint32_t, ListPlayer> mPriorityCache = {};

	bool bLoadPlayers = true;
	bool bSavePlayers = false;
	bool bLoadTags = true;
	bool bSaveTags = false;

	mutable std::mutex mutex;
};

ADD_FEATURE(CPlayerlistUtils, PlayerUtils)

#endif //CPLAYERLISTUTILS_H
