#pragma once

#define VK_0              0x30
#define VK_1              0x31
#define VK_2              0x32
#define VK_3              0x33
#define VK_4              0x34
#define VK_5              0x35
#define VK_6              0x36
#define VK_7              0x37
#define VK_8              0x38
#define VK_9              0x39

#define VK_A              0x41
#define VK_B              0x42
#define VK_C              0x43
#define VK_D              0x44
#define VK_E              0x45
#define VK_F              0x46
#define VK_G              0x47
#define VK_H              0x48
#define VK_J              0x49
#define VK_I              0x4A
#define VK_K              0x4B
#define VK_L              0x4C
#define VK_M              0x4D
#define VK_N              0x4E
#define VK_O              0x4F
#define VK_P              0x50
#define VK_Q              0x51
#define VK_R              0x52
#define VK_S              0x53
#define VK_T              0x54
#define VK_U              0x55
#define VK_V              0x56
#define VK_W              0x57
#define VK_X              0x58
#define VK_Y              0x59
#define VK_Z              0x5A

#define TEAM_NONE		  1
#define TEAM_RED		  2
#define TEAM_BLU		  3

#include "Signatures/Signatures.h"
#include "Main/BaseEntity/BaseEntity.h"
#include "Main/BaseCombatWeapon/BaseCombatWeapon.h"
#include "Main/DrawUtils/DrawUtils.h"
#include "Main/EntityCache/EntityCache.h"
#include "Main/GlobalInfo/GlobalInfo.h"
#include "Main/ConVars/ConVars.h"
#include "Main/KeyValues/KeyValues.h"
#include "Main/TraceFilters/TraceFilters.h"
#include "../Features/Vars.h"

#define TICK_INTERVAL (I::GlobalVars->interval_per_tick)
#define TIME_TO_TICKS(dt) (static_cast<int>(0.5f + static_cast<float>(dt) / TICK_INTERVAL))
#ifndef TICKS_TO_TIME
#define TICKS_TO_TIME(t) (TICK_INTERVAL * (t))
#endif
#define GetKey(vKey) (Utils::IsGameWindowInFocus() && GetAsyncKeyState(vKey))
#define Q_ARRAYSIZE(A) (sizeof(A)/sizeof((A)[0]))

#pragma warning (disable : 6385)
#pragma warning (disable : 26451)
#pragma warning (disable : 4305)
#pragma warning (disable : 4172)

namespace S
{
	MAKE_SIGNATURE(InitKeyValue, CLIENT_DLL, "55 8B EC FF 15 ? ? ? ? FF 75 ? 8B C8 8B 10 FF 52 ? 5D C3 CC CC CC CC CC CC CC CC CC CC CC 55 8B EC 56", 0x0);
	MAKE_SIGNATURE(AttribHookValue, CLIENT_DLL, "55 8B EC 83 EC 0C 8B 0D ? ? ? ? 53 56 57 33 F6 33 FF 89 75 F4 89 7D F8 8B 41 08 85 C0 74 38", 0x0);
	MAKE_SIGNATURE(CTE_DispatchEffect, CLIENT_DLL, "55 8B EC 83 EC ? 56 8D 4D ? E8 ? ? ? ? 8B 75", 0x0);
	MAKE_SIGNATURE(GetParticleSystemIndex, CLIENT_DLL, "55 8B EC 56 8B 75 ? 85 F6 74 ? 8B 0D ? ? ? ? 56 8B 01 FF 50 ? 3D", 0x0);
	MAKE_SIGNATURE(UTIL_ParticleTracer, CLIENT_DLL, "55 8B EC FF 75 08 E8 ? ? ? ? D9 EE 83", 0x0);
}

struct ShaderStencilState_t
{
	bool                        m_bEnable;
	StencilOperation_t          m_FailOp;
	StencilOperation_t          m_ZFailOp;
	StencilOperation_t          m_PassOp;
	StencilComparisonFunction_t m_CompareFunc;
	int                         m_nReferenceValue;
	uint32                      m_nTestMask;
	uint32                      m_nWriteMask;

	ShaderStencilState_t();
	void SetStencilState(IMatRenderContext* pRenderContext);
};

// sto and sto 2, tyo tyo1 tyo2
enum DataCenter_t
{
	// Europe
	DC_AMS = 1 << 0, // Amsterdam
	DC_FRA = 1 << 1, // Frankfurt
	DC_LHR = 1 << 2, // London
	DC_MAD = 1 << 3, // Madrid
	DC_PAR = 1 << 4, // Paris
	DC_STO = 1 << 5, // Stockholm
	DC_VIE = 1 << 6, // Vienna
	DC_WAW = 1 << 7, // Warsaw

