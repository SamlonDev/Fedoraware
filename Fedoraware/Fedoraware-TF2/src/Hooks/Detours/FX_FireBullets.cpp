#include "../Hooks.h"
#include "../../Features/Backtrack/Backtrack.h"
#include "../../Features/Resolver/Resolver.h"
#include "../../Features/NoSpread/NoSpreadHitscan/NoSpreadHitscan.h"

namespace S
{
	constexpr auto C_TFWeaponBaseGun_FireBullet_Signatur = Hooks::Signature("83 C4 ? 5F 5E 5B 8B E5 5D C2 ? ? CC CC CC CC 53");
}

void __fastcall FX_FireBullets_Hook(void* pEcx, void* , int iPlayer, const Vec3& vecOrigin, const Vec3& vecAngles, int iWeapon, int iMode, int iSeed, float flSpread, float flDamage, bool bCritical)
{
	if (iPlayer != I::EngineClient->GetLocalPlayer())
	{
		F::Backtrack.ReportShot(iPlayer);
		F::Resolver.FXFireBullet(iPlayer, vecAngles);
	}

	if (reinterpret_cast<uintptr_t>(_ReturnAddress()) != reinterpret_cast<uintptr_t>(S::C_TFWeaponBaseGun_FireBullet_Signatur))
		return;

	if (Vars::Aimbot::General::NoSpread.Value)
		iSeed = F::NoSpreadHitscan.iSeed;

	return Hook.Original<FN>()(pEcx, iPlayer, vecOrigin, vecAngles, iWeapon, iMode, iSeed, flSpread, flDamage, bCritical);
}
