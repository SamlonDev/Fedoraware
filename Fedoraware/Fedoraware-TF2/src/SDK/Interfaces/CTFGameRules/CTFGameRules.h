#pragma once
#include "../../NetVars/NetVars.h"

// mfed

namespace S
{
	MAKE_SIGNATURE(CTFGameRules_Get, CLIENT_DLL, "8B 0D ? ? ? ? 56 57 8B 01 8B 40 ? FF D0 84 C0", 0x2);
}

class CGameRulesProxy
{
public:
};

class CTeamplayRoundBasedRulesProxy : public CGameRulesProxy
{
public:
	NETVAR(m_iRoundState, int, "CTeamplayRoundBasedRulesProxy", "m_iRoundState") //68
	NETVAR(m_bInWaitingForPlayers, bool, "CTeamplayRoundBasedRulesProxy", "m_bInWaitingForPlayers") //84
	NETVAR(m_iWinningTeam, int, "CTeamplayRoundBasedRulesProxy", "m_iWinningTeam") //76
	NETVAR(m_bInOvertime, bool, "CTeamplayRoundBasedRulesProxy", "m_bInOvertime") //72
	NETVAR(m_bInSetup, bool, "CTeamplayRoundBasedRulesProxy", "m_bInSetup") //73
	NETVAR(m_bSwitchedTeamsThisRound, bool, "CTeamplayRoundBasedRulesProxy", "m_bSwitchedTeamsThisRound") //74
	NETVAR(m_bAwaitingReadyRestart, bool, "CTeamplayRoundBasedRulesProxy", "m_bAwaitingReadyRestart") //85
	NETVAR(m_flRestartRoundTime, float, "CTeamplayRoundBasedRulesProxy", "m_flRestartRoundTime") //88
	NETVAR(m_flMapResetTime, float, "CTeamplayRoundBasedRulesProxy", "m_flMapResetTime") //92
	NETVAR(m_nRoundsPlayed, int, "CTeamplayRoundBasedRulesProxy", "m_nRoundsPlayed") //292
	NETVAR(m_flNextRespawnWave, void*, "CTeamplayRoundBasedRulesProxy", "m_flNextRespawnWave") //96
	NETVAR(m_TeamRespawnWaveTimes, void*, "CTeamplayRoundBasedRulesProxy", "m_TeamRespawnWaveTimes") //304
	NETVAR(m_bTeamReady, void*, "CTeamplayRoundBasedRulesProxy", "m_bTeamReady") //224
	NETVAR(m_bStopWatch, bool, "CTeamplayRoundBasedRulesProxy", "m_bStopWatch") //256
	NETVAR(m_bMultipleTrains, bool, "CTeamplayRoundBasedRulesProxy", "m_bMultipleTrains") //257
	NETVAR(m_bPlayerReady, void*, "CTeamplayRoundBasedRulesProxy", "m_bPlayerReady") //258
	NETVAR(m_bCheatsEnabledDuringLevel, bool, "CTeamplayRoundBasedRulesProxy", "m_bCheatsEnabledDuringLevel") //291
	NETVAR(m_flCountdownTime, float, "CTeamplayRoundBasedRulesProxy", "m_flCountdownTime") //296
	NETVAR(m_flStateTransitionTime, float, "CTeamplayRoundBasedRulesProxy", "m_flStateTransitionTime") //300
};

