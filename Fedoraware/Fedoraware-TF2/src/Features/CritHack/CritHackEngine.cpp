// attemtped to use game api, seems inconsistent

#include "CritHack.h"
#include "../TickHandler/TickHandler.h"

#define TF_DAMAGE_CRIT_MULTIPLIER		3.0f
#define TF_DAMAGE_CRIT_CHANCE			0.02f
#define TF_DAMAGE_CRIT_CHANCE_RAPID		0.02f
#define TF_DAMAGE_CRIT_DURATION_RAPID	2.0f
#define TF_DAMAGE_CRIT_CHANCE_MELEE		0.15f

bool CCritHack::IsEnabled()
{
	if (!Vars::CritHack::Active.Value || !I::EngineClient->IsInGame())
		return false;
	static auto tf_weapon_criticals = g_ConVars.FindVar("tf_weapon_criticals");
	if (tf_weapon_criticals && !tf_weapon_criticals->GetBool())
		return false;

	return true;
}



void CCritHack::Fill(CBaseCombatWeapon* pWeapon, const CUserCmd* pCmd, const bool bAttacking, int n)
{
	static int prev_weapon = 0;
	static int previousCrit = 0;
	static int starting_num = pCmd->command_number;

	//if (bAttacking && G::CanPrimaryAttack/* || pCmd->buttons & IN_ATTACK*/)
	//	return;

	if (prev_weapon != pWeapon->GetIndex())
	{
		starting_num = pCmd->command_number;
		prev_weapon = pWeapon->GetIndex();
		ForceCmds.clear();
		SkipCmds.clear();
	}

	for (int i = 0; i < n; i++)
	{
		if (ForceCmds.size() >= 15)
			break;

		const int cmd_num = starting_num + i;

		if (IsCritCommand(cmd_num))
			ForceCmds.push_back(cmd_num);
	}
	for (int i = 0; i < n; i++)
	{
		if (SkipCmds.size() >= 15)
			break;

		const int cmd_num = starting_num + i;

		if (IsCritCommand(cmd_num, false))
			SkipCmds.push_back(cmd_num);
	}
	starting_num += n;
}

int CCritHack::LastGoodCritTick(const CUserCmd* pCmd)
{
	int retVal = -1;
	bool popBack = false;

	for (const auto& cmd : ForceCmds)
	{
		if (cmd >= pCmd->command_number)
			retVal = cmd;
		else
			popBack = true;
	}

	if (popBack)
		ForceCmds.pop_front();

	return retVal;
}

int CCritHack::LastGoodSkipTick(const CUserCmd* pCmd)
{
	int retVal = -1;
	bool popBack = false;

	for (const auto& cmd : SkipCmds)
	{
		if (cmd >= pCmd->command_number)
			retVal = cmd;
		else
			popBack = true;
	}

	if (popBack)
		SkipCmds.pop_front();

	return retVal;
}



bool CCritHack::IsCritCommand(const i32 command_number, const bool crit)
{
	const auto& pWeapon = g_EntityCache.GetWeapon();
	if (!pWeapon)
		return false;

	const auto random_seed = MD5_PseudoRandom(command_number) & 0x7FFFFFFF;
	Utils::RandomSeed(DecryptOrEncryptSeed(pWeapon, random_seed));
	const auto random_int = Utils::RandomInt(0, 9999);

	//return crit ? random_int < 100 : random_int > 6000;
	return crit ? random_int < 50 : random_int > 8000;
}

u32 CCritHack::DecryptOrEncryptSeed(CBaseCombatWeapon* pWeapon, u32 seed)
{
	if (!pWeapon)
		return 0;

	unsigned int iMask = pWeapon->GetIndex() << 8 | I::EngineClient->GetLocalPlayer();

	if (pWeapon->GetSlot() == SLOT_MELEE)
		iMask <<= 8;

	return iMask ^ seed;
}



