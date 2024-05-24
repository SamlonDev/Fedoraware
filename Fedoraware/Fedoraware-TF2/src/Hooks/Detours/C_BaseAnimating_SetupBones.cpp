#include "../Hooks.h"

#include "../../Features/Backtrack/Backtrack.h"

MAKE_HOOK(C_BaseAnimating_SetupBones, S::CBaseAnimating_SetupBones(), bool, __fastcall,
	void* ecx, void* edx, matrix3x4* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
{
	if (Vars::Misc::Game::SetupBonesOptimization.Value && !F::Backtrack.bSettingUpBones)
	{
		auto base_ent = reinterpret_cast<CBaseEntity*>(reinterpret_cast<uintptr_t>(ecx) - 0x4);
		if (base_ent)
		{
			auto GetRootMoveParent = [&]()
			{
				auto pEntity = base_ent;
				auto pParent = base_ent->GetMoveParent();

				int i = 0;
				while (pParent)
				{
					if (i > 32) //XD
						break;
					i++;

					pEntity = pParent;
					pParent = pEntity->GetMoveParent();
				}

				return pEntity;
			};

			auto owner = GetRootMoveParent();
			auto ent = owner ? owner : base_ent;
			if (ent->GetClassID() == ETFClassID::CTFPlayer && ent != g_EntityCache.GetLocal())
			{
				if (pBoneToWorldOut)
				{
					auto bones{ ent->GetCachedBoneData() };
					if (bones)
						std::memcpy(pBoneToWorldOut, bones->Base(), sizeof(matrix3x4) * std::min(nMaxBones, bones->Count()));
				}

				return true;
			}
		}
	}

	return Hook.Original<FN>()(ecx, edx, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
}