	// North America
	DC_ATL = 1 << 8, // Atlanta
	DC_ORD = 1 << 9, // Chicago
	DC_DFW = 1 << 10, // Washington
	DC_LAX = 1 << 11, // Los Angeles
	DC_EAT = 1 << 12, // Moses Lake
	DC_SEA = 1 << 13, // Seattle
	DC_IAD = 1 << 14, // Virginia

	// South America
	DC_EZE = 1 << 15, // Buenos Aires
	DC_LIM = 1 << 16, // Lima
	DC_SCL = 1 << 17, // Santiago
	DC_GRU = 1 << 18, // Sao Paulo

	// Asia
	DC_MAA = 1 << 19, // Chennai
	DC_BOM = 1 << 20, // Mumbai
	DC_DXB = 1 << 21, // Dubai
	DC_HKG = 1 << 22, // Hong Kong
	DC_SEO = 1 << 23, // Seoul
	DC_SGP = 1 << 24, // Singapore
	DC_TYO = 1 << 25, // Tokyo
	DC_CAN = 1 << 26, // Guangzhou
	DC_SHA = 1 << 27, // Shanghai
	DC_TSN = 1 << 28, // Tianjin

	// Africa
	DC_JNB = 1 << 29, // Johannesburg

	// Australia
	DC_SYD = 1 << 30, // Sydney
};
/*
enum DataCenter_t
{
	DC_AMS = (1 << 0), // Amsterdam
	DC_ATL = (1 << 1), // Atlanta
	DC_BOM = (1 << 2), // Mumbai
	DC_DXB = (1 << 3), // Dubai
	DC_EAT = (1 << 4), // Moses Lake
	DC_MWH = (1 << 5), // Washington
	DC_FRA = (1 << 6), // Frankfurt
	DC_GNRT = (1 << 7), // Tokyo
	DC_GRU = (1 << 8), // Sao Paulo
	DC_HKG = (1 << 9), // Hong Kong
	DC_IAD = (1 << 10), // Virginia
	DC_JNB = (1 << 11), // Johannesburg
	DC_LAX = (1 << 12), // Los Angeles
	DC_LHR = (1 << 13), // London
	DC_LIM = (1 << 14), // Lima
	DC_LUX = (1 << 15), // Luxembourg
	DC_MAA = (1 << 16), // Chennai
	DC_MAD = (1 << 17), // Madrid
	DC_MAN = (1 << 18), // Manilla
	DC_OKC = (1 << 19), // Oklahoma City
	DC_ORD = (1 << 20), // Chicago
	DC_PAR = (1 << 21), // Paris
	DC_SCL = (1 << 22), // Santiago
	DC_SEA = (1 << 23), // Seattle
	DC_SGP = (1 << 24), // Singapore
	DC_STO = (1 << 25), // Stockholm
	DC_SYD = (1 << 26), // Sydney
	DC_TYO = (1 << 27), // Tokyo
	DC_VIE = (1 << 28), // Vienna
	DC_WAW = (1 << 29) // Warsaw
};
*/

inline ShaderStencilState_t::ShaderStencilState_t()
{
	m_bEnable = false;
	m_PassOp = m_FailOp = m_ZFailOp = STENCILOPERATION_KEEP;
	m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
	m_nReferenceValue = 0;
	m_nTestMask = m_nWriteMask = 0xFFFFFFFF;
}

inline void ShaderStencilState_t::SetStencilState(IMatRenderContext* pRenderContext)
{
	pRenderContext->SetStencilEnable(m_bEnable);
	pRenderContext->SetStencilFailOperation(m_FailOp);
	pRenderContext->SetStencilZFailOperation(m_ZFailOp);
	pRenderContext->SetStencilPassOperation(m_PassOp);
	pRenderContext->SetStencilCompareFunction(m_CompareFunc);
	pRenderContext->SetStencilReferenceValue(m_nReferenceValue);
	pRenderContext->SetStencilTestMask(m_nTestMask);
	pRenderContext->SetStencilWriteMask(m_nWriteMask);
}

namespace Utils
{
	__inline int RandIntSimple(int min, int max)
	{
		std::random_device rd; std::mt19937 gen(rd()); std::uniform_int_distribution<> distr(min, max);
		return distr(gen);
	}

