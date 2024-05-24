#include "../Hooks.h"
#include "../../SDK/Main/CEconNotification.h"

namespace S
{
	MAKE_SIGNATURE(CTFPlayer_OnHasNewItems_NotificationQueue_Add_Call, CLIENT_DLL, "83 C4 ? 5E C3 CC CC CC CC CC CC CC 56 8B F1 E8", 0x0);
}

MAKE_HOOK(NotificationQueue_Add, S::NotificationQueue_Add(), int, __cdecl,
	CEconNotification* pNotification)
{
	static const auto dwOnHasNewItems = S::CTFPlayer_OnHasNewItems_NotificationQueue_Add_Call();
	const auto dwRetAddr = reinterpret_cast<DWORD>(_ReturnAddress());

	if (Vars::Misc::Automation::AcceptItemDrops.Value && dwRetAddr == dwOnHasNewItems)
	{
		pNotification->Accept();
		pNotification->Trigger();
		pNotification->UpdateTick();
		pNotification->MarkForDeletion();
		return 0;
	}

	return Hook.Original<FN>()(pNotification);
}