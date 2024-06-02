#include "../Hooks.h"

// Using statements to reduce verbosity
using DATA_CENTER_MAP = std::unordered_map<std::string, unsigned>;
using DATA_CENTER_ITERATOR = DATA_CENTER_MAP::const_iterator;
using FN = int(__fastcall*)(void*, void*, SteamNetworkingPOPID);

const DATA_CENTER_MAP DATA_CENTER_HASH
{
	{"ams", DC_AMS},
	{"fra", DC_FRA},
	{"lhr", DC_LHR},
	{"mad", DC_MAD},
	{"par", DC_PAR},
	{"sto", DC_STO},
	{"sto2", DC_STO},
	{"vie", DC_VIE},
	{"waw", DC_WAW},

	{"atl", DC_ATL},
	{"ord", DC_ORD},
	{"dfw", DC_DFW},
	{"lax", DC_LAX},
	{"eat", DC_EAT},
	{"sea", DC_SEA},
	{"iad", DC_IAD},

	{"eze", DC_EZE},
	{"lim", DC_LIM},
	{"scl", DC_SCL},
	{"gru", DC_GRU},

	{"maa", DC_MAA},
	{"bom", DC_BOM},
	{"dxb", DC_DXB},
	{"hkg", DC_HKG},
	{"seo", DC_SEO},
	{"sgp", DC_SGP},
	{"tyo", DC_TYO},
	{"tyo1", DC_TYO},
	{"tyo2", DC_TYO},
	{"can", DC_CAN},
	{"sha", DC_SHA},
	{"tsn", DC_TSN},

	{"jnb", DC_JNB},

	{"syd", DC_SYD},
};

void POPID_ToString(SteamNetworkingPOPID popID, char* out) noexcept
{
	out[0] = static_cast<char>(popID >> 16);
	out[1] = static_cast<char>(popID >> 8);
	out[2] = static_cast<char>(popID);
	out[3] = static_cast<char>(popID >> 24);
	out[4] = 0;
}

int ISteamNetworkingUtils_GetDirectPingToPOP_Hook(void* ecx, void* edx, SteamNetworkingPOPID popID) __fastcall restrict override
{
	if (!ecx || !edx)
	{
		return Hook.Original<FN>()(ecx, edx, popID);
	}

	if (!Vars::Misc::Queueing::ForceRegions.Value)
	{
		return Hook.Original<FN>()(ecx, edx, popID);
	}

	char popIDName[5];
	POPID_ToString(popID, popIDName);

	const DATA_CENTER_ITERATOR pos = DATA_CENTER_HASH.find(popIDName);
	if (pos == DATA_CENTER_HASH.end())
	{
		return Hook.Original<FN>()(ecx, edx, popID);
	}

	const bool isAllowed = Vars::Misc::Queueing::ForceRegions.Value & pos->second;
	return isAllowed ? 1 : 999999;
}