class CTFGameRulesProxy : public CTeamplayRoundBasedRulesProxy
{
public:
	NETVAR(m_nGameType, int, "CTFGameRulesProxy", "m_nGameType") //492
	NETVAR(m_nStopWatchState, int, "CTFGameRulesProxy", "m_nStopWatchState") //496
	NETVAR(m_pszTeamGoalStringRed, const char*, "CTFGameRulesProxy", "m_pszTeamGoalStringRed") //500
	NETVAR(m_pszTeamGoalStringBlue, const char*, "CTFGameRulesProxy", "m_pszTeamGoalStringBlue") //756
	NETVAR(m_flCapturePointEnableTime, float, "CTFGameRulesProxy", "m_flCapturePointEnableTime") //1012
	NETVAR(m_nHudType, int, "CTFGameRulesProxy", "m_nHudType") //1020
	NETVAR(m_bIsInTraining, bool, "CTFGameRulesProxy", "m_bIsInTraining") //1024
	NETVAR(m_bAllowTrainingAchievements, bool, "CTFGameRulesProxy", "m_bAllowTrainingAchievements") //1025
	NETVAR(m_bIsWaitingForTrainingContinue, bool, "CTFGameRulesProxy", "m_bIsWaitingForTrainingContinue") //1026
	NETVAR(m_bIsTrainingHUDVisible, bool, "CTFGameRulesProxy", "m_bIsTrainingHUDVisible") //1027
	NETVAR(m_bIsInItemTestingMode, bool, "CTFGameRulesProxy", "m_bIsInItemTestingMode") //1028
	NETVAR(m_hBonusLogic, int, "CTFGameRulesProxy", "m_hBonusLogic") //1044
	NETVAR(m_bPlayingKoth, bool, "CTFGameRulesProxy", "m_bPlayingKoth") //1048
	NETVAR(m_bPlayingMedieval, bool, "CTFGameRulesProxy", "m_bPlayingMedieval") //1051
	NETVAR(m_bPlayingHybrid_CTF_CP, bool, "CTFGameRulesProxy", "m_bPlayingHybrid_CTF_CP") //1052
	NETVAR(m_bPlayingSpecialDeliveryMode, bool, "CTFGameRulesProxy", "m_bPlayingSpecialDeliveryMode") //1053
	NETVAR(m_bPlayingRobotDestructionMode, bool, "CTFGameRulesProxy", "m_bPlayingRobotDestructionMode") //1050
	NETVAR(m_hRedKothTimer, int, "CTFGameRulesProxy", "m_hRedKothTimer") //1076
	NETVAR(m_hBlueKothTimer, int, "CTFGameRulesProxy", "m_hBlueKothTimer") //1080
	NETVAR(m_nMapHolidayType, int, "CTFGameRulesProxy", "m_nMapHolidayType") //1084
	NETVAR(m_itHandle, int, "CTFGameRulesProxy", "m_itHandle") //1844
	NETVAR(m_bPlayingMannVsMachine, bool, "CTFGameRulesProxy", "m_bPlayingMannVsMachine") //1054
	NETVAR(m_hBirthdayPlayer, int, "CTFGameRulesProxy", "m_hBirthdayPlayer") //1848
	NETVAR(m_nBossHealth, int, "CTFGameRulesProxy", "m_nBossHealth") //1832
	NETVAR(m_nMaxBossHealth, int, "CTFGameRulesProxy", "m_nMaxBossHealth") //1836
	NETVAR(m_fBossNormalizedTravelDistance, int, "CTFGameRulesProxy", "m_fBossNormalizedTravelDistance") //1840
	NETVAR(m_bMannVsMachineAlarmStatus, bool, "CTFGameRulesProxy", "m_bMannVsMachineAlarmStatus") //1055
	NETVAR(m_bHaveMinPlayersToEnableReady, bool, "CTFGameRulesProxy", "m_bHaveMinPlayersToEnableReady") //1056
	NETVAR(m_bBountyModeEnabled, bool, "CTFGameRulesProxy", "m_bBountyModeEnabled") //1057
	NETVAR(m_nHalloweenEffect, int, "CTFGameRulesProxy", "m_nHalloweenEffect") //1852
	NETVAR(m_fHalloweenEffectStartTime, float, "CTFGameRulesProxy", "m_fHalloweenEffectStartTime") //1856
	NETVAR(m_fHalloweenEffectDuration, float, "CTFGameRulesProxy", "m_fHalloweenEffectDuration") //1860
	NETVAR(m_halloweenScenario, int, "CTFGameRulesProxy", "m_halloweenScenario") //1864
	NETVAR(m_bHelltowerPlayersInHell, bool, "CTFGameRulesProxy", "m_bHelltowerPlayersInHell") //1069
	NETVAR(m_bIsUsingSpells, bool, "CTFGameRulesProxy", "m_bIsUsingSpells") //1070
	NETVAR(m_bCompetitiveMode, bool, "CTFGameRulesProxy", "m_bCompetitiveMode") //1058
	NETVAR(m_nMatchGroupType, int, "CTFGameRulesProxy", "m_nMatchGroupType") //1064
	NETVAR(m_bMatchEnded, bool, "CTFGameRulesProxy", "m_bMatchEnded") //1068
	NETVAR(m_bPowerupMode, bool, "CTFGameRulesProxy", "m_bPowerupMode") //1049
	NETVAR(m_pszCustomUpgradesFile, const char*, "CTFGameRulesProxy", "m_pszCustomUpgradesFile") //1088
	NETVAR(m_bTruceActive, bool, "CTFGameRulesProxy", "m_bTruceActive") //1071
	NETVAR(m_bShowMatchSummary, bool, "CTFGameRulesProxy", "m_bShowMatchSummary") //1348
	NETVAR(m_bMapHasMatchSummaryStage, bool, "CTFGameRulesProxy", "m_bMapHasMatchSummaryStage") //1349
	NETVAR(m_bPlayersAreOnMatchSummaryStage, bool, "CTFGameRulesProxy", "m_bPlayersAreOnMatchSummaryStage") //1350
	NETVAR(m_bStopWatchWinner, bool, "CTFGameRulesProxy", "m_bStopWatchWinner") //1351
	NETVAR(m_ePlayerWantsRematch, void*, "CTFGameRulesProxy", "m_ePlayerWantsRematch") //1352
	NETVAR(m_eRematchState, int, "CTFGameRulesProxy", "m_eRematchState") //1488
	NETVAR(m_nNextMapVoteOptions, void*, "CTFGameRulesProxy", "m_nNextMapVoteOptions") //1492
	NETVAR(m_nForceUpgrades, int, "CTFGameRulesProxy", "m_nForceUpgrades") //1868
	NETVAR(m_nForceEscortPushLogic, int, "CTFGameRulesProxy", "m_nForceEscortPushLogic") //1872
	NETVAR(m_bRopesHolidayLightsAllowed, bool, "CTFGameRulesProxy", "m_bRopesHolidayLightsAllowed") //1073

	inline bool IsPlayerReady(int playerIndex)
	{
		if (playerIndex > 101)
			return false;

		constexpr auto offset = 258;
		bool* ReadyStatus = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + offset);
		if (!ReadyStatus)
			return false;

		return ReadyStatus[playerIndex];
	}

	inline gamerules_roundstate_t GetState()
	{
		return *reinterpret_cast<gamerules_roundstate_t*>(m_iRoundState());
	}
};

class CTFGameRules
{
public:
	CTFGameRulesProxy* GetProxy()
	{
		return reinterpret_cast<CTFGameRulesProxy*>(this);
	}

	CTFGameRules* Get()
	{
		return **reinterpret_cast<CTFGameRules***>(S::CTFGameRules_Get());
	}
};