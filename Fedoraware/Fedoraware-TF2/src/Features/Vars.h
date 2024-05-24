#pragma once
#include "../SDK/SDK.h"

#include <typeinfo>

// forward declartion of ConfigVar
template <class T>
class ConfigVar;

class CVarBase
{
public:
	size_t m_iType;
	std::string m_sName;
	int m_iFlags;

	// getter for ConfigVar
	template <class T>
	ConfigVar<T>* GetVar()
	{
		if (typeid(T).hash_code() != m_iType)
			return nullptr;

		return reinterpret_cast<ConfigVar<T>*>(this);
	}
};

template <class T>
class ConfigVar : public CVarBase
{
public:
	T Default;
	T Value;
	std::unordered_map<std::string, T> Map;
	ConfigVar(T value, std::string name, int iFlags = 0);
};

inline std::vector<CVarBase*> g_Vars;

template<class T>
inline ConfigVar<T>::ConfigVar(T value, std::string name, int iFlags)
{
	Default = value;
	Value = value;
	Map["default"] = value;
	m_iType = typeid(T).hash_code();
	m_sName = name;
	g_Vars.push_back(this);
	m_iFlags = iFlags;
}

#define NAMESPACE_BEGIN(name) \
	namespace name { \
		inline std::string GetNamespace() { return "Vars::" + std::string(#name) + "::"; } \
		inline std::string GetSubname() { return ""; } \

#define SUBNAMESPACE_BEGIN(name) \
	namespace name { \
		inline std::string GetSubname() { return std::string(#name) + "::"; } \

#define NAMESPACE_END(name) \
	}
#define SUBNAMESPACE_END(name) \
	}

#define CVar(name, value, ...) inline ConfigVar<decltype(value)> name{ value, GetNamespace() + GetSubname() + std::string(#name), __VA_ARGS__ };

#define VISUAL (1 << 0)
#define NOSAVE (1 << 1)
#define NOCOND (1 << 2)

namespace Vars
{
	NAMESPACE_BEGIN(Menu)
		CVar(CheatName, std::string("SalmonPaste"), VISUAL)
		CVar(CheatPrefix, std::string("[SalmonPaste]"), VISUAL)
		CVar(MenuPrimaryKey, VK_INSERT, NOCOND)
		CVar(MenuSecondaryKey, VK_F3, NOCOND)

		CVar(ShowBinds, false)
		CVar(BindsDisplay, DragBox_t(), NOCOND)
		CVar(MenuShowsBinds, false, NOCOND)

		CVar(Indicators, 0b1)
		//CVar(SpectatorAvatars, false, VISUAL)

		CVar(TicksDisplay, DragBox_t(), NOCOND)
		CVar(CritsDisplay, DragBox_t(), NOCOND)
		CVar(SpectatorsDisplay, DragBox_t(), NOCOND)
		CVar(PingDisplay, DragBox_t(), NOCOND)
		CVar(ConditionsDisplay, DragBox_t(), NOCOND)
		CVar(SeedPredictionDisplay, DragBox_t(), NOCOND)

		CVar(DPI, 1.f, NOCOND)

		SUBNAMESPACE_BEGIN(Theme) // possibly reduce the amount of theme vars
			CVar(Accent, Color_t(255, 140, 105, 255), VISUAL)
			CVar(Background, Color_t(22, 0, 22, 255), VISUAL)
			CVar(Foreground, Color_t(44, 29, 25, 255), VISUAL)
			CVar(Foremost, Color_t(88, 58, 49, 255), VISUAL)
			CVar(Inactive, Color_t(135, 135, 135, 255), VISUAL)
			CVar(Active, Color_t(255, 255, 255, 255), VISUAL)
		SUBNAMESPACE_END(Theme)
	NAMESPACE_END(Menu)

	NAMESPACE_BEGIN(Aimbot)
		SUBNAMESPACE_BEGIN(General)
			CVar(AimType, 0) // 0 - Off, 1 - Normal, 2 - Smooth, 3 - Silent
			CVar(TargetSelection, 1) // 0 - FOV, 1 - Distance
			CVar(Target, 0b1111101) // { Bombs, NPCs, Stickies, Teleporter, Dispenser, Sentry, Players }
			CVar(Ignore, 0b11111111) // { Taunting, Disguised, Unsimulated, Vaccinator, Friends, Dead Ringer, Cloaked, Invulnerable }
			CVar(AimFOV, 90.f)
			CVar(Smoothing, 25.f)
			CVar(MaxTargets, 10)
			CVar(IgnoreCloakPercentage, 0)
			CVar(TickTolerance, 7)
			CVar(AutoShoot, true)
			CVar(FOVCircle, true)
			CVar(NoSpread, false)
		SUBNAMESPACE_END(Global)

		SUBNAMESPACE_BEGIN(Hitscan)
			CVar(Hitboxes, 0b11000) // { Legs, Arms, Body, Pelvis, Head }
			CVar(Modifiers, 0) // { Extinguish team, Bodyaim if lethal, Auto scope, Scoped only, Wait for charge, Wait for heatshot, Tapfire }
			CVar(PointScale, 0.f)
			CVar(TapFireDist, 10.f)
		SUBNAMESPACE_END(HITSCAN)

		SUBNAMESPACE_BEGIN(Projectile)
			CVar(StrafePrediction, 0b01) // { Ground, Air }
			CVar(SplashPrediction, 1) // 0 - Off, 1 - Obstructed, 2 - Prefer, 3 - Only
			CVar(AutoDetonate, 0b11) // { Flares, Stickies }
			CVar(AutoAirblast, 2) // 0 - Off, 1 - Legit, 2 - Rage
			CVar(Modifiers, 0b111) // { Bodyaim if lethal, Cancel charge, Charge weapon }
			CVar(PredictionTime, 15.f)
			CVar(Hitchance, 99.f)
			CVar(AutodetRadius, 90.f)
			CVar(SplashRadius, 90.f)
			CVar(AutoRelease, 0.f)

			CVar(iGroundSamples, 5,  ) // debug
			CVar(iAirSamples, 5,  ) // debug
			CVar(VerticalShift, 5.f,  ) // debug
			CVar(LatOff, 0.f,  ) // debug
			CVar(HullInc, 0.f,  ) // debug
			CVar(DragOverride, 0.f,  ) // debug
			CVar(TimeOverride, 0.f,  ) // debug
			CVar(HuntermanLerp, 50.f,  ) // debug
			CVar(SplashPoints, 80,  ) // debug
			CVar(SplashCount, 5,  ) // debug
		SUBNAMESPACE_END(Projectile)

		SUBNAMESPACE_BEGIN(Melee)
			CVar(AutoBackstab, true)
			CVar(IgnoreRazorback, true)
			CVar(SwingPrediction, true)
			CVar(WhipTeam, true)

			CVar(SwingTicks, 1,  ) // debug
		SUBNAMESPACE_END(Melee)
	NAMESPACE_END(AIMBOT)
	
	NAMESPACE_BEGIN(CritHack)
		CVar(ForceCrits, true)
		CVar(AvoidRandom, true)
		CVar(AlwaysMelee, true)
	NAMESPACE_END(CritHack)

	NAMESPACE_BEGIN(Backtrack)
		CVar(Enabled, true)
		CVar(PreferOnShot, true)
		CVar(Latency, 2)
		CVar(Interp, 3)
		CVar(Window, 190)

		CVar(Offset, 0,  ) // debug
	NAMESPACE_END(Backtrack)

	NAMESPACE_BEGIN(CL_Move)
		SUBNAMESPACE_BEGIN(Doubletap)
			CVar(Doubletap, true)
			CVar(Warp, false)
			CVar(RechargeTicks, true)
			CVar(AntiWarp, true)
			CVar(TickLimit, 2)
			CVar(WarpRate, 2)
			CVar(PassiveRecharge, 6)
		SUBNAMESPACE_END(DoubleTap)

		SUBNAMESPACE_BEGIN(Fakelag)
			CVar(Fakelag, 0) // 0 - Off, 1 - Plain, 2 - Random, 3 - Adaptive
			CVar(PlainTicks, 12)
			CVar(RandomTicks, IntRange_t(14, 18));
			CVar(Options, 0b000) // { While Airborne, While Unducking, While Moving }
			CVar(UnchokeOnAttack, true)
			CVar(RetainBlastJump, true)
		SUBNAMESPACE_END(FakeLag)

		CVar(AutoPeek, false)

		CVar(SpeedEnabled, false)
		CVar(SpeedFactor, 5)
	NAMESPACE_END(CL_Move)

	NAMESPACE_BEGIN(AntiHack)
		SUBNAMESPACE_BEGIN(AntiAim)
			CVar(PitchReal, 3) // 0 - None, 1 - Zero, 2 - Up, 3 - Down, 4 - Zero
			CVar(PitchFake, 2) // 0 - None, 1 - Zero, 2 - Up, 3 - Down
			CVar(YawReal, 4) // 0 - Forward, 1 - Left, 2 - Right, 3 - Backwards, 4 - Spin, 5 - Edge
			CVar(YawFake, 4) // 0 - Forward, 1 - Left, 2 - Right, 3 - Backwards, 4 - Spin, 5 - Edge
			CVar(RealYawMode, 1) // 0 - View, 1 - Target
			CVar(FakeYawMode, 1) // 0 - View, 1 - Target
			CVar(RealYawOffset, 0)
			CVar(FakeYawOffset, 0)
			CVar(SpinSpeed, 90.f)
			CVar(AntiOverlap, false)
			CVar(InvalidShootPitch, false)
		SUBNAMESPACE_END(AntiAim)

		SUBNAMESPACE_BEGIN(Resolver)
			CVar(Resolver, true)
			CVar(AutoResolveCheaters, true)
			CVar(IgnoreAirborne, false)
		SUBNAMESPACE_END(Resolver)
	NAMESPACE_END(AntiHack)

	NAMESPACE_BEGIN(CheaterDetection)
		CVar(Methods, 0b000000000)			// { Duck Speed, Aimbot, OOB Angles, Aim Flicking, Bunnyhopping, Packet Choking, Simtime Changes, Score, Accuracy }
		CVar(Protections, 0b000)			// { Timing Out, Lagging Client, Double Scans }
		CVar(SuspicionGate, 10)				// the amount of infractions prior to marking someone as a cheater
		CVar(PacketManipGate, 14)			// the avg choke for someone to receive and infraction for packet choking
		CVar(BHopMaxDelay, 1)				// max groundticks used when detecting a bunny hop.
		CVar(BHopDetectionsRequired, 5)		// how many times must some be seen bunny hopping to receive an infraction
		CVar(ScoreMultiplier, 2.f)			// multiply the avg score/s by this to receive the maximum amount
		CVar(MinimumFlickDistance, 20.f)	// min mouse flick size to suspect someone of angle cheats.
		CVar(MaximumNoise, 5.f)				// max mouse noise prior to a flick to mark somebody
		CVar(MinimumAimbotFoV, 3.f)			// scaled with how many ticks a player has choked up to ->
		CVar(MaxScaledAimbotFoV, 20.f)		// self explanatory
	NAMESPACE_END(CheaterDetection)

	NAMESPACE_BEGIN(Auto)
		SUBNAMESPACE_BEGIN(Global)
			CVar(Active, true)
		SUBNAMESPACE_END(Global)

		SUBNAMESPACE_BEGIN(Uber)
			CVar(Active, true)
			CVar(OnlyFriends, false)
			CVar(PopLocal, false)
			CVar(AutoVaccinator, 0b111) // { Fire, Blast, Bullet }
			CVar(ReactClasses, 0b000000000)
			CVar(HealthLeft, 35.0f)
			CVar(ReactFOV, 25)
			CVar(VoiceCommand, false)
		SUBNAMESPACE_END(Uber)
	NAMESPACE_END(Triggerbot)

	NAMESPACE_BEGIN(ESP)
		CVar(Draw, 2047, VISUAL)
		CVar(Player, 1048319, VISUAL)
		CVar(Building, 255, VISUAL)

		CVar(ActiveAlpha, 255, VISUAL)
		CVar(DormantAlpha, 255, VISUAL)
		CVar(DormantPriority, true, VISUAL)
		CVar(DormantTime, 100.f, VISUAL)
	NAMESPACE_END(ESP)

	NAMESPACE_BEGIN(Chams)
		SUBNAMESPACE_BEGIN(Friendly)
			CVar(Players, true, VISUAL)
			CVar(Buildings, true, VISUAL)
			CVar(Ragdolls, true, VISUAL)
			CVar(Projectiles, true, VISUAL)
			
			CVar(VisibleMaterial, std::vector<std::string> { "Original" }, VISUAL)
			CVar(OccludedMaterial, std::vector<std::string> {}, VISUAL)
			CVar(VisibleColor, Color_t(255, 140, 105, 255), VISUAL)
			CVar(OccludedColor, Color_t(255, 140, 105, 255), VISUAL)
		SUBNAMESPACE_END(Friendly)

		SUBNAMESPACE_BEGIN(Enemy)
			CVar(Players, true, VISUAL)
			CVar(Buildings, true, VISUAL)
			CVar(Ragdolls, true, VISUAL)
			CVar(Projectiles, true, VISUAL)

			CVar(VisibleMaterial, std::vector<std::string> { "Original" }, VISUAL)
			CVar(OccludedMaterial, std::vector<std::string> {}, VISUAL)
			CVar(VisibleColor, Color_t(255, 140, 105, 255), VISUAL)
			CVar(OccludedColor, Color_t(255, 140, 105, 255), VISUAL)
		SUBNAMESPACE_END(Enemy)

		SUBNAMESPACE_BEGIN(World)
			CVar(NPCs, true, VISUAL)
			CVar(Pickups, true, VISUAL)
			CVar(Bombs, true, VISUAL)
			CVar(Halloween, true, VISUAL)
		
			CVar(VisibleMaterial, std::vector<std::string> { "Original" }, VISUAL)
			CVar(OccludedMaterial, std::vector<std::string> {}, VISUAL)
			CVar(VisibleColor, Color_t(255, 140, 105, 255), VISUAL)
			CVar(OccludedColor, Color_t(255, 140, 105, 255), VISUAL)
		SUBNAMESPACE_END(World)

		SUBNAMESPACE_BEGIN(Backtrack)
			CVar(Active, true, VISUAL)
			CVar(Draw, 1, VISUAL)
				
			CVar(VisibleMaterial, std::vector<std::string> { "Original" }, VISUAL)
			CVar(OccludedMaterial, std::vector<std::string> {},  ) // unused
			CVar(VisibleColor, Color_t(255, 140, 105, 255), VISUAL)
			CVar(OccludedColor, Color_t(255, 140, 105, 255),  ) // unused
		SUBNAMESPACE_END(Backtrack)

		SUBNAMESPACE_BEGIN(FakeAngle)
			CVar(Active, true, VISUAL)
		
			CVar(VisibleMaterial, std::vector<std::string> { "Original" }, VISUAL)
			CVar(OccludedMaterial, std::vector<std::string> {},  ) // unused
			CVar(VisibleColor, Color_t(255, 140, 105, 255), VISUAL)
			CVar(OccludedColor, Color_t(255, 140, 105, 255),  ) // unused
		SUBNAMESPACE_END(FakeAngle)

		SUBNAMESPACE_BEGIN(Viewmodel)
			CVar(Weapon, true, VISUAL)
			CVar(Hands, true, VISUAL)
			
			CVar(VisibleMaterial, std::vector<std::string> { "Original" }, VISUAL)
			CVar(OccludedMaterial, std::vector<std::string> {},  ) // unused
			CVar(VisibleColor, Color_t(255, 140, 105, 255), VISUAL)
			CVar(OccludedColor, Color_t(255, 140, 105, 255),  ) // unused
		SUBNAMESPACE_END(Viewmodel)
	NAMESPACE_END(Chams)

	NAMESPACE_BEGIN(Glow)
		SUBNAMESPACE_BEGIN(Friendly)
			CVar(Players, true, VISUAL)
			CVar(Buildings, true, VISUAL)
			CVar(Ragdolls, true, VISUAL)
			CVar(Projectiles, true, VISUAL)
				
			CVar(Stencil, true, VISUAL)
			CVar(Blur, true, VISUAL)
			CVar(StencilScale, 10, VISUAL)
			CVar(BlurScale, 10, VISUAL)
		SUBNAMESPACE_END(Friendly)

		SUBNAMESPACE_BEGIN(Enemy)
			CVar(Players, true, VISUAL)
			CVar(Buildings, true, VISUAL)
			CVar(Ragdolls, true, VISUAL)
			CVar(Projectiles, true, VISUAL)
				
			CVar(Stencil, true, VISUAL)
			CVar(Blur, true, VISUAL)
			CVar(StencilScale, 10, VISUAL)
			CVar(BlurScale, 10, VISUAL)
		SUBNAMESPACE_END(Enemy)

		SUBNAMESPACE_BEGIN(World)
			CVar(NPCs, true, VISUAL)
			CVar(Pickups, true, VISUAL)
			CVar(Bombs, true, VISUAL)
			CVar(Halloween, true, VISUAL)
				
			CVar(Stencil, true, VISUAL)
			CVar(Blur, true, VISUAL)
			CVar(StencilScale, 10, VISUAL)
			CVar(BlurScale, 10, VISUAL)
		SUBNAMESPACE_END(World)

		SUBNAMESPACE_BEGIN(Backtrack)
			CVar(Active, true, VISUAL)
			CVar(Draw, 2, VISUAL)
				
			CVar(Stencil, true, VISUAL)
			CVar(Blur, true, VISUAL)
			CVar(StencilScale, 10, VISUAL)
			CVar(BlurScale, 10, VISUAL)
		SUBNAMESPACE_END(Backtrack)

		SUBNAMESPACE_BEGIN(FakeAngle)
			CVar(Active, true, VISUAL)
				
			CVar(Stencil, true, VISUAL)
			CVar(Blur, true, VISUAL)
			CVar(StencilScale, 10, VISUAL)
			CVar(BlurScale, 10, VISUAL)
		SUBNAMESPACE_END(FakeAngle)

		SUBNAMESPACE_BEGIN(Viewmodel)
			CVar(Weapon, true, VISUAL)
			CVar(Hands, true, VISUAL)

			CVar(Stencil, true, VISUAL)
			CVar(Blur, true, VISUAL)
			CVar(StencilScale, 10, VISUAL)
			CVar(BlurScale, 10, VISUAL)
		SUBNAMESPACE_END(Viewmodel)
	NAMESPACE_END(GLOW)

	NAMESPACE_BEGIN(Visuals)
		SUBNAMESPACE_BEGIN(Removals)
			CVar(Scope, true, VISUAL)
			CVar(Interpolation, true)
			CVar(Disguises, true, VISUAL)
			CVar(ScreenOverlays, true, VISUAL)
			CVar(Taunts, true, VISUAL)
			CVar(ScreenEffects, true, VISUAL)
			CVar(ViewPunch, true, VISUAL)
			CVar(AngleForcing, true, VISUAL)
			CVar(MOTD, true, VISUAL)
			CVar(ConvarQueries, true, VISUAL)
			CVar(PostProcessing, true, VISUAL)
			CVar(DSP, true, VISUAL)
		SUBNAMESPACE_END(Removals)

		SUBNAMESPACE_BEGIN(UI)
			CVar(FieldOfView, 0, VISUAL)
			CVar(ZoomFieldOfView, 0, VISUAL)
			CVar(RevealScoreboard, true, VISUAL)
			CVar(ScoreboardPlayerlist, true)
			CVar(CleanScreenshots, true)
			CVar(ScoreboardColors, true, VISUAL)
			CVar(SniperSightlines, true, VISUAL)
			CVar(PickupTimers, true, VISUAL)
		SUBNAMESPACE_END(Viewmodel)

		SUBNAMESPACE_BEGIN(Viewmodel)
			CVar(CrosshairAim, true, VISUAL)
			CVar(ViewmodelAim, true, VISUAL)
			CVar(OffsetX, 45, VISUAL)
			CVar(OffsetY, 45, VISUAL)
			CVar(OffsetZ, 45, VISUAL)
			CVar(Roll, 90, VISUAL)
			CVar(Sway, true, VISUAL)
			CVar(SwayScale, 5.f, VISUAL)
			CVar(SwayInterp, 0.05f, VISUAL)
		SUBNAMESPACE_END(Viewmodel)

		SUBNAMESPACE_BEGIN(Tracers)
			CVar(ParticleTracer, std::string("Off"), VISUAL)
			CVar(ParticleTracerCrits, std::string("Off"), VISUAL)
		SUBNAMESPACE_END(Tracers)

		SUBNAMESPACE_BEGIN(Ragdolls)
			CVar(NoRagdolls, false, VISUAL)
			CVar(NoGib, false, VISUAL)
			CVar(Active, true, VISUAL)
			CVar(EnemyOnly, true, VISUAL)
			CVar(Effects, 1, VISUAL)
			CVar(Type, 1, VISUAL)
			CVar(Force, 5.f, VISUAL)
			CVar(ForceHorizontal, 5.f, VISUAL)
			CVar(ForceVertical, 5.f, VISUAL)
		SUBNAMESPACE_END(RagdollEffects)

		SUBNAMESPACE_BEGIN(Bullet)
			CVar(BulletTracer, true, VISUAL)
		SUBNAMESPACE_END(Bullet)

		SUBNAMESPACE_BEGIN(Simulation)
			CVar(Enabled, true, VISUAL)
			CVar(Timed, true, VISUAL)
			CVar(Separators, true, VISUAL)
			CVar(SeparatorLength, 1, VISUAL)
			CVar(SeparatorSpacing, 1, VISUAL)
			CVar(ProjectileTrajectory, true, VISUAL)
			CVar(ProjectileCamera, true, VISUAL)
			CVar(ProjectileWindow, WindowBox_t(), NOCOND)
			CVar(TrajectoryOnShot, true, VISUAL)
			CVar(SwingLines, true, VISUAL)
		SUBNAMESPACE_END(ProjectileTrajectory)

		SUBNAMESPACE_BEGIN(Trajectory)
			CVar(Overwrite, false,  ) // debug
			CVar(OffX, 16.f,  ) // debug
			CVar(OffY, 8.f,  ) // debug
			CVar(OffZ, -6.f,  ) // debug
			CVar(Pipes, true,  ) // debug
			CVar(Hull, 5.f,  ) // debug
			CVar(Speed, 1200.f,  ) // debug
			CVar(Gravity, 1.f,  ) // debug
			CVar(NoSpin, false,  ) // debug
			CVar(LifeTime, 2.2f,  ) // debug
			CVar(UpVelocity, 200.f,  ) // debug
			CVar(AngVelocityX, 600.f,  ) // debug
			CVar(AngVelocityY, -1200.f,  ) // debug
			CVar(AngVelocityZ, 0.f,  ) // debug
			CVar(Drag, 1.f,  ) // debug
			CVar(DragBasisX, 0.003902f,  ) // debug
			CVar(DragBasisY, 0.009962f,  ) // debug
			CVar(DragBasisZ, 0.009962f,  ) // debug
			CVar(AngDragBasisX, 0.003618f,  ) // debug
			CVar(AngDragBasisY, 0.001514f,  ) // debug
			CVar(AngDragBasisZ, 0.001514f,  ) // debug
			CVar(MaxVelocity, k_flMaxVelocity,  ) // debug
			CVar(MaxAngularVelocity, k_flMaxAngularVelocity,  ) // debug
		SUBNAMESPACE_END(ProjectileTrajectory)

		SUBNAMESPACE_BEGIN(Hitbox)
			CVar(ShowHitboxes, true, VISUAL)
		SUBNAMESPACE_END(Hitbox)

		SUBNAMESPACE_BEGIN(ThirdPerson)
			CVar(Active, true, VISUAL)
			CVar(Distance, 1000.f, VISUAL)
			CVar(Right, 200.f, VISUAL)
			CVar(Up, 200.f, VISUAL)
			CVar(Crosshair, true, VISUAL)
		SUBNAMESPACE_END(ThirdPerson)

		SUBNAMESPACE_BEGIN(FOVArrows)
			CVar(Active, true, VISUAL)
			CVar(Offset, 250, VISUAL)
			CVar(MaxDist, 1000.f, VISUAL)
		SUBNAMESPACE_END(Arrows)

		SUBNAMESPACE_BEGIN(World)
			CVar(Modulations, 0b11111, VISUAL) // { Fog, Particle, Prop, Sky, World }
			CVar(SkyboxChanger, std::string("Off"), VISUAL)
			CVar(WorldTexture, std::string("Default"), VISUAL)
			CVar(NearPropFade, true, VISUAL)
			CVar(NoPropFade, false, VISUAL)
		SUBNAMESPACE_END(World)

		SUBNAMESPACE_BEGIN(Beams)
			CVar(Active, true, VISUAL)
			CVar(Rainbow, false, VISUAL)
			CVar(BeamColor, Color_t(255, 140, 105, 255), VISUAL)
			CVar(UseCustomModel, false, VISUAL)
			CVar(Model, std::string("sprites/physbeam.vmt"), VISUAL)
			CVar(Life, 2.f, VISUAL)
			CVar(Width, 2.f, VISUAL)
			CVar(EndWidth, 2.f, VISUAL)
			CVar(FadeLength, 10.f, VISUAL)
			CVar(Amplitude, 2.f, VISUAL)
			CVar(Brightness, 255.f, VISUAL)
			CVar(Speed, 0.2f, VISUAL)
			CVar(Flags, 65792, VISUAL)
			CVar(Segments, 2, VISUAL)
		SUBNAMESPACE_END(Beans)
	NAMESPACE_END(Visuals)

	NAMESPACE_BEGIN(Radar)
		SUBNAMESPACE_BEGIN(Main)
			CVar(Active, false, VISUAL)
			CVar(AlwaysDraw, true, VISUAL)
			CVar(Style, 1, VISUAL) // 0 - Circle, 1 - Rectangle
			CVar(Window, WindowBox_t(), NOCOND)
			CVar(Range, 2500, VISUAL)
			CVar(BackAlpha, 255, VISUAL)
			CVar(LineAlpha, 255, VISUAL)
		SUBNAMESPACE_END(Main)

		SUBNAMESPACE_BEGIN(Players)
			CVar(Active, true, VISUAL)
			CVar(Background, true, VISUAL)
			CVar(IconType, 2, VISUAL) // 0 - Icons, 1 - Portraits, 2 - Avatars
			CVar(Draw, 0b11111, VISUAL) // { Cloaked, Friends, Team, Enemy, Local }
			CVar(IconSize, 24, VISUAL)
			CVar(Health, true, VISUAL)
			CVar(Height, true, VISUAL)
		SUBNAMESPACE_END(Players)

		SUBNAMESPACE_BEGIN(Buildings)
			CVar(Active, true, VISUAL)
			CVar(Background, true, VISUAL)
			CVar(Draw, 0b1111, VISUAL) // { Friends, Team, Enemy, Local }
			CVar(Health, true, VISUAL)
			CVar(IconSize, 36, VISUAL)
		SUBNAMESPACE_END(Buildings)

		SUBNAMESPACE_BEGIN(World)
			CVar(Active, false, VISUAL)
			CVar(Background, true, VISUAL)
			CVar(Draw, 0b11111, VISUAL) // { Halloween, Bombs, Money, Ammo, Health }
			CVar(IconSize, 36, VISUAL)
		SUBNAMESPACE_END(World)
	NAMESPACE_END(Radar)

	NAMESPACE_BEGIN(Misc)
		SUBNAMESPACE_BEGIN(Movement)
			CVar(AutoStrafe, 2)
			CVar(AutoStrafeTurnScale, 0.5f)
			CVar(Bunnyhop, true)
			CVar(AutoJumpbug, true)
			CVar(AutoRocketJump, true)
			CVar(AutoCTap, true)
			CVar(FastStop, true)
			CVar(FastAccel, true)
			CVar(FastStrafe, true)
			CVar(NoPush, true)
			CVar(CrouchSpeed, true)
			

			CVar(TimingOffset, -1,  ) // debug
			CVar(ApplyAbove, 0,  ) // debug
		SUBNAMESPACE_END(Movement)

		SUBNAMESPACE_BEGIN(Exploits)
			CVar(CheatsBypass, true)
			CVar(BypassPure, true)
			CVar(PingReducer, true)
			CVar(PingTarget, 1)
			CVar(EquipRegionUnlock, true)
		SUBNAMESPACE_END(Exploits)

		SUBNAMESPACE_BEGIN(Automation)
			CVar(AntiBackstab, true)
			CVar(AntiAFK, true)
			CVar(AntiAutobalance, true)
			CVar(AcceptItemDrops, true)
			CVar(TauntControl, true)
			CVar(KartControl, true)
			CVar(BackpackExpander, false)
		SUBNAMESPACE_END(Automation)

		SUBNAMESPACE_BEGIN(Paste)
			CVar(ChatSpam, 1)
			CVar(SpamInterval, 10.f)
			CVar(Followbot, false)
			CVar(Followbotpookies, false)
			CVar(Followbotdistance, 150.f)
			CVar(MedigunSwitch, false)
			CVar(Watermark, true);
			CVar(Krillsays, true);
		SUBNAMESPACE_END(Paste)

		SUBNAMESPACE_BEGIN(Sound)
			CVar(Block, 2)
			CVar(GiantWeaponSounds, true)
		SUBNAMESPACE_END(Sound)

		SUBNAMESPACE_BEGIN(MannVsMachine)
			CVar(InstantRespawn, true)
			CVar(InstantRevive, true)
		SUBNAMESPACE_END(Sound)

		SUBNAMESPACE_BEGIN(Game)
			CVar(NetworkFix, true)
			CVar(PredictionErrorJitterFix, true)
			CVar(SetupBonesOptimization, true)
		SUBNAMESPACE_END(Game)

		SUBNAMESPACE_BEGIN(Queueing)
			CVar(ForceRegions, 5)
			CVar(FreezeQueue, true)
			CVar(AutoCasualQueue, 0)
		SUBNAMESPACE_END(Queueing)

		SUBNAMESPACE_BEGIN(Chat)
			CVar(Tags, true)
		SUBNAMESPACE_END(Chat)

		SUBNAMESPACE_BEGIN(Steam)
			CVar(EnableRPC, true)
			CVar(MatchGroup, 0) // 0 - Special Event; 1 - MvM Mann Up; 2 - Competitive; 3 - Casual; 4 - MvM Boot Camp;
			CVar(OverrideMenu, false) // Override matchgroup when in main menu
			CVar(MapText, std::string("Salmon.dv"))
			CVar(GroupSize, 1337)
		SUBNAMESPACE_END(Steam)
	NAMESPACE_END(Misc)

	NAMESPACE_BEGIN(Colors)
		CVar(FOVCircle, Color_t(255, 140, 105, 255), VISUAL)
		CVar(Relative, true, VISUAL)
		CVar(TeamRed, Color_t(255, 140, 105, 255), VISUAL)
		CVar(TeamBlu, Color_t(255, 140, 105, 255), VISUAL)
		CVar(Enemy, Color_t(255, 140, 105, 255), VISUAL)
		CVar(Team, Color_t(255, 140, 105, 255), VISUAL)
		CVar(Local, Color_t(255, 140, 105, 255), VISUAL)
		CVar(Target, Color_t(255, 140, 105, 255), VISUAL)
		CVar(Invulnerable, Color_t(255, 140, 105, 255), VISUAL)
		CVar(Cloak, Color_t(255, 140, 105, 255), VISUAL)
		CVar(Overheal, Color_t(255, 140, 105, 255), VISUAL)
		CVar(HealthBar, Gradient_t({ 255, 140, 105, 255 }, { 255, 140, 105, 0 }), VISUAL)
		CVar(UberBar, Color_t(255, 140, 105, 255), VISUAL)
		CVar(Health, Color_t(255, 140, 105, 255), VISUAL)
		CVar(Ammo, Color_t(255, 140, 105, 255), VISUAL)
		CVar(NPC, Color_t(255, 140, 105, 255), VISUAL)
		CVar(Bomb, Color_t(255, 140, 105, 255), VISUAL)
		CVar(Money, Color_t(255, 140, 105, 255), VISUAL)
		CVar(Halloween, Color_t(255, 140, 105, 255), VISUAL)

		CVar(WorldModulation, Color_t(255, 140, 105, 255), VISUAL)
		CVar(SkyModulation, Color_t(255, 140, 105, 255), VISUAL)
		CVar(PropModulation, Color_t(255, 140, 105, 255), VISUAL)
		CVar(ParticleModulation, Color_t(255, 140, 105, 255), VISUAL)
		CVar(FogModulation, Color_t(255, 140, 105, 255), VISUAL)

		CVar(BulletTracer, Color_t(255, 140, 105, 255), VISUAL)
		CVar(PredictionColor, Color_t(255, 140, 105, 255), VISUAL)
		CVar(ProjectileColor, Color_t(255, 140, 105, 255), VISUAL)
		CVar(ClippedColor, Color_t(255, 140, 105, 255), VISUAL)
		CVar(HitboxEdge, Color_t(255, 140, 105, 255), VISUAL)
		CVar(HitboxFace, Color_t(255, 140, 105, 255), VISUAL)
	NAMESPACE_END(Colors)

	NAMESPACE_BEGIN(Logging)
		CVar(Logs, 0b1111) // { Damage, Class Changes, Vote cast, Vote start, Cheat Detection, Tags }
		// LogTo // { Console, Party, Chat, Toasts }
		CVar(Lifetime, 5.f, VISUAL)

		SUBNAMESPACE_BEGIN(VoteStart)
			CVar(LogTo, 0b1111)
		SUBNAMESPACE_END(VoteStart)

		SUBNAMESPACE_BEGIN(VoteCast)
			CVar(LogTo, 0b1111)
		SUBNAMESPACE_END(VoteCast)

		SUBNAMESPACE_BEGIN(ClassChange)
			CVar(LogTo, 0b1111)
		SUBNAMESPACE_END(ClassChange)

		SUBNAMESPACE_BEGIN(Damage)
			CVar(LogTo, 0b1111)
		SUBNAMESPACE_END(Damage)

		SUBNAMESPACE_BEGIN(CheatDetection)
			CVar(LogTo, 0b1111)
		SUBNAMESPACE_END(CheatDetection)

		SUBNAMESPACE_BEGIN(Tags)
			CVar(LogTo, 0b1111)
		SUBNAMESPACE_END(Tags)
	NAMESPACE_END(Logging)

	NAMESPACE_BEGIN(Debug)
		CVar(Info, false,  )
		CVar(Logging, false,  )
		CVar(ServerHitbox, false,  )
		CVar(AntiAimLines, false,  )
	NAMESPACE_END(Debug)

	NAMESPACE_BEGIN(Fonts)
		SUBNAMESPACE_BEGIN(FONT_ESP)
			CVar(szName, std::string("Tahoma"), VISUAL | NOCOND)
			CVar(nTall, 12, VISUAL | NOCOND)
			CVar(nWeight, 0, VISUAL | NOCOND)
			CVar(nFlags, 512, VISUAL | NOCOND)
		SUBNAMESPACE_END(FONT_ESP)

		SUBNAMESPACE_BEGIN(FONT_NAME)
			CVar(szName, std::string("Tahoma"), VISUAL | NOCOND)
			CVar(nTall, 12, VISUAL | NOCOND)
			CVar(nWeight, 0, VISUAL | NOCOND)
			CVar(nFlags, 512, VISUAL | NOCOND)
		SUBNAMESPACE_END(FONT_NAME)

		SUBNAMESPACE_BEGIN(FONT_INDICATORS)
			CVar(szName, std::string("Tahoma"), VISUAL | NOCOND)
			CVar(nTall, 13, VISUAL | NOCOND)
			CVar(nWeight, -1, VISUAL | NOCOND)
			CVar(nFlags, 512, VISUAL | NOCOND)
		SUBNAMESPACE_END(FONT_INDICATORS)
	NAMESPACE_END(Fonts)
}