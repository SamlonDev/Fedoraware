#include "../Hooks.h"

MAKE_HOOK(CInventoryManager_ShowItemsPickedUp, S::CInventoryManager_ShowItemsPickedUp(), bool, __fastcall,
	void* ecx, void* edx, bool bForce, bool bReturnToGame, bool bNoPanel)
{
	if (Vars::Misc::Automation::AcceptItemDrops.Value)
	{
		Hook.Original<FN>()(ecx, edx, true, true, true);
		return false;
	}
	return Hook.Original<FN>()(ecx, edx, bForce, bReturnToGame, bNoPanel);
}