void CCritHack::GetTotalCrits(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	if (!Vars::CritHack::Active.Value)
		return;

	const int slot = pWeapon->GetSlot();
	auto tfWeaponInfo = pWeapon->GetTFWeaponInfo();
	if (!tfWeaponInfo)
		return;
	auto& weaponData = tfWeaponInfo->GetWeaponData(0);

	float flDamage = weaponData.m_nDamage;
	flDamage = Utils::AttribHookValue(flDamage, "mult_dmg", pWeapon);
	int nProjectilesPerShot = weaponData.m_nBulletsPerShot;
	if (nProjectilesPerShot >= 1)
		nProjectilesPerShot = Utils::AttribHookValue(nProjectilesPerShot, "mult_bullets_per_shot", pWeapon);
	else
		nProjectilesPerShot = 1;
	Storage[slot].Damage = flDamage *= nProjectilesPerShot;

	if (pWeapon->IsRapidFire())
	{
		flDamage *= TF_DAMAGE_CRIT_DURATION_RAPID / weaponData.m_flTimeFireDelay;

		if (flDamage * TF_DAMAGE_CRIT_MULTIPLIER > BucketCap)
			flDamage = BucketCap / TF_DAMAGE_CRIT_MULTIPLIER;
	}

	float flMult = slot == SLOT_MELEE ? 0.5f : Math::RemapValClamped((float)(pWeapon->CritSeedRequests() + 1) / (float)(pWeapon->CritChecks() + 1), 0.1f, 1.f, 1.f, 3.f);
	Storage[slot].Cost = flDamage * TF_DAMAGE_CRIT_MULTIPLIER * flMult;

	if (BucketCap)
		Storage[slot].PotentialCrits = static_cast<unsigned int>((BucketCap - Storage[slot].Damage) / (3 * flDamage / (slot == SLOT_MELEE ? 2 : 1) - Storage[slot].Damage));

	int iCrits = 0;
	{
		int shots = pWeapon->CritChecks(), crits = pWeapon->CritSeedRequests();
		float bucket = pWeapon->CritTokenBucket(), flCost = flDamage * TF_DAMAGE_CRIT_MULTIPLIER;
		const int iAttempts = std::min(Storage[slot].PotentialCrits + 1, 100);
		for (int i = 0; i < iAttempts; i++)
		{
			shots++; crits++;

			flMult = slot == SLOT_MELEE ? 0.5f : Math::RemapValClamped((float)crits / (float)shots, 0.1f, 1.f, 1.f, 3.f);
			bucket = std::min(bucket + Storage[slot].Damage, BucketCap) - flCost * flMult;
			if (bucket < 0.f)
				break;

			iCrits++;
		}
	}

	if (iCrits <= 0)
	{
		iCrits = 0;

		int shots = pWeapon->CritChecks() + 1, crits = pWeapon->CritSeedRequests() + 1;
		float bucket = std::min(pWeapon->CritTokenBucket() + Storage[slot].Damage, BucketCap), flCost = flDamage * TF_DAMAGE_CRIT_MULTIPLIER;
		for (int i = 0; i < 100; i++)
		{
			iCrits--;
			if (!pWeapon->IsRapidFire() || !(i % int(weaponData.m_flTimeFireDelay / I::GlobalVars->interval_per_tick)))
				shots++;

			flMult = slot == SLOT_MELEE ? 0.5f : Math::RemapValClamped((float)crits / (float)shots, 0.1f, 1.f, 1.f, 3.f);
			bucket = std::min(bucket + Storage[slot].Damage, BucketCap);
			if (bucket >= flCost * flMult)
				break;
		}
	}

	Storage[slot].AvailableCrits = iCrits;
}

void CCritHack::CanFireCriticalShotHandler(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	CritBanned = false;

	CritChance = 0.02f * pLocal->GetCritMult();
	if (pWeapon->IsRapidFire())
	{
		float flNonCritDuration = (TF_DAMAGE_CRIT_DURATION_RAPID / CritChance) - TF_DAMAGE_CRIT_DURATION_RAPID;
		CritChance = 1.f / flNonCritDuration;
	}
	CritChance += 0.1f;

	if (CritDamage == 0.f || pWeapon->GetSlot() == SLOT_MELEE)
		return;

	const auto divCritDamage = CritDamage / TF_DAMAGE_CRIT_MULTIPLIER;
	const auto ratio = divCritDamage / (AllDamage - 2 * divCritDamage);

	CritBanned = ratio >= CritChance;
}

