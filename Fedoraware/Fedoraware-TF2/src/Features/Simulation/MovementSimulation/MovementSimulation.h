#pragma once

#include "../../Feature.h"

// Represents data about a player.
struct PlayerData
{
	Vec3 m_vecOrigin = {};
	Vec3 m_vecVelocity = {};
	Vec3 m_vecBaseVelocity = {};
	Vec3 m_vecViewOffset = {};
	int m_hGroundEntity = -1;
	int m_fFlags = 0;
	float m_flDucktime = 0.f;
	float m_flDuckJumpTime = 0.f;
	bool m_bDucked = false;
	bool m_bDucking = false;
	bool m_bInDuckJump = false;
	float m_flModelScale = 0.f;
	int m_nButtons = 0;
	float m_flLastMovementStunChange = 0.f;
	float m_flStunLerpTarget = 0.f;
	bool m_bStunNeedsFadeOut = false;
	float m_flPrevTauntYaw = 0.f;
	float m_flTauntYaw = 0.f;
	float m_flCurrentTauntMoveSpeed = 0.f;
	int m_iKartState = 0;
	float m_flVehicleReverseTime = 0.f;
	float m_flHypeMeter = 0.f;
	float m_flMaxspeed = 0.f;
	int m_nAirDucked = 0;
	bool m_bJumping = false;
	int m_iAirDash = 0;
	float m_flWaterJumpTime = 0.f;
	float m_flSwimSoundTime = 0.f;
	int m_surfaceProps = 0;
	void* m_pSurfaceData = nullptr;
	float m_surfaceFriction = 0.f;
	char m_chTextureType = 0;
	Vec3 m_vecPunchAngle = {};
	Vec3 m_vecPunchAngleVel = {};
	float m_flJumpTime = 0.f;
	byte m_MoveType = 0;
	byte m_MoveCollide = 0;
	Vec3 m_vecLadderNormal = {};
	float m_flGravity = 0.f;
	byte m_nWaterLevel = 0;
	byte m_nWaterType = 0;
	float m_flFallVelocity = 0.f;
	int m_nPlayerCond = 0;
	int m_nPlayerCondEx = 0;
	int m_nPlayerCondEx2 = 0;
	int m_nPlayerCondEx3 = 0;
	int m_nPlayerCondEx4 = 0;
	int _condition_bits = 0;
};

// Represents data about a player that is stored and restored during movement simulation.
struct PlayerStorage
{
	CBaseEntity* m_pPlayer = nullptr;
	CMoveData m_MoveData = {};
	PlayerData m_PlayerData = {};

	float m_flAverageYaw = 0.f;

	std::deque<std::pair<Vec3, Vec3>> PredictionLines;

	bool m_bFailed = false;
	bool m_bInitFailed = false;

	// Clears the PredictionLines deque.
	void ClearPredictionLines()
	{
		PredictionLines.clear();
	}

	// Updates the m_flAverageYaw variable.
	void UpdateAverageYaw()
	{
		// Calculate the average yaw here.
	}

	// Constructor
	PlayerStorage()
	{
		ClearPredictionLines();
	}

	// Destructor
	~PlayerStorage()
	{
		// Deallocate memory here if necessary.
	}
};

// Represents a movement simulation.
class CMovementSimulation
{
private:
	void Store(PlayerStorage& playerStorage);
	void Reset(PlayerStorage& playerStorage);

	bool SetupMoveData(PlayerStorage& playerStorage);
	bool GetYawDifference(const std::deque<std::pair<Vec3, float>>& mPositionRecords, const int i, float* flYaw);
	float GetAverageYaw(const int iIndex, const int iSamples);
	bool StrafePrediction(PlayerStorage& playerStorage, const int iSamples);

	bool m_bOldInPrediction = false;
	bool m_bOldFirstTimePredicted = false;
	float m_flOldFrametime = 0.f;

	std::map<int, std::deque<std::pair<Vec3, float>>> mVelocities;

public:
	// Fills the mVelocities map with data.
	void FillVelocities();

	// Initializes the movement simulation.
	bool Initialize(CBaseEntity* pPlayer, PlayerStorage& playerStorageOut, bool useHitchance = true, bool cancelStrafe = false);

	// Runs a tick of the movement simulation.
	void RunTick(PlayerStorage& playerStorage);

	// Restores the player's state after the movement simulation.
	void Restore(PlayerStorage& playerStorage);

	// Checks if the player is on the ground.
	bool IsPlayerOnGround(const PlayerData& playerData)
	{
		// Check if the player is on the ground here.
	}

	// Updates the m_flFallVelocity variable in the player's data.
	void UpdateFallVelocity(PlayerData& playerData)
	{
		// Update the fall velocity here.
	}
};

ADD_FEATURE(CMovementSimulation, MoveSim)
