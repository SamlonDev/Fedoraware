#include "../Hooks.h"

#include "../../SDK/Main/CEconNotification.h"

MAKE_HOOK(CEconNotification_HasNewItems, S::CEconNotification_HasNewItems(), CEconNotification*, __fastcall,
	void* ecx, void* edx)
{
	return Hook.Original<FN>()(ecx, edx);
}