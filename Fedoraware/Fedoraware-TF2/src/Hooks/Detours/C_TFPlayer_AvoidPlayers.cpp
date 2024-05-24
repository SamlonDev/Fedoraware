#include "../Hooks.h"

MAKE_HOOK(C_TFPlayer_AvoidPlayers, S::CTFPlayer_AvoidPlayers(), void, __fastcall,
	void* ecx, void* edx, CUserCmd* pCmd)
{
	if (Vars::Misc::Movement::NoPush.Value)
		return;

	Hook.Original<FN>()(ecx, edx, pCmd);
}