void CCritHack::GetDamageTilUnban(CBaseEntity* pLocal)
{
	DamageTilUnban = 0;

	if (!CritBanned)
		return;

	const auto divCritDamage = CritDamage / TF_DAMAGE_CRIT_MULTIPLIER;

	DamageTilUnban = divCritDamage / CritChance + 2 * divCritDamage - AllDamage;
}

bool CCritHack::WeaponCanCrit(CBaseCombatWeapon* pWeapon)
{
	if (Utils::AttribHookValue(1.f, "mult_crit_chance", pWeapon) <= 0.f)
		return false;

	switch (pWeapon->GetWeaponID())
	{
	case TF_WEAPON_LUNCHBOX:
	case TF_WEAPON_JAR_MILK:
	case TF_WEAPON_BUFF_ITEM:
	case TF_WEAPON_FLAME_BALL:
	case TF_WEAPON_JAR_GAS:
	case TF_WEAPON_ROCKETPACK:
	case TF_WEAPON_LASER_POINTER:
	case TF_WEAPON_MEDIGUN:
	case TF_WEAPON_SNIPERRIFLE:
	case TF_WEAPON_SNIPERRIFLE_DECAP:
	case TF_WEAPON_SNIPERRIFLE_CLASSIC:
	case TF_WEAPON_COMPOUND_BOW:
	case TF_WEAPON_JAR:
	case TF_WEAPON_KNIFE:
	case TF_WEAPON_PDA_SPY:
	case TF_WEAPON_PDA_SPY_BUILD:
	case TF_WEAPON_PDA:
	case TF_WEAPON_PDA_ENGINEER_BUILD:
	case TF_WEAPON_PDA_ENGINEER_DESTROY:
	case TF_WEAPON_BUILDER:
		return false;
	}

	return true;
}



void CCritHack::ResetWeapon(CBaseCombatWeapon* pWeapon)
{
	if (!pWeapon)
		return;

	const auto slot = pWeapon->GetSlot();
	const auto index = pWeapon->m_iItemDefinitionIndex();
	if (Storage[slot].DefIndex == index)
		return;

	Utils::ConLog("Crithack", "Resetting weapon", { 0, 255, 255, 255 }, Vars::Debug::Logging.Value);

	Storage[slot] = {};
	Storage[slot].DefIndex = index;
}

void CCritHack::Reset()
{
	Utils::ConLog("Crithack", "Resetting all", { 0, 255, 255, 255 }, Vars::Debug::Logging.Value);

	ForceCmds = {};
	SkipCmds = {};

	Storage = {};

	CritDamage = 0;
	AllDamage = 0;

	CritBanned = false;
	DamageTilUnban = 0;
	CritChance = 0.f;
	ProtectData = false;
}



