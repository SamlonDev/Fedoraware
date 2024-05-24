#include "../Hooks.h"

MAKE_HOOK(DSP_Process, S::DSP_Process(), void, __cdecl,
	unsigned int idsp, int* pbfront, int* pbrear, int* pbcenter, int sampleCount)
{
	if (!Vars::Visuals::Removals::DSP.Value)
		Hook.Original<FN>()(idsp, pbfront, pbrear, pbcenter, sampleCount);
}