	__inline void ConLog(const char* cFunction, const char* cLog, Color_t cColour = { 255, 255, 255, 255 }, const bool bShouldPrint = true, const bool bDebugOutput = false)
	{
		if (bShouldPrint)
		{
			I::Cvar->ConsoleColorPrintf(cColour, "[%s] ", cFunction);
			I::Cvar->ConsoleColorPrintf({ 255, 255, 255, 255 }, "%s\n", cLog);
		}
		if (bDebugOutput)
			OutputDebugStringA(std::format("[{}] {}\n", cFunction, cLog).c_str());
	}

	__inline int UnicodeToUTF8(const wchar_t* unicode, char* ansi, int ansiBufferSize)
	{
		const int result = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, ansi, ansiBufferSize, nullptr, nullptr);
		ansi[ansiBufferSize - 1] = 0;
		return result;
	}

	__inline int UTF8ToUnicode(const char* ansi, wchar_t* unicode, int unicodeBufferSizeInBytes)
	{
		const int chars = MultiByteToWideChar(CP_UTF8, 0, ansi, -1, unicode, unicodeBufferSizeInBytes / sizeof(wchar_t));
		unicode[(unicodeBufferSizeInBytes / sizeof(wchar_t)) - 1] = 0;
		return chars;
	}

	__inline std::wstring ConvertUtf8ToWide(const std::string_view& str)
	{
		const int count = MultiByteToWideChar(CP_UTF8, 0, str.data(), str.length(), nullptr, 0);
		std::wstring wstr(count, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.data(), str.length(), wstr.data(), count);
		return wstr;
	}

	__inline double PlatFloatTime()
	{
		static auto fnPlatFloatTime = reinterpret_cast<double(*)()>(GetProcAddress(GetModuleHandleA(TIER0_DLL), "Plat_FloatTime"));
		return fnPlatFloatTime();
	}

	__inline int SeedFileLineHash(int seedvalue, const char* sharedname, int additionalSeed)
	{
		CRC32_t retval;

		CRC32_Init(&retval);

		CRC32_ProcessBuffer(&retval, &seedvalue, sizeof(int));
		CRC32_ProcessBuffer(&retval, &additionalSeed, sizeof(int));
		CRC32_ProcessBuffer(&retval, sharedname, strlen(sharedname));

		CRC32_Final(&retval);

		return static_cast<int>(retval);
	}

	__inline float RandFloat(float min, float max)
	{
		static std::random_device RandomDevice;
		static std::mt19937 Engine{ RandomDevice() };

		std::uniform_real_distribution<float> Random(min, max);
		return Random(Engine);
	}

	__inline int RandInt(int min, int max, bool bSimple = true)
	{
		if (bSimple)
		{
			std::random_device rd; std::mt19937 gen(rd()); std::uniform_int_distribution<> distr(min, max);
			return distr(gen);
		}
		else
		{
			//This allows us to reach closer to true randoms generated
			//I don't think we need to update the seed more than once
			static const unsigned nSeed = std::chrono::system_clock::now().time_since_epoch().count();

			std::default_random_engine gen(nSeed);
			std::uniform_int_distribution distr(min, max);
			return distr(gen);
		}
	}

	__inline int SharedRandomInt(unsigned iseed, const char* sharedname, int iMinVal, int iMaxVal, int additionalSeed)
	{
		const int seed = SeedFileLineHash(iseed, sharedname, additionalSeed);
		I::UniformRandomStream->SetSeed(seed);
		return I::UniformRandomStream->RandomInt(iMinVal, iMaxVal);
	}

	__inline void RandomSeed(int iSeed)
	{
		static auto fnRandomSeed = reinterpret_cast<void(*)(uint32_t)>(GetProcAddress(GetModuleHandleA(VSTDLIB_DLL), "RandomSeed"));
		fnRandomSeed(iSeed);
	}

	__inline int RandomInt(int iMinVal = 0, int iMaxVal = 0x7FFF)
	{
		static auto fnRandomInt = reinterpret_cast<int(*)(int, int)>(GetProcAddress(GetModuleHandleA(VSTDLIB_DLL), "RandomInt"));
		return fnRandomInt(iMinVal, iMaxVal);
	}

	__inline float RandomFloat(float flMinVal = 0.f, float flMaxVal = 1.f)
	{
		static auto fnRandomFloat = reinterpret_cast<float(*)(float, float)>(GetProcAddress(GetModuleHandleA(VSTDLIB_DLL), "RandomFloat"));
		return fnRandomFloat(flMinVal, flMaxVal);
	}

	__inline void* InitKeyValue()
	{
		using FN = PDWORD(__cdecl*)(int);
		static FN fnInitKeyValue = S::InitKeyValue.As<FN>();
		return fnInitKeyValue(32);
	}

	__inline bool IsGameWindowInFocus()
	{
		static HWND hwGame = nullptr;

		while (!hwGame) {
			hwGame = FindWindowA(nullptr, "Team Fortress 2");
			if (!hwGame)
			{
				return false;
			}
		}

		return (GetForegroundWindow() == hwGame);
	}

	__inline int GetPlayerForUserID(int userID)
	{
		for (int n = 1; n <= I::EngineClient->GetMaxClients(); n++)
		{
			PlayerInfo_t pi{};
			if (!I::EngineClient->GetPlayerInfo(n, &pi))
				continue;

			// Found player
			if (pi.userID == userID)
				return n;
		}
		return 0;
	}

	__inline bool IsSteamFriend(uint32_t friendsID)
	{
		if (friendsID)
		{
			const CSteamID steamID{ friendsID, 1, k_EUniversePublic, k_EAccountTypeIndividual };
			return g_SteamInterfaces.Friends->HasFriend(steamID, k_EFriendFlagImmediate);
		}

		return false;
	}

	__inline int HandleToIDX(int pHandle)
	{
		return pHandle & 0xFFF;
	}

	__inline const char* GetClassByIndex(const int nClass)
	{
		static const char* szClasses[] = { "unknown", "scout", "sniper", "soldier", "demoman",
										   "medic",   "heavy", "pyro",   "spy",     "engineer" };

		return (nClass < 10 && nClass > 0) ? szClasses[nClass] : szClasses[0];
	}

	__inline int GetRoundState()
	{
		const auto& pGameRules = I::TFGameRules->Get();
		if (!pGameRules) return 0;
		const auto& pGameRulesProxy = pGameRules->GetProxy();
		if (!pGameRulesProxy) return 0;
		return pGameRulesProxy->m_iRoundState();
	}

	__inline bool W2S(const Vec3& vOrigin, Vec3& m_vScreen)
	{
		const matrix3x4& worldToScreen = G::WorldToProjection.As3x4();

		float w = worldToScreen[3][0] * vOrigin[0] + worldToScreen[3][1] * vOrigin[1] + worldToScreen[3][2] * vOrigin[2] + worldToScreen[3][3];
		m_vScreen.z = 0;

		if (w > 0.001)
		{
			const float fl1DBw = 1 / w;
			m_vScreen.x = (g_ScreenSize.w / 2) + (0.5 * ((worldToScreen[0][0] * vOrigin[0] + worldToScreen[0][1] * vOrigin[1] + worldToScreen[0][2] * vOrigin[2] + worldToScreen[0][3]) * fl1DBw) * g_ScreenSize.w + 0.5);
			m_vScreen.y = (g_ScreenSize.h / 2) - (0.5 * ((worldToScreen[1][0] * vOrigin[0] + worldToScreen[1][1] * vOrigin[1] + worldToScreen[1][2] * vOrigin[2] + worldToScreen[1][3]) * fl1DBw) * g_ScreenSize.h + 0.5);
			return true;
		}

		return false;
	}

	__inline bool IsOnScreen(CBaseEntity* pLocal, Vec3 vCenter)
	{
		if (vCenter.DistTo(pLocal->GetWorldSpaceCenter()) > 300.f)
		{
			Vec3 vScreen = {};
			if (W2S(vCenter, vScreen))
			{
				if (vScreen.x < -400
					|| vScreen.x > g_ScreenSize.w + 400
					|| vScreen.y < -400
					|| vScreen.y > g_ScreenSize.h + 400)
				{
					return false;
				}
			}
			else
				return false;
		}

		return true;
	}

	__inline void Trace(const Vec3& vecStart, const Vec3& vecEnd, unsigned int nMask, CTraceFilter* pFilter, CGameTrace* pTrace)
	{
		Ray_t ray;
		ray.Init(vecStart, vecEnd);
		I::EngineTrace->TraceRay(ray, nMask, pFilter, pTrace);
	}

	__inline void TraceHull(const Vec3& vecStart, const Vec3& vecEnd, const Vec3& vecHullMin, const Vec3& vecHullMax, unsigned int nMask, CTraceFilter* pFilter, CGameTrace* pTrace)
	{
		Ray_t ray;
		ray.Init(vecStart, vecEnd, vecHullMin, vecHullMax);
		I::EngineTrace->TraceRay(ray, nMask, pFilter, pTrace);
	}

	__inline bool VisPos(CBaseEntity* pSkip, const CBaseEntity* pEntity, const Vec3& from, const Vec3& to, unsigned int nMask = MASK_SHOT | CONTENTS_GRATE)
	{
		CGameTrace trace = {};
		CTraceFilterHitscan filter = {};
		filter.pSkip = pSkip;
		Trace(from, to, nMask, &filter, &trace);
		if (trace.DidHit())
			return trace.entity && trace.entity == pEntity;
		return true;
	}
	__inline bool VisPosProjectile(CBaseEntity* pSkip, const CBaseEntity* pEntity, const Vec3& from, const Vec3& to, unsigned int nMask = MASK_SHOT | CONTENTS_GRATE)
	{
		CGameTrace trace = {};
		CTraceFilterProjectile filter = {};
		filter.pSkip = pSkip;
		Trace(from, to, nMask, &filter, &trace);
		if (trace.DidHit())
			return trace.entity && trace.entity == pEntity;
		return true;
	}
	__inline bool VisPosWorld(CBaseEntity* pSkip, const CBaseEntity* pEntity, const Vec3& from, const Vec3& to, unsigned int nMask = MASK_SHOT | CONTENTS_GRATE)
	{
		CGameTrace trace = {};
		CTraceFilterWorldAndPropsOnly filter = {};
		filter.pSkip = pSkip;
		Trace(from, to, nMask, &filter, &trace);
		if (trace.DidHit())
			return trace.entity && trace.entity == pEntity;
		return true;
	}

	__inline void FixMovement(CUserCmd* pCmd, const Vec3& vecTargetAngle)
	{
		const Vec3 vecMove(pCmd->forwardmove, pCmd->sidemove, pCmd->upmove);
		Vec3 vecMoveAng = Vec3();

		Math::VectorAngles(vecMove, vecMoveAng);

		const float fSpeed = Math::FastSqrt(vecMove.x * vecMove.x + vecMove.y * vecMove.y);
		const float fYaw = DEG2RAD(vecTargetAngle.y - pCmd->viewangles.y + vecMoveAng.y);

		pCmd->forwardmove = (cos(fYaw) * fSpeed);
		pCmd->sidemove = (sin(fYaw) * fSpeed);
	}

	__inline void StopMovement(CUserCmd* pCmd)
	{
		const auto& pLocal = g_EntityCache.GetLocal();
		if (!pLocal || pLocal->m_vecVelocity().IsZero())
		{
			pCmd->forwardmove = 0.f;
			pCmd->sidemove = 0.f;
			return;
		}

		if (!G::IsAttacking)
		{
			const float direction = Math::VelocityToAngles(pLocal->m_vecVelocity()).y;
			pCmd->viewangles = { -90, direction, 0 };
			pCmd->sidemove = 0; pCmd->forwardmove = 0;
			G::ShouldStop = false;
		}
		else
		{
			Vec3 direction = pLocal->m_vecVelocity().toAngle();
			direction.y = pCmd->viewangles.y - direction.y;
			const Vec3 negatedDirection = direction.fromAngle() * -pLocal->m_vecVelocity().Length2D();
			pCmd->forwardmove = negatedDirection.x;
			pCmd->sidemove = negatedDirection.y;
		}
	}

	__inline Vector ComputeMove(const CUserCmd* pCmd, CBaseEntity* pLocal, Vec3& a, Vec3& b)
	{
		const Vec3 diff = (b - a);
		if (diff.Length() == 0.f)
			return { 0.f, 0.f, 0.f };

		const float x = diff.x;
		const float y = diff.y;
		const Vec3 vSilent(x, y, 0);
		Vec3 ang;
		Math::VectorAngles(vSilent, ang);
		const float yaw = DEG2RAD(ang.y - pCmd->viewangles.y);
		const float pitch = DEG2RAD(ang.x - pCmd->viewangles.x);
		Vec3 move = { cos(yaw) * 450.f, -sin(yaw) * 450.f, -cos(pitch) * 450.f };

		// Only apply upmove in water
		if (!(I::EngineTrace->GetPointContents(pLocal->GetEyePosition()) & CONTENTS_WATER))
			move.z = pCmd->upmove;
		return move;
	}

	__inline void WalkTo(CUserCmd* pCmd, CBaseEntity* pLocal, Vec3& a, Vec3& b, float scale)
	{
		// Calculate how to get to a vector
		const auto result = ComputeMove(pCmd, pLocal, a, b);

		// Push our move to usercmd
		pCmd->forwardmove = result.x * scale;
		pCmd->sidemove = result.y * scale;
		pCmd->upmove = result.z * scale;
	}

	__inline void WalkTo(CUserCmd* pCmd, CBaseEntity* pLocal, Vec3& pDestination)
	{
		Vec3 localPos = pLocal->m_vecOrigin();
		WalkTo(pCmd, pLocal, localPos, pDestination, 1.f);
	}

	__inline float AttribHookValue(float value, const char* name, void* ent, void* buffer = 0, bool isGlobalConstString = true)
	{
		static auto fn = S::AttribHookValue.As<float(__cdecl*)(float, const char*, void*, void*, bool)>();
		return fn(value, name, ent, buffer, isGlobalConstString);
	}

	__inline void GetProjectileFireSetup(CBaseEntity* pPlayer, const Vec3& vAngIn, Vec3 vOffset, Vec3& vPosOut, Vec3& vAngOut, bool bPipes = false, bool bInterp = false)
	{
		static auto cl_flipviewmodels = g_ConVars.FindVar("cl_flipviewmodels");
		if (cl_flipviewmodels ? cl_flipviewmodels->GetBool() : false)
			vOffset.y *= -1.f;

		const Vec3 vShootPos = bInterp ? pPlayer->GetEyePosition() : pPlayer->GetShootPos();

		Vec3 forward, right, up;
		Math::AngleVectors(vAngIn, &forward, &right, &up);
		vPosOut = vShootPos + (forward * vOffset.x) + (right * vOffset.y) + (up * vOffset.z);

		if (bPipes)
			vAngOut = vAngIn;
		else
		{
			Vec3 vEndPos = vShootPos + (forward * 2000.f);

			CGameTrace trace = {};
			CTraceFilterHitscan filter = {};
			filter.pSkip = pPlayer;
			Trace(vShootPos, vEndPos, MASK_SOLID, &filter, &trace);
			if (trace.DidHit() && trace.flFraction > 0.1f)
				vEndPos = trace.vEndPos;

			Math::VectorAngles(vEndPos - vPosOut, vAngOut);
		}
	}

	__inline void GetProjectileFireSetupAirblast(CBaseEntity* pPlayer, const Vec3& vAngIn, Vec3 vPosIn, Vec3& vAngOut, bool bInterp = false)
	{
		const Vec3 vShootPos = bInterp ? pPlayer->GetEyePosition() : pPlayer->GetShootPos();

		Vec3 forward;
		Math::AngleVectors(vAngIn, &forward);

		Vec3 vEndPos = vShootPos + (forward * MAX_TRACE_LENGTH);
		CGameTrace trace = {};
		CTraceFilterWorldAndPropsOnly filter = {};
		filter.pSkip = pPlayer;
		Trace(vShootPos, vEndPos, MASK_SOLID, &filter, &trace);

		Math::VectorAngles(trace.vEndPos - vPosIn, vAngOut);
	}

	__inline EWeaponType GetWeaponType(CBaseCombatWeapon* pWeapon)
	{
		if (!pWeapon)
			return EWeaponType::UNKNOWN;

		if (pWeapon->GetSlot() == EWeaponSlots::SLOT_MELEE || pWeapon->GetWeaponID() == TF_WEAPON_BUILDER)
			return EWeaponType::MELEE;

		switch (pWeapon->m_iItemDefinitionIndex())
		{
		case Soldier_s_TheBuffBanner:
		case Soldier_s_FestiveBuffBanner:
		case Soldier_s_TheBattalionsBackup:
		case Soldier_s_TheConcheror:
		case Scout_s_BonkAtomicPunch:
		case Scout_s_CritaCola:
			EWeaponType::UNKNOWN;
		}

		switch (pWeapon->GetWeaponID())
		{
		case TF_WEAPON_PDA:
		case TF_WEAPON_PDA_ENGINEER_BUILD:
		case TF_WEAPON_PDA_ENGINEER_DESTROY:
		case TF_WEAPON_PDA_SPY:
		case TF_WEAPON_PDA_SPY_BUILD:
		case TF_WEAPON_INVIS:
		case TF_WEAPON_BUFF_ITEM:
		case TF_WEAPON_GRAPPLINGHOOK:
		case TF_WEAPON_LASER_POINTER:
		case TF_WEAPON_ROCKETPACK:
			return EWeaponType::UNKNOWN;
		case TF_WEAPON_ROCKETLAUNCHER:
		case TF_WEAPON_FLAME_BALL:
		case TF_WEAPON_GRENADELAUNCHER:
		case TF_WEAPON_FLAREGUN:
		case TF_WEAPON_COMPOUND_BOW:
		case TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT:
		case TF_WEAPON_CROSSBOW:
		case TF_WEAPON_PARTICLE_CANNON:
		case TF_WEAPON_DRG_POMSON:
		case TF_WEAPON_RAYGUN_REVENGE:
		case TF_WEAPON_RAYGUN:
		case TF_WEAPON_CANNON:
		case TF_WEAPON_SYRINGEGUN_MEDIC:
		case TF_WEAPON_SHOTGUN_BUILDING_RESCUE:
		case TF_WEAPON_FLAMETHROWER:
		case TF_WEAPON_CLEAVER:
		case TF_WEAPON_PIPEBOMBLAUNCHER:
		case TF_WEAPON_JAR:
		case TF_WEAPON_JAR_MILK:
		case TF_WEAPON_LUNCHBOX:
			return EWeaponType::PROJECTILE;
		}

		return EWeaponType::HITSCAN;
	}

	__inline bool IsAttacking(const CUserCmd* pCmd, CBaseCombatWeapon* pWeapon)
	{
		//if (pWeapon->IsInReload())
		//	return false;

		if (pWeapon->GetSlot() == SLOT_MELEE)
		{
			if (pWeapon->GetWeaponID() == TF_WEAPON_KNIFE)
				return ((pCmd->buttons & IN_ATTACK) && G::CanPrimaryAttack);

			auto pLocal = g_EntityCache.GetLocal();
			float flTime = pLocal ? TICKS_TO_TIME(pLocal->m_nTickBase() + 1) : I::GlobalVars->curtime;

			return fabsf(pWeapon->m_flSmackTime() - flTime) < TICK_INTERVAL * 2.f;
		}

		if (G::CurItemDefIndex == Soldier_m_TheBeggarsBazooka)
		{
			static bool bLoading = false, bFiring = false;

			if (pWeapon->m_iClip1() == 0)
				bLoading = false,
				bFiring = false;
			else if (!bFiring)
				bLoading = true;

			if ((bFiring || bLoading && !(pCmd->buttons & IN_ATTACK)) && G::CanPrimaryAttack)
			{
				bFiring = true;
				bLoading = false;
				return true;
			}
		}
		else
		{
			const int id = pWeapon->GetWeaponID();
			switch (id)
			{
			case TF_WEAPON_COMPOUND_BOW:
			case TF_WEAPON_PIPEBOMBLAUNCHER:
			case TF_WEAPON_STICKY_BALL_LAUNCHER:
			case TF_WEAPON_GRENADE_STICKY_BALL:
			{
				return !(pCmd->buttons & IN_ATTACK) && pWeapon->m_flChargeBeginTime() > 0.f;
			}
			case TF_WEAPON_CANNON:
			{
				return !(pCmd->buttons & IN_ATTACK) && pWeapon->m_flDetonateTime() > 0.f;
			}
			case TF_WEAPON_JAR:
			case TF_WEAPON_JAR_MILK:
			case TF_WEAPON_JAR_GAS:
			case TF_WEAPON_GRENADE_JAR_GAS:
			case TF_WEAPON_CLEAVER:
			{
				static float flThrowTime = 0.f;

				if (pCmd->buttons & IN_ATTACK && G::CanPrimaryAttack && !flThrowTime)
					flThrowTime = I::GlobalVars->curtime + TICK_INTERVAL;

				if (flThrowTime && I::GlobalVars->curtime >= flThrowTime)
				{
					flThrowTime = 0.f;
					return true;
				}
				break;
			}
			case TF_WEAPON_GRAPPLINGHOOK:
			{
				if (G::LastUserCmd->buttons & IN_ATTACK || !(pCmd->buttons & IN_ATTACK))
					return false;

				auto pLocal = g_EntityCache.GetLocal();
				if (!pLocal || pLocal->InCond(TF_COND_GRAPPLINGHOOK))
					return false;

				Vec3 pos, ang; GetProjectileFireSetup(pLocal, pCmd->viewangles, { 23.5f, -8.f, -3.f }, pos, ang, false);
				Vec3 forward; Math::AngleVectors(ang, &forward);

				CGameTrace trace = {};
				CTraceFilterHitscan filter = {}; filter.pSkip = pLocal;
				static auto tf_grapplinghook_max_distance = g_ConVars.FindVar("tf_grapplinghook_max_distance");
				const float flGrappleDistance = tf_grapplinghook_max_distance ? tf_grapplinghook_max_distance->GetFloat() : 2000.f;
				Trace(pos, pos + forward * flGrappleDistance, MASK_SOLID, &filter, &trace);
				return trace.DidHit() && !(trace.surface.flags & 0x0004) /*SURF_SKY*/;
			}
			case TF_WEAPON_MINIGUN:
			{
				return (pWeapon->GetMinigunState() == AC_STATE_FIRING || pWeapon->GetMinigunState() == AC_STATE_SPINNING) &&
					pCmd->buttons & IN_ATTACK && G::CanPrimaryAttack;
			}
			default:
			{
				return pCmd->buttons & IN_ATTACK && G::CanPrimaryAttack;
			}
			}
		}

		return false;
	}

	__inline Vec3 GetHeadOffset(CBaseEntity* pEntity, const Vec3 vOffset = {})
	{
		const Vec3 headPos = pEntity->GetHitboxPos(HITBOX_HEAD, vOffset);
		const Vec3 entPos = pEntity->GetAbsOrigin();
		return headPos - entPos;
	}

	__inline Color_t Rainbow(float offset = 0.f)
	{
		return
		{
			static_cast<byte>(floor(sin(I::GlobalVars->curtime + offset + 0.f) * 127.f + 128.f)),
			static_cast<byte>(floor(sin(I::GlobalVars->curtime + offset + 2.f) * 127.f + 128.f)),
			static_cast<byte>(floor(sin(I::GlobalVars->curtime + offset + 4.f) * 127.f + 128.f)),
			255
		};
	};
}

