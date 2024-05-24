#include "../Hooks.h"

namespace S
{
	MAKE_SIGNATURE(CTFRagdoll_CreateTFRagdoll_CTFPlayer_BRenderAsZombie_Call, CLIENT_DLL, "E8 ? ? ? ? 84 C0 74 07 C6 87 ? ? ? ? ? 8B 06 8B CE 6A 00 FF 90 ? ? ? ? 84 DB", 0x5);
}

MAKE_HOOK(C_TFPlayer_BRenderAsZombie, S::CTFPlayer_BRenderAsZombie(), bool, __fastcall,
	void* ecx, void* edx, bool bWeaponsCheck)
{
	static auto dwCall = S::CTFRagdoll_CreateTFRagdoll_CTFPlayer_BRenderAsZombie_Call();
	const auto dwRetAddr = reinterpret_cast<DWORD>(_ReturnAddress());

	if (Vars::Visuals::Ragdolls::NoGib.Value && dwRetAddr == dwCall)
		return true;
	
	return Hook.Original<FN>()(ecx, edx, bWeaponsCheck);
}