void CCritHack::Run(CUserCmd* pCmd)
{
	if (!IsEnabled())
		return;

	auto bucketCap = g_ConVars.FindVar("tf_weapon_criticals_bucket_cap");
	BucketCap = bucketCap ? bucketCap->GetFloat() : 1000.f;

	const auto& pLocal = g_EntityCache.GetLocal();
	const auto& pWeapon = g_EntityCache.GetWeapon();
	if (!pLocal || !pWeapon || !pLocal->IsAlive() || pLocal->IsCritBoosted() || !WeaponCanCrit(pWeapon))
		return;

	ResetWeapon(pWeapon);
	if (pWeapon->GetWeaponID() == TF_WEAPON_MINIGUN && pCmd->buttons & IN_ATTACK)
		pCmd->buttons &= ~IN_ATTACK2;

	bool bAttacking = G::IsAttacking;
	if (G::CurWeaponType == EWeaponType::MELEE)
	{
		bAttacking = G::CanPrimaryAttack && pCmd->buttons & IN_ATTACK;
		if (!bAttacking && pWeapon->GetWeaponID() == TF_WEAPON_FISTS)
			bAttacking = G::CanPrimaryAttack && pCmd->buttons & IN_ATTACK2;
	}
	if (pWeapon->GetWeaponID() == TF_WEAPON_PIPEBOMBLAUNCHER || pWeapon->GetWeaponID() == TF_WEAPON_STICKY_BALL_LAUNCHER || pWeapon->GetWeaponID() == TF_WEAPON_GRENADE_STICKY_BALL
		|| pWeapon->GetWeaponID() == TF_WEAPON_CANNON)
	{
		const float flChargeS = pWeapon->m_flChargeBeginTime() > 0.f ? I::GlobalVars->curtime - pWeapon->m_flChargeBeginTime() : 0.f;
		const float flChargeC = pWeapon->m_flDetonateTime() > 0.f ? pWeapon->m_flDetonateTime() - I::GlobalVars->curtime : 1.f;
		const float flAmount = pWeapon->GetWeaponID() != TF_WEAPON_CANNON
			? Math::RemapValClamped(flChargeS, 0.f, Utils::AttribHookValue(4.f, "stickybomb_charge_rate", pWeapon), 0.f, 1.f)
			: 1.f - Math::RemapValClamped(flChargeC, 0.f, Utils::AttribHookValue(0.f, "grenade_launcher_mortar_mode", pWeapon), 0.f, 1.f);

		const bool bUnheld = !(pCmd->buttons & IN_ATTACK) && flAmount > 0.f;
		const bool bSwapping = pCmd->weaponselect;
		const bool bFull = flAmount >= 1.f; // possibly add exception to skip when full with cannon

		bAttacking = (bUnheld || bFull) && !bSwapping;
	}
	if ((pWeapon->GetWeaponID() == TF_WEAPON_MINIGUN || pWeapon->GetWeaponID() == TF_WEAPON_FLAMETHROWER) && !(G::LastUserCmd->buttons & IN_ATTACK)) // silly
		bAttacking = false;

	GetTotalCrits(pLocal, pWeapon);
	CanFireCriticalShotHandler(pLocal, pWeapon);
	GetDamageTilUnban(pLocal);
	Fill(pWeapon, pCmd, bAttacking, 15);

	const int closestCrit = LastGoodCritTick(pCmd), closestSkip = LastGoodSkipTick(pCmd);
	const bool bPressed = F::KeyHandler.Down(Vars::CritHack::CritKey.Value) || Vars::CritHack::AlwaysMelee.Value && pWeapon->GetSlot() == SLOT_MELEE;
	if (bAttacking && !pWeapon->IsInReload())
	{
		const bool bWait = pWeapon->IsRapidFire() && pWeapon->LastRapidfireCritCheckTime() + 1.f > I::GlobalVars->interval_per_tick * pLocal->m_nTickBase();

		if (bPressed && Storage[pWeapon->GetSlot()].AvailableCrits > 0 && (!CritBanned || pWeapon->GetSlot() == SLOT_MELEE) && closestCrit >= 0 && !bWait)
			pCmd->command_number = closestCrit;
		else if (Vars::CritHack::AvoidRandom.Value && closestSkip >= 0)
			pCmd->command_number = closestSkip;
	}
	/*
	else if (IsEnabled() && closest_skip >= 0)
		pCmd->command_number = closest_skip;
	*/

	//if (pCmd->command_number == closestCrit || pCmd->command_number == closestSkip)
		WishRandomSeed = MD5_PseudoRandom(pCmd->command_number) & 0x7FFFFFFF;

	if (pCmd->command_number == closestCrit)
		ForceCmds.pop_front();
	else if (pCmd->command_number == closestSkip)
		SkipCmds.pop_front();
}

