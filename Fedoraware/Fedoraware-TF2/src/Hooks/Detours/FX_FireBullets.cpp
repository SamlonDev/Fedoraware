#include "../Hooks.h"
#include "../../Features/Backtrack/Backtrack.h"
#include "../../Features/Resolver/Resolver.h"
#include"../../Features/NoSpread/NoSpreadHitscan/NoSpreadHitscan.h"

namespace S
{
	MAKE_SIGNATURE(C_TFWeaponBaseGun_FireBullet_Call, CLIENT_DLL, "83 C4 ? 5F 5E 5B 8B E5 5D C2 ? ? CC CC CC CC 53", 0x0);
}

MAKE_HOOK(FX_FireBullets, S::FX_FireBullets(), void, __cdecl,
	void* pWpn, int iPlayer, const Vec3& vecOrigin, const Vec3& vecAngles, int iWeapon, int iMode, int iSeed, float flSpread, float flDamage, bool bCritical)
{
	static auto dwFireBulletCall = S::C_TFWeaponBaseGun_FireBullet_Call();
	const auto dwRetAddr = reinterpret_cast<DWORD>(_ReturnAddress());

	if (iPlayer != I::EngineClient->GetLocalPlayer())
	{
		//Utils::ConLog("FX_FireBullets", tfm::format("%d {%.1f, %.1f, %.1f}", iPlayer, vecAngles.x, vecAngles.y, vecAngles.z).c_str(), {0, 222, 255, 255});
		F::Backtrack.ReportShot(iPlayer);
		F::Resolver.FXFireBullet(iPlayer, vecAngles);
	}

	if (dwRetAddr != dwFireBulletCall)
		return;

	if (Vars::Aimbot::General::NoSpread.Value)
		iSeed = F::NoSpreadHitscan.iSeed;

	return Hook.Original<FN>()(pWpn, iPlayer, vecOrigin, vecAngles, iWeapon, iMode, iSeed, flSpread, flDamage, bCritical);
}