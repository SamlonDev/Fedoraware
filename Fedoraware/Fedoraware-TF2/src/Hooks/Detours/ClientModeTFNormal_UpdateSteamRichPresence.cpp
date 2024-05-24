#include "../Hooks.h"

#include "../../Features/Misc/Misc.h"

MAKE_HOOK(ClientModeTFNormal_UpdateSteamRichPresence, S::ClientModeTFNormal_UpdateSteamRichPresence(), void, __fastcall,
	void* ecx, void* edx)
{
	F::Misc.SteamRPC();
}