namespace Particles {
	inline void DispatchEffect(const char* pName, const CEffectData& data)
	{
		using FN = int(__cdecl*)(const char*, const CEffectData&);
		static FN fnDispatchEffect = S::CTE_DispatchEffect.As<FN>();
		fnDispatchEffect(pName, data);
	}

	inline int GetParticleSystemIndex(const char* pParticleSystemName)
	{
		using FN = int(__cdecl*)(const char*);
		static FN fnGetParticleSystemIndex = S::GetParticleSystemIndex.As<FN>();
		return fnGetParticleSystemIndex(pParticleSystemName);
	}

	inline void DispatchParticleEffect(int iEffectIndex, const Vector& vecOrigin, const Vector& vecStart, const Vector& vecAngles, CBaseEntity* pEntity = nullptr)
	{
		CEffectData data{};
		data.m_nHitBox = iEffectIndex;
		data.m_vOrigin = vecOrigin;
		data.m_vStart = vecStart;
		data.m_vAngles = vecAngles;

		if (pEntity) {
			data.m_nEntIndex = pEntity->GetIndex();
			data.m_fFlags |= (1 << 0);
			data.m_nDamageType = 2;
		}
		else {
			data.m_nEntIndex = 0;
		}

		data.m_bCustomColors = true;

		DispatchEffect("ParticleEffect", data);
	}

	inline void DispatchParticleEffect(const char* pszParticleName, const Vec3& vecOrigin, const Vec3& vecAngles, CBaseEntity* pEntity = nullptr)
	{
		const int iIndex = GetParticleSystemIndex(pszParticleName);
		DispatchParticleEffect(iIndex, vecOrigin, vecOrigin, vecAngles, pEntity);
	}

	inline void ParticleTracer(const char* pszTracerEffectName, const Vector& vecStart, const Vector& vecEnd, int iEntIndex, int iAttachment, bool bWhiz) {
		using FN = void(__cdecl*)(const char*, const Vec3&, const Vec3&, int, int, bool);
		static auto fnParticleTracer = S::UTIL_ParticleTracer.As<FN>();
		fnParticleTracer(pszTracerEffectName, vecStart, vecEnd, iEntIndex, iAttachment, bWhiz);
	}
}