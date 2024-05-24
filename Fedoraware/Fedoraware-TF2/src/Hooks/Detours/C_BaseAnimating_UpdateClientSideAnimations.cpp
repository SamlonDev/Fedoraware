#include "../Hooks.h"

MAKE_HOOK(CBaseAnimating_UpdateClientSideAnimation, S::CBaseAnimating_UpdateClientSideAnimation(), void, __fastcall,
	void* ecx, void* edx)
{
	auto pEntity = reinterpret_cast<CBaseEntity*>(ecx);
	if (!G::UpdatingAnims && (pEntity == g_EntityCache.GetLocal() ? !pEntity->IsInBumperKart() : true))
		return;

	G::AnimateKart = true;
	Hook.Original<FN>()(ecx, edx);
	G::AnimateKart = false;
}