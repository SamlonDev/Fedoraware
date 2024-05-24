#include "../Hooks.h"

namespace S
{
	MAKE_SIGNATURE(KeyValues_SetInt_Desired, CLIENT_DLL, "8B 03 8B CB 56 57 FF 90 ? ? ? ? 6A", 0x0);
	MAKE_SIGNATURE(KeyValues_SetInt_Jump, CLIENT_DLL, "89 45 BC E8 ? ? ? ? 3B C7 75 1D 80 7D F8 00 75 17 8B 4D C0", 0x0);
}

MAKE_HOOK(KeyValues_SetInt, S::KeyValues_SetInt(), void, __fastcall,
	void* ecx, void* edx, const char* szKeyName, int iValue)
{
	static DWORD dwDesired = S::KeyValues_SetInt_Desired();
	static DWORD dwJump = S::KeyValues_SetInt_Jump();
	const auto dwRetAddr = reinterpret_cast<DWORD>(_ReturnAddress());

	Hook.Original<FN>()(ecx, edx, szKeyName, iValue);

	if (!Vars::Visuals::UI::RevealScoreboard.Value) // Vars::Visuals::UI::CleanScreenshots.Value ineffective, doesn't update in time
		return;

	/* Scoreboard class reveal */
	if (dwRetAddr == dwDesired && std::string_view(szKeyName).find("nemesis") != std::string_view::npos)
		*static_cast<uintptr_t*>(_AddressOfReturnAddress()) = dwJump;
}