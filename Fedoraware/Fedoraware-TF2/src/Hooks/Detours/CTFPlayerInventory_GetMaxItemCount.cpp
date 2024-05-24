#include "../Hooks.h"

MAKE_HOOK(CTFPlayerInventory_GetMaxItemCount, S::CTFPlayerInventory_GetMaxItemCount(), int, __fastcall,
	void* ecx, void* edx)
{
	if (Vars::Misc::Automation::BackpackExpander.Value)
		return 3000;
	return Hook.Original<FN>()(ecx, edx);
}