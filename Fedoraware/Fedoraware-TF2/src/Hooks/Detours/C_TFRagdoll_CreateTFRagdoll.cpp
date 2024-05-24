#include "../Hooks.h"

#define Offset(type, ent, offset) *reinterpret_cast<type>(ent + offset)

void ClearEffects(CBaseEntity* pEntity)
{
	Offset(bool*, pEntity, 0xC91) = false; // Gib
	Offset(bool*, pEntity, 0xC92) = false; // Burning
	Offset(bool*, pEntity, 0xC93) = false; // Electrocuted
	Offset(bool*, pEntity, 0xC99) = false; // Become ash
	Offset(bool*, pEntity, 0xCA0) = false; // Gold
	Offset(bool*, pEntity, 0xCA1) = false; // Ice
}

MAKE_HOOK(C_TFRagdoll_CreateTFRagdoll, S::CTFRagdoll_CreateTFRagdoll(), void, __fastcall,
	void* ecx, void* edx)
{
	if (Vars::Visuals::Ragdolls::NoRagdolls.Value)
		return;

	if (const auto& pEntity = static_cast<CBaseEntity*>(ecx))
	{
		bool bValid = Vars::Visuals::Ragdolls::Active.Value;

		if (bValid && Vars::Visuals::Ragdolls::EnemyOnly.Value)
		{
			if (const auto& pLocal = g_EntityCache.GetLocal())
			{
				if (Offset(int*, pEntity, 0xCBC) == pLocal->m_iTeamNum()) // Team offset
					bValid = false;
			}
		}

		if (bValid)
		{
			ClearEffects(pEntity);

			Offset(bool*, pEntity, 0xC92) = Vars::Visuals::Ragdolls::Effects.Value & (1 << 0);
			Offset(bool*, pEntity, 0xC93) = Vars::Visuals::Ragdolls::Effects.Value & (1 << 1);
			Offset(bool*, pEntity, 0xC99) = Vars::Visuals::Ragdolls::Effects.Value & (1 << 2);
			Offset(bool*, pEntity, 0xC95) = Vars::Visuals::Ragdolls::Effects.Value & (1 << 3);
			Offset(bool*, pEntity, 0xCA0) = Vars::Visuals::Ragdolls::Type.Value == 1;
			Offset(bool*, pEntity, 0xCA1) = Vars::Visuals::Ragdolls::Type.Value == 2;

			pEntity->m_vecForce() *= Vars::Visuals::Ragdolls::Force.Value;
			pEntity->m_vecForce().x *= Vars::Visuals::Ragdolls::ForceHorizontal.Value;
			pEntity->m_vecForce().y *= Vars::Visuals::Ragdolls::ForceHorizontal.Value;
			pEntity->m_vecForce().z *= Vars::Visuals::Ragdolls::ForceVertical.Value;
		}
	}

	Hook.Original<FN>()(ecx, edx);
}