#include "ConVars.h"

void CConVars::Init()
{
	afkTimer = I::Cvar->FindVar("mp_idlemaxtime");

	if (!afkTimer) {
		// Handle error
	}

	using FlagType = EConVarFlags;
	constexpr FlagType HIDDEN = FlagType::FCVAR_HIDDEN;
	constexpr FlagType DEVELOPMENT_ONLY = FlagType::FCVAR_DEVELOPMENT_ONLY;
	constexpr FlagType CHEAT = FlagType::FCVAR_CHEAT;
	constexpr FlagType NOT_CONNECTED = FlagType::FCVAR_NOT_CONNECTED;

	for (ConCommandBase* cmdBase = I::Cvar->GetCommands(); cmdBase != nullptr; cmdBase = cmdBase->m_pNext) {
		cmdBase->m_nFlags &= ~(HIDDEN | DEVELOPMENT_ONLY | CHEAT | NOT_CONNECTED);
	}
}

using ConVarPtr = ConVar*;

ConVarPtr CConVars::FindVar(const char* cvarname) const
{
	const uint64_t hash = FNV1A::HashConst(cvarname);
	if (!cvarMap.contains(hash)) {
		cvarMap[hash] = I::Cvar->FindVar(cvarname);
		if (!cvarMap[hash]) {
			// Handle error
		}
	}
	return cvarMap[hash];
}
