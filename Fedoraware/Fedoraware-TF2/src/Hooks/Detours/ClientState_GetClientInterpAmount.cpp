#include "../Hooks.h"
#include "../../Features/Vars.h"

MAKE_HOOK(ClientState_GetClientInterpAmount, S::ClientState_GetClientInterpAmount(), float, __fastcall,
	void* ecx, void* edx)
{
	G::LerpTime = Hook.Original<FN>()(ecx, edx);
	return Vars::Visuals::Removals::Interpolation.Value ? 0.f : G::LerpTime;
}