bool CCritHack::CalcIsAttackCriticalHandler(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	if (/*!I::Prediction->m_bFirstTimePredicted ||*/ !pLocal || !pWeapon)
		return false;

	if (pWeapon->GetWeaponID() == TF_WEAPON_MINIGUN || pWeapon->GetWeaponID() == TF_WEAPON_FLAMETHROWER)
	{
		static auto AmmoCount = pLocal->GetAmmoCount(pWeapon->m_iPrimaryAmmoType());
		if (AmmoCount != pLocal->GetAmmoCount(pWeapon->m_iPrimaryAmmoType()))
		{
			AmmoCount = pLocal->GetAmmoCount(pWeapon->m_iPrimaryAmmoType());
			return false;
		}
	}

	if (WishRandomSeed != 0)
	{
		*I::RandomSeed = WishRandomSeed;
		WishRandomSeed = 0;
	}

	return true;
}

void CCritHack::Event(CGameEvent* pEvent, FNV1A_t uNameHash)
{
	if (uNameHash == FNV1A::HashConst("player_hurt"))
	{
		const auto& pLocal = g_EntityCache.GetLocal();
		if (!pLocal)
			return;

		const auto attacked = I::EngineClient->GetPlayerForUserID(pEvent->GetInt("userid"));
		const auto attacker = I::EngineClient->GetPlayerForUserID(pEvent->GetInt("attacker"));
		auto	   crit = pEvent->GetInt("crit");
		auto       damage = pEvent->GetInt("damageamount");
		const auto health = pEvent->GetInt("health");
		const auto weapon_id = pEvent->GetInt("weaponid");

		if (attacker == pLocal->GetIndex() && attacked != attacker)
		{
			const auto& pWeapon = g_EntityCache.GetWeapon();
			if (!pWeapon || pWeapon->GetWeaponID() == weapon_id && pWeapon->GetSlot() == SLOT_MELEE || pLocal->InCond(TF_COND_CRITBOOSTED))
				return;

			AllDamage += damage;
			if (crit)
				CritDamage += damage;
		}
	}
	else if (uNameHash == FNV1A::HashConst("teamplay_round_start"))
	{
		CritDamage = 0.f;
		AllDamage = 0.f;
	}
	else if (uNameHash == FNV1A::HashConst("client_beginconnect") || uNameHash == FNV1A::HashConst("client_disconnect") ||
		uNameHash == FNV1A::HashConst("game_newmap"))
	{
		Reset();
	}
	else if (uNameHash == FNV1A::HashConst("player_changeclass"))
	{
		const bool bLocal = I::EngineClient->GetPlayerForUserID(pEvent->GetInt("userid")) == I::EngineClient->GetLocalPlayer();
		if (bLocal)
			Storage = {};
	}
}

