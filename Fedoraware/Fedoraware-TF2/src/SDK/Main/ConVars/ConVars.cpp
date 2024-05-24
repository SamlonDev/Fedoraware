#include "ConVars.h"

void CConVars::Init()
{
	afkTimer = I::Cvar->FindVar("mp_idlemaxtime");

	ConCommandBase* cmdBase = I::Cvar->GetCommands();
	while (cmdBase != nullptr)
	{
		constexpr int FCVAR_HIDDEN = (int)EConVarFlags::FCVAR_HIDDEN;
		constexpr int FCVAR_DEVELOPMENT_ONLY = (int)EConVarFlags::FCVAR_DEVELOPMENT_ONLY;
		constexpr int FCVAR_CHEAT = (int)EConVarFlags::FCVAR_CHEAT;
		constexpr int FCVAR_NOT_CONNECTED = (int)EConVarFlags::FCVAR_NOT_CONNECTED;
		cmdBase->m_nFlags &= ~(FCVAR_HIDDEN | FCVAR_DEVELOPMENT_ONLY | FCVAR_CHEAT | FCVAR_NOT_CONNECTED);

		cmdBase = cmdBase->m_pNext;
	}
}

ConVar* CConVars::FindVar(const char* cvarname)
{
	if (!cvarMap.contains(FNV1A::HashConst(cvarname)))
		cvarMap[FNV1A::HashConst(cvarname)] = I::Cvar->FindVar(cvarname);
	return cvarMap[FNV1A::HashConst(cvarname)];
}