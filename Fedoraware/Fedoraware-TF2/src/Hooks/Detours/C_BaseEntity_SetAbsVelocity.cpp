#include "../Hooks.h"
#include <intrin.h>
#include "../../Features/Backtrack/Backtrack.h"

namespace S
{
	MAKE_SIGNATURE(CBasePlayer_PostDataUpdate_SetAbsVelocityCall, CLIENT_DLL, "E8 ? ? ? ? 53 8B CF E8 ? ? ? ? 8D 47 F8 39 05", 0x5);
}

MAKE_HOOK(C_BaseEntity_SetAbsVelocity, S::CBaseEntity_SetAbsVelocity(), void, __fastcall,
	void* ecx, void* edx, const Vec3& vecAbsVelocity)
{
	static const auto dwSetAbsVelocityCall = S::CBasePlayer_PostDataUpdate_SetAbsVelocityCall();
	const auto dwRetAddr = reinterpret_cast<DWORD>(_ReturnAddress());

	if (dwRetAddr == dwSetAbsVelocityCall)
	{
		const auto pPlayer = static_cast<CBaseEntity*>(ecx);
		if (pPlayer && G::VelFixRecords.contains(pPlayer))
		{
			const auto newRecord = VelFixRecord(pPlayer->m_vecOrigin(), pPlayer->m_vecMaxs().z - pPlayer->m_vecMins().z, pPlayer->m_flSimulationTime());
			const auto& oldRecord = G::VelFixRecords[pPlayer];

			const float flDelta = newRecord.m_flSimulationTime - oldRecord.m_flSimulationTime;
			const Vec3 vDelta = newRecord.m_vecOrigin - oldRecord.m_vecOrigin;

			static auto sv_lagcompensation_teleport_dist = g_ConVars.FindVar("sv_lagcompensation_teleport_dist");
			const float flDist = powf(sv_lagcompensation_teleport_dist ? sv_lagcompensation_teleport_dist->GetFloat() : 64.f, 2.f) * TIME_TO_TICKS(flDelta);
			if (flDelta > 0.f && vDelta.Length2DSqr() < flDist)
			{
				Vec3 vOldOrigin = oldRecord.m_vecOrigin;
				if (!pPlayer->IsOnGround())
					vOldOrigin.z += oldRecord.m_flHeight - newRecord.m_flHeight;
				Hook.Original<FN>()(ecx, edx, (newRecord.m_vecOrigin - vOldOrigin) / flDelta);
			}
			return;
		}
	}

	Hook.Original<FN>()(ecx, edx, vecAbsVelocity);
}