void CCritHack::Draw()
{
	if (!Vars::CritHack::Active.Value || !(Vars::Menu::Indicators.Value & (1 << 1)))
		return;
	if (!IsEnabled() || !G::CurrentUserCmd)
		return;

	const auto& pLocal = g_EntityCache.GetLocal();
	if (!pLocal || !pLocal->IsAlive() || pLocal->IsAGhost())
		return;

	const auto& pWeapon = pLocal->GetActiveWeapon();
	if (!pWeapon)
		return;

	int x = Vars::Menu::CritsDisplay.Value.x;
	int y = Vars::Menu::CritsDisplay.Value.y + 8;

	const auto& fFont = g_Draw.GetFont(FONT_INDICATORS);

	EAlign align = ALIGN_TOP;
	if (x <= (100 + 50 * Vars::Menu::DPI.Value))
	{
		x -= 42 * Vars::Menu::DPI.Value;
		align = ALIGN_TOPLEFT;
	}
	else if (x >= g_ScreenSize.w - (100 + 50 * Vars::Menu::DPI.Value))
	{
		x += 42 * Vars::Menu::DPI.Value;
		align = ALIGN_TOPRIGHT;
	}

	if (WeaponCanCrit(pWeapon))
	{
		const auto slot = pWeapon->GetSlot();
		const auto bRapidFire = pWeapon->IsRapidFire();

		if (Storage[slot].Damage > 0)
		{
			if (pLocal->IsCritBoosted())
				g_Draw.String(fFont, x, y, { 100, 255, 255, 255 }, align, "Crit Boosted");
			else if (pWeapon->CritTime() > I::GlobalVars->interval_per_tick * pLocal->m_nTickBase())
			{
				const float time = pWeapon->CritTime() - I::GlobalVars->interval_per_tick * pLocal->m_nTickBase();
				g_Draw.String(fFont, x, y, { 100, 255, 255, 255 }, align, std::format("Streaming crits {:.1f}s", time).c_str());
			}
			else if (!CritBanned)
			{
				if (Storage[slot].AvailableCrits > 0)
				{
					if (bRapidFire && pWeapon->LastRapidfireCritCheckTime() + 1.f > I::GlobalVars->interval_per_tick * pLocal->m_nTickBase())
					{
						//const float time = std::max((TICKS_TO_TIME(Storage[slot].StreamWait - pLocal->m_nTickBase())), 0.f);
						const float time = pWeapon->LastRapidfireCritCheckTime() + 1.f - I::GlobalVars->interval_per_tick * pLocal->m_nTickBase();
						g_Draw.String(fFont, x, y, Vars::Menu::Theme::Active.Value, align, std::format("Wait {:.1f}s", time).c_str());
					}
					else
						g_Draw.String(fFont, x, y, { 150, 255, 150, 255 }, align, "Crit Ready");
				}
				else
				{
					const int shots = -Storage[slot].AvailableCrits;
					g_Draw.String(fFont, x, y, { 255, 150, 150, 255 }, align, shots == 1 ? std::format("Crit in {} shot", shots).c_str() : std::format("Crit in {} shots", shots).c_str());
				}
			}
			else if (CritBanned)
				g_Draw.String(fFont, x, y, { 255, 150, 150, 255 }, align, std::format("Deal {} damage", DamageTilUnban).c_str());

			g_Draw.String(fFont, x, y + fFont.nTall + 2, Vars::Menu::Theme::Active.Value, align, std::format("{} / {} potential crits", std::max(Storage[slot].AvailableCrits, 0), Storage[slot].PotentialCrits).c_str());
		}
		else
			g_Draw.String(fFont, x, y, Vars::Menu::Theme::Active.Value, align, "Calculating");

		if (Vars::Debug::Info.Value)
		{
			g_Draw.String(fFont, x, y + fFont.nTall * 3, { 255, 255, 255, 255 }, align, std::format("AllDamage: {}, CritDamage: {}", AllDamage, CritDamage).c_str());
			g_Draw.String(fFont, x, y + fFont.nTall * 4, { 255, 255, 255, 255 }, align, std::format("Bucket: {}", pWeapon->CritTokenBucket()).c_str());
			g_Draw.String(fFont, x, y + fFont.nTall * 5, { 255, 255, 255, 255 }, align, std::format("Damage: {}, Cost: {}", Storage[slot].Damage, Storage[slot].Cost).c_str());
			g_Draw.String(fFont, x, y + fFont.nTall * 6, { 255, 255, 255, 255 }, align, std::format("CritChecks: {}, CritSeedRequests: {}", pWeapon->CritChecks(), pWeapon->CritSeedRequests()).c_str());
			g_Draw.String(fFont, x, y + fFont.nTall * 7, { 255, 255, 255, 255 }, align, std::format("CritBanned: {}, DamageTilUnban: {}", CritBanned, DamageTilUnban).c_str());
			g_Draw.String(fFont, x, y + fFont.nTall * 8, { 255, 255, 255, 255 }, align, std::format("CritChance: {:.2f}", CritChance).c_str());
			g_Draw.String(fFont, x, y + fFont.nTall * 9, { 255, 255, 255, 255 }, align, std::format("Force: {}, Skip: {}", ForceCmds.size(), SkipCmds.size()).c_str());

			g_Draw.String(fFont, x, y + fFont.nTall * 11, { 255, 255, 255, 255 }, align, L"G::WeaponCanAttack %d (%d)", G::CanPrimaryAttack, I::GlobalVars->tickcount % 66);
			g_Draw.String(fFont, x, y + fFont.nTall * 12, { 255, 255, 255, 255 }, align, L"G::IsAttacking %d", G::IsAttacking);
		}
	}
}