#include "../Hooks.h"

#include "../../Features/NetworkFix/NetworkFix.h"

MAKE_HOOK(CL_ReadPackets, S::CL_ReadPackets(), void, __cdecl,
	bool bFinalTick)
{
	if (F::NetworkFix.ShouldReadPackets())
		Hook.Original<FN>()(bFinalTick);
}