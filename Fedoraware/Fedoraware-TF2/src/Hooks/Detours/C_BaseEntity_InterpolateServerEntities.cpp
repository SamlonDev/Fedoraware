#include "../Hooks.h"

MAKE_HOOK(C_BaseEntity_InterpolateServerEntities, S::CBaseEntity_InterpolateServerEntities(), void, __fastcall,
	void* ecx, void* edx)
{
	if (const auto& pLocal = g_EntityCache.GetLocal())
	{
		if (G::Recharge && !I::Input->CAM_IsThirdPerson() && pLocal->IsAlive())
			return;
	}

	return Hook.Original<FN>()(ecx, edx);
}