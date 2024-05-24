#include "Menu.h"

#include "../Vars.h"
#include "../Color.h"
#include "../Misc/Misc.h"
#include "../Visuals/Visuals.h"
#include "../Resolver/Resolver.h"

#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx9.h>
#include <ImGui/imgui_stdlib.h>

#include "Fonts/IconsMaterialDesign.h"
#include "Fonts/RobotoMedium.h"
#include "Fonts/RobotoBold.h"
#include "Fonts/RobotoBlack.h"
#include "Fonts/CascadiaMono.h"

#include "Playerlist/PlayerUtils.h"

#include "Components.hpp"
#include "ConfigManager/ConfigManager.h"
#include "Conditions/Conditions.h"
#include "../CameraWindow/CameraWindow.h"
#include "../Backtrack/Backtrack.h"

#include <mutex>

#pragma warning (disable : 4309)

int unuPrimary = 0;
int unuSecondary = 0;

constexpr auto SIZE_FULL_WIDTH = ImVec2(-1, 20);

/* The main menu */
void CMenu::DrawMenu()
{
	using namespace ImGui;

	ImVec2 mainWindowPos = {};
	ImVec2 mainWindowSize = {};

	SetNextWindowSize(ImVec2(750, 500), ImGuiCond_FirstUseEver);
	PushStyleVar(ImGuiStyleVar_WindowMinSize, { 750, 500 });
	if (Begin("MainWindow", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar))
	{
		const auto windowPos = mainWindowPos = GetWindowPos();
		const auto windowSize = mainWindowSize = GetWindowSize();

		// Main tabs
		FTabs({ "BAIMBOT", "VISCUALS", "MICS", "TOST", "OPTIONAS" }, &CurrentTab, TabSize, { 0, SubTabSize.y }, true, {ICON_MD_GROUP, ICON_MD_IMAGE, ICON_MD_PUBLIC, ICON_MD_MENU_BOOK, ICON_MD_SETTINGS});
		
		// Sub tabs
		switch (CurrentTab)
		{
		case 0: FTabs({ "GENERAL tab", "EICH VI EICH", "MANUALIZATION" }, &CurrentAimbotTab, SubTabSize, { TabSize.x, 0 }); break;
		case 1: FTabs({ "WALL HECK", "CHRMS", "GLOWIE", "MICS##", "RADIUS", "DON'T" }, &CurrentVisualsTab, SubTabSize, { TabSize.x, 0 }); break;
		case 2: FTabs({ "MICS##" }, nullptr, SubTabSize, { TabSize.x, 0 }); break;
		case 3: FTabs({ "TOST##", "OPTIONAS##" }, &CurrentLogsTab, SubTabSize, { TabSize.x, 0 }); break;
		case 4: FTabs({ "JEW PASTE", "CONDS <3", "CHEATERS", "BLOAT" }, &CurrentConfigTab, SubTabSize, { TabSize.x, 0 }); break;
		}

		// Main content
		SetCursorPos({ TabSize.x, SubTabSize.y });
		PushStyleVar(ImGuiStyleVar_WindowPadding, { 8.f, 8.f });
		PushStyleColor(ImGuiCol_ChildBg, {});
		if (BeginChild("Content", { windowSize.x - TabSize.x, windowSize.y - SubTabSize.y }, false, ImGuiWindowFlags_AlwaysUseWindowPadding))
		{
			PushStyleColor(ImGuiCol_ChildBg, Foreground.Value);
			PushStyleVar(ImGuiStyleVar_ChildRounding, 3.f);

			switch (CurrentTab)
			{
			case 0: MenuAimbot(); break;
			case 1: MenuVisuals(); break;
			case 2: MenuMisc(); break;
			case 3: MenuLogs(); break;
			case 4: MenuSettings(); break;
			}

			PopStyleVar();
			PopStyleColor();
		} EndChild();
		PopStyleColor();
		PopStyleVar();

		// End
		End();
	}
	PopStyleVar();

	// Title Text
	if (Vars::Menu::CheatName.Value != "")
	{
		PushFont(FontTitle);
		const auto textSize = CalcTextSize(Vars::Menu::CheatName.Value.c_str());
		SetNextWindowSize({ std::min(textSize.x + 26.f, mainWindowSize.x), 40.f });
		SetNextWindowPos({ mainWindowPos.x, mainWindowPos.y - 48.f });
		PushStyleVar(ImGuiStyleVar_WindowMinSize, { 40.f, 40.f });
		if (Begin("TitleWindow", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoFocusOnAppearing))
		{
			const auto windowPos = GetWindowPos();
			GetWindowDrawList()->AddText(FontTitle, FontTitle->FontSize, { windowPos.x + 13.f, windowPos.y + 10.f }, Accent, Vars::Menu::CheatName.Value.c_str());

			End();
		}
		PopStyleVar();
		PopFont();
	}

	// Condition Text
	if (sCondition != "default")
	{
		const auto textSize = CalcTextSize(std::format("Editing cond {}", sCondition).c_str());
		SetNextWindowSize({ std::min(textSize.x + 56.f, mainWindowSize.x), 40.f });
		SetNextWindowPos({ mainWindowPos.x, mainWindowPos.y + mainWindowSize.y + 8.f });
		PushStyleVar(ImGuiStyleVar_WindowMinSize, { 40.f, 40.f });
		if (Begin("ConditionWindow", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar))
		{
			const auto windowPos = GetWindowPos();
			const auto preSize = CalcTextSize("Editing cond ");
			GetWindowDrawList()->AddText(FontRegular, FontRegular->FontSize, { windowPos.x + 16.f, windowPos.y + 13.f }, Active, "Editing cond ");
			GetWindowDrawList()->AddText(FontRegular, FontRegular->FontSize, { windowPos.x + 16.f + preSize.x, windowPos.y + 13.f }, Accent, sCondition.c_str());

			SetCursorPos({ textSize.x + 28, 11 });
			if (IconButton(ICON_MD_CANCEL))
				sCondition = "default";

			End();
		}
		PopStyleVar();
	}
}

#pragma region Tabs
/* Tab: Aimbot */
void CMenu::MenuAimbot()
{
	using namespace ImGui;

	switch (CurrentAimbotTab)
	{
	// General
	case 0:
		if (BeginTable("AimbotTable", 2))
		{
			/* Column 1 */
			TableNextColumn();
			if (Section("Global"))
			{
				FDropdown("Aim type", Vars::Aimbot::General::AimType, { "On", "Shrimple", "Smooth", "Silent" }, {}, FDropdown_Left);
				FDropdown("How to aim", Vars::Aimbot::General::TargetSelection, { "by FOV", "by Distance" }, {}, FDropdown_Right);
				FDropdown("who to shoot", Vars::Aimbot::General::Target, { "entities", "hostile architecture", "passive architecture", "transport", "traps", "NPCs", "ICBMs" }, {}, FDropdown_Multi | FDropdown_Left);
				FDropdown("Ingore", Vars::Aimbot::General::Ignore, { "can't kill", "hide", "hide when shot", "Vacc", "unsimutaled pasltes", "teammates but no", "kill these fo sho" }, {}, FDropdown_Multi | FDropdown_Right);
				FSlider("how big is your skill issue?", Vars::Aimbot::General::AimFOV, 1.f, 180.f, 1.f, "%.0f", FSlider_Clamp);
				bTransparent = FGet(Vars::Aimbot::General::AimType) != 2;
					FSlider("Smoothing## Hitscan", Vars::Aimbot::General::Smoothing, 0.f, 100.f, 1.f, "%.0f%%", FSlider_Clamp);
				bTransparent = false;
				FSlider("targets maks", Vars::Aimbot::General::MaxTargets, 1, 6, 1, "%d", FSlider_Clamp);
				bTransparent = !(FGet(Vars::Aimbot::General::Ignore) & 1 << 1);
					FSlider("Ingore cloaka", Vars::Aimbot::General::IgnoreCloakPercentage, 0, 100, 10, "%d%%", FSlider_Clamp);
				bTransparent = false;
				bTransparent = !(FGet(Vars::Aimbot::General::Ignore) & 1 << 4);
					FSlider("tick intolerant", Vars::Aimbot::General::TickTolerance, 0, 21, 1, "%d", FSlider_Clamp);
				bTransparent = false;
				FColorPicker("shot FOV coloUr", Vars::Colors::FOVCircle);
				FToggle("manualshot", Vars::Aimbot::General::AutoShoot);
				FToggle("shot FOV", Vars::Aimbot::General::FOVCircle, FToggle_Middle);
				FToggle("Crithack", Vars::CritHack::ForceCrits);
				FToggle("off real crits", Vars::CritHack::AvoidRandom, FToggle_Middle);
				FToggle("melee 24/7", Vars::CritHack::AlwaysMelee);
				FToggle("mantissa steps", Vars::Aimbot::General::NoSpread, FToggle_Middle);
			} EndSection();
			if (Section("russian mobilization"))
			{
				FToggle("yes?", Vars::Backtrack::Enabled);
				FToggle("prefer on atak", Vars::Backtrack::PreferOnShot, FToggle_Middle);
				FSlider("future track", Vars::Backtrack::Latency, 0, F::Backtrack.flMaxUnlag * 1000, 5, "%d", FSlider_Clamp); // unreliable above 900
				FSlider("Back track", Vars::Backtrack::Interp, 0, F::Backtrack.flMaxUnlag * 1000, 5, "%d", FSlider_Clamp);
				FSlider("Windows 10 Pro", Vars::Backtrack::Window, 1, 200, 5, "%d", FSlider_Clamp);
			} EndSection();
			if (Vars::Debug::Info.Value)
			{
				if (Section("debug## backtrack"))
				{
					FSlider("offset", Vars::Backtrack::Offset, -5, 5);
				} EndSection();
			}

			/* Column 2 */
			TableNextColumn();
			if (Section("shrimple weapons"))
			{
				FDropdown("where aim", Vars::Aimbot::Hitscan::Hitboxes, { "Hed", "Baim", "Penis", "Fingers", "Feet" }, { 1 << 0, 1 << 2, 1 << 1, 1 << 3, 1 << 4 }, FDropdown_Multi);
				FDropdown("Modifiers## Hitscan", Vars::Aimbot::Hitscan::Modifiers, { "Tapping niggaz", "wait for HEADSHOTTTT", "Wait for uber", "only when gamer 1337 mode", "Auto gamer 1337 mode", "ultimate baim death", "cooldown nigga!" }, {}, FDropdown_Multi);
				FSlider("points", Vars::Aimbot::Hitscan::PointScale, 0.f, 100.f, 5.f, "%.0f%%", FSlider_Clamp | FSlider_Precision);
				bTransparent = !(FGet(Vars::Aimbot::Hitscan::Modifiers) & 1 << 0);
					FSlider("tapping niggaz v2", Vars::Aimbot::Hitscan::TapFireDist, 250.f, 1000.f, 50.f, "%.0f", FSlider_Clamp | FSlider_Precision);
				bTransparent = false;
			} EndSection();
			if (Section("legitard proof weapons"))
			{
				FDropdown("Predict", Vars::Aimbot::Projectile::StrafePrediction, { "in Air", "on Ground" }, {}, FDropdown_Multi | FDropdown_Left);
				FDropdown("Splashy Splashy", Vars::Aimbot::Projectile::SplashPrediction, { "On", "Include", "Prefer", "Only" }, {}, FDropdown_Right);
				FDropdown("Auto detonate", Vars::Aimbot::Projectile::AutoDetonate, { "Stickies", "Flares" }, {}, FDropdown_Multi | FDropdown_Left);
				FDropdown("Auto airblast", Vars::Aimbot::Projectile::AutoAirblast, { "On", "Legit", "Rage" }, {}, FDropdown_Right);
				FDropdown("Modifiers## Projectile", Vars::Aimbot::Projectile::Modifiers, { "Charge shot", "Cancel charge", "Bodyaim if lethal" }, {}, FDropdown_Multi);
				FSlider("Max simulation time", Vars::Aimbot::Projectile::PredictionTime, 0.1f, 10.f, 0.1f, "%.1fs");
				bTransparent = !FGet(Vars::Aimbot::Projectile::StrafePrediction);
					FSlider("Hit chance", Vars::Aimbot::Projectile::Hitchance, 0.f, 100.f, 5.f, "%.0f%%", FSlider_Clamp | FSlider_Precision);
				bTransparent = false;
				FSlider("Autodet radio", Vars::Aimbot::Projectile::AutodetRadius, 0.f, 100.f, 5.f, "%.0f%%", FSlider_Clamp | FSlider_Precision);
				FSlider("Splash radio", Vars::Aimbot::Projectile::SplashRadius, 0.f, 100.f, 5.f, "%.0f%%", FSlider_Clamp | FSlider_Precision);
				bTransparent = !FGet(Vars::Aimbot::Projectile::AutoRelease);
					FSlider("Auto cum", Vars::Aimbot::Projectile::AutoRelease, 0.f, 100.f, 5.f, "%.0f%%", FSlider_Clamp | FSlider_Precision);
				bTransparent = false;
			} EndSection();
			if (Vars::Debug::Info.Value)
			{
				if (Section("debug## projectile"))
				{
					FSlider("ground samples", Vars::Aimbot::Projectile::iGroundSamples, 3, 66, 1, "%d", FSlider_Left);
					FSlider("air samples", Vars::Aimbot::Projectile::iAirSamples, 3, 66, 1, "%d", FSlider_Right);
					FSlider("vert shift", Vars::Aimbot::Projectile::VerticalShift, 0.f, 20.f, 0.5f, "%.1f", FSlider_Left);
					FSlider("hunterman lerp", Vars::Aimbot::Projectile::HuntermanLerp, 0.f, 100.f, 1.f, "%.0f%%", FSlider_Right);
					FSlider("latency offset", Vars::Aimbot::Projectile::LatOff, -1.f, 1.f, 0.1f, "%.1f", FSlider_Left);
					FSlider("hull inc", Vars::Aimbot::Projectile::HullInc, 0.f, 3.f, 0.5f, "%.1f", FSlider_Right);
					FSlider("drag override", Vars::Aimbot::Projectile::DragOverride, 0.f, 1.f, 0.001f, "%.3f", FSlider_Left);
					FSlider("time override", Vars::Aimbot::Projectile::TimeOverride, 0.f, 1.f, 0.001f, "%.3f", FSlider_Right);
					FSlider("splash points", Vars::Aimbot::Projectile::SplashPoints, 0, 100, 1, "%d", FSlider_Left);
					FSlider("splash count", Vars::Aimbot::Projectile::SplashCount, 1, 5, 1, "%d", FSlider_Right);
				} EndSection();
			}
			if (Section("epik 100% crit moment"))
			{
				FToggle("stabby stabby", Vars::Aimbot::Melee::AutoBackstab);
				FToggle("Ingore razorback", Vars::Aimbot::Melee::IgnoreRazorback, FToggle_Middle);
				FToggle("melee pred", Vars::Aimbot::Melee::SwingPrediction);
				FToggle("Whip niggers", Vars::Aimbot::Melee::WhipTeam, FToggle_Middle);
			} EndSection();
			if (Vars::Debug::Info.Value)
			{
				if (Section("debug## melee"))
					FSlider("swing ticks", Vars::Aimbot::Melee::SwingTicks, 10, 14);
				EndSection();
			}

			EndTable();
		}
		break;
	// HvH
	case 1:
		if (BeginTable("HvHTable", 2))
		{
			/* Column 1 */
			TableNextColumn();
			if (Section("Quadrupletap"))
			{
				FToggle("Quadrupletap", Vars::CL_Move::Doubletap::Doubletap);
				FToggle("Teleport", Vars::CL_Move::Doubletap::Warp, FToggle_Middle);
				FToggle("Recharge ticks", Vars::CL_Move::Doubletap::RechargeTicks);
				FToggle("Anti-Teleport", Vars::CL_Move::Doubletap::AntiWarp, FToggle_Middle);
				FSlider("Tick limit", Vars::CL_Move::Doubletap::TickLimit, 2, 22, 1, "%d", FSlider_Clamp);
				FSlider("Teleport rate", Vars::CL_Move::Doubletap::WarpRate, 2, 22, 1, "%d", FSlider_Clamp);
				FSlider("Passive recharge", Vars::CL_Move::Doubletap::PassiveRecharge, 0, 66, 1, "%d", FSlider_Clamp);
			} EndSection();
			if (Section("Reallag"))
			{
				FDropdown("Reallag", Vars::CL_Move::Fakelag::Fakelag, { "On", "Shrimple", "math.random", "Adaptive" }, {}, FSlider_Left);
				FDropdown("Options", Vars::CL_Move::Fakelag::Options, { "Only moving", "On unduck", "Not airborne" }, {}, FDropdown_Multi | FSlider_Right);
				bTransparent = FGet(Vars::CL_Move::Fakelag::Fakelag) != 1;
					FSlider("Plain ticks", Vars::CL_Move::Fakelag::PlainTicks, 1, 22, 1, "%d", FSlider_Clamp | FSlider_Left);
				bTransparent = FGet(Vars::CL_Move::Fakelag::Fakelag) != 2;
					FSlider("Random ticks", Vars::CL_Move::Fakelag::RandomTicks, 1, 22, 1, "%d - %d", FSlider_Clamp | FSlider_Right);
				bTransparent = false;
				FToggle("Unchoke on attack", Vars::CL_Move::Fakelag::UnchokeOnAttack);
				FToggle("Retain blastjump", Vars::CL_Move::Fakelag::RetainBlastJump, FToggle_Middle);
			} EndSection();
			if (Section("spinnnnn :3"))
			{
				FDropdown("spin option1", Vars::AntiHack::AntiAim::PitchReal, { "Yes", "Yes", "Yes", "Yes" }, {}, FDropdown_Left);
				FDropdown("spin option2", Vars::AntiHack::AntiAim::PitchFake, { "Yes", "Yes", "Yes" }, {}, FDropdown_Right);
				FDropdown("spin option3", Vars::AntiHack::AntiAim::YawReal, { "Yes", "Yes", "Yes", "Yes", "Yes", "Yes"}, {}, FDropdown_Left);
				FDropdown("spin option4", Vars::AntiHack::AntiAim::YawFake, { "Yes", "Yes", "Yes", "Yes", "Yes", "Yes"}, {}, FDropdown_Right);
				FDropdown("spin option5", Vars::AntiHack::AntiAim::RealYawMode, { "Yes", "Yes" }, {}, FDropdown_Left);
				FDropdown("spin option6", Vars::AntiHack::AntiAim::FakeYawMode, { "Yes", "Yes" }, {}, FDropdown_Right);
				FSlider("spin option## Offset1", Vars::AntiHack::AntiAim::RealYawOffset, -180, 180, 5, "%d", FSlider_Left | FSlider_Clamp | FSlider_Precision);
				FSlider("spin option## Offset2", Vars::AntiHack::AntiAim::FakeYawOffset, -180, 180, 5, "%d", FSlider_Right | FSlider_Clamp | FSlider_Precision);
				bTransparent = FGet(Vars::AntiHack::AntiAim::YawFake) != 4 && FGet(Vars::AntiHack::AntiAim::YawReal) != 4;
					FSlider("spin Speed", Vars::AntiHack::AntiAim::SpinSpeed, -30.f, 30.f, 1.f, "%.0f", FSlider_Left);
				bTransparent = false;
				SetCursorPos({ GetWindowSize().x / 2 + 4, GetCursorPosY() - 24 });
				FToggle("Anti-overlap", Vars::AntiHack::AntiAim::AntiOverlap);
				FToggle("Hide pitch on shot", Vars::AntiHack::AntiAim::InvalidShootPitch, FToggle_Middle);
			} EndSection();

			/* Column 2 */
			TableNextColumn();
			if (Section("math solver"))
			{
				FToggle("No", Vars::AntiHack::Resolver::Resolver);
				bTransparent = !FGet(Vars::AntiHack::Resolver::Resolver);
				FToggle("Ingore in-air", Vars::AntiHack::Resolver::IgnoreAirborne, FToggle_Middle);
				bTransparent = false;
			} EndSection();
			if (Section("Auto Pee"))
			{
				FToggle("Auto pee", Vars::CL_Move::AutoPeek);
			} EndSection();
			if (Section("MethHack"))
			{
				FToggle("MethHack", Vars::CL_Move::SpeedEnabled);
				bTransparent = !FGet(Vars::CL_Move::SpeedEnabled);
				FSlider("MethHack factor", Vars::CL_Move::SpeedFactor, 1, 50, 1);
				bTransparent = false;
			} EndSection();

			EndTable();
		}
		break;
	// Trigger
	case 2:
		if (BeginTable("TriggerTable", 2))
		{
			/* Column 1 */
			TableNextColumn();
			if (Section("Global"))
			{
				FToggle("OFF", Vars::Auto::Global::Active);
			} EndSection();
			if (Section("Autouber"))
			{
				FToggle("OFF", Vars::Auto::Uber::Active);
				FToggle("Activate charge trigger", Vars::Auto::Uber::VoiceCommand, FToggle_Middle);
				FToggle("Uber pookie only", Vars::Auto::Uber::OnlyFriends);
				FToggle("egoism", Vars::Auto::Uber::PopLocal, FToggle_Middle);
				FDropdown("Auto better uber", Vars::Auto::Uber::AutoVaccinator, { "Boolet", "BUM BUM", "PyroFag" }, {}, FDropdown_Multi | FDropdown_Left);
				FDropdown("histcan classes according to you", Vars::Auto::Uber::ReactClasses, { "Scunt", "projectile", "PyroFag", "sandviscg", "Engibor", "bot", "french" }, { 1 << 0, 1 << 1, 1 << 2, 1 << 4, 1 << 5, 1 << 7, 1 << 8 }, FDropdown_Multi | FDropdown_Right);
				FSlider("DEATH", Vars::Auto::Uber::HealthLeft, 1.f, 99.f, 5.f, "%.0f%%", FSlider_Clamp | FSlider_Left);
				FSlider("blind?", Vars::Auto::Uber::ReactFOV, 0, 90, 1, "%d", FSlider_Clamp | FSlider_Right);
			} EndSection();

			/* Column 2 */
			TableNextColumn();

			EndTable();
		}
	}
}

/* Tab: Visuals */
void CMenu::MenuVisuals()
{
	using namespace ImGui;

	switch (CurrentVisualsTab)
	{
	// ESP
	case 0:
		if (BeginTable("VisualsESPTable", 2))
		{
			/* Column 1 */
			TableNextColumn();
			if (Section("IBS"))
			{
				FDropdown("make art", Vars::ESP::Draw, { "guys", "other guys", "Frens :3", "you!", "NPCs", "Death", "4mm0", "Ca$h", "ICBMs", "m4g1c", "halloween shit" }, {}, FDropdown_Multi);
				FDropdown("mor IBS shit", Vars::ESP::Player, { "N1ck", "Death mode", "Death text", "charge bar", "yes/no text", "Class icon", "u blind?", "bloat", "bloat", "long shot meter", "yucky", "Bonerz", "Tags", "Labels", "Booffs", "Debooffs", "Miscanelsus", "is this nigga cheating?", "miliseconds", "high kills, low healths" }, {}, FDropdown_Multi);
				FDropdown("architecture", Vars::ESP::Building, { "N1ck", "Death mode", "Death text", "long shot meter", "yucky", "ultimate owneg", "casual level", "Conditions" }, {}, FDropdown_Multi);
			} EndSection();

			/* Column 2 */
			TableNextColumn();
			if (Section("ColoUrs"))
			{
				FToggle("Relative coloUrs", Vars::Colors::Relative);
				if (FGet(Vars::Colors::Relative))
				{
					FColorPicker("guys coloUr", Vars::Colors::Enemy, 0, FColorPicker_Left);
					FColorPicker("other guys coloUr", Vars::Colors::Team, 0, FColorPicker_Middle | FColorPicker_SameLine);
				}
				else
				{
					FColorPicker("YELLOW coloUr", Vars::Colors::TeamRed, 0, FColorPicker_Left);
					FColorPicker("GREEN coloUr", Vars::Colors::TeamBlu, 0, FColorPicker_Middle | FColorPicker_SameLine);
				}

				FColorPicker("Death mode is top", Vars::Colors::HealthBar, false, 0, FColorPicker_Left);
				FColorPicker("Death mode is bottom", Vars::Colors::HealthBar, true, 0, FColorPicker_Middle | FColorPicker_SameLine);
				FColorPicker("charge me doctor", Vars::Colors::UberBar, 0, FColorPicker_Left);
				FColorPicker("fucking uber", Vars::Colors::Invulnerable, 0, FColorPicker_Middle | FColorPicker_SameLine);
				FColorPicker("over dead coloUr", Vars::Colors::Overheal, 0, FColorPicker_Left);
				FColorPicker("spy claork", Vars::Colors::Cloak, 0, FColorPicker_Middle | FColorPicker_SameLine);
				FColorPicker("self IBS", Vars::Colors::Local, 0, FColorPicker_Left);
				FColorPicker("aimbotik coloUr", Vars::Colors::Target, 0, FColorPicker_Middle | FColorPicker_SameLine);

				FColorPicker("pills", Vars::Colors::Health, 0, FColorPicker_Left);
				FColorPicker("boolets", Vars::Colors::Ammo, 0, FColorPicker_Middle | FColorPicker_SameLine);
				FColorPicker("PC coloUr", Vars::Colors::NPC, 0, FColorPicker_Left);
				FColorPicker("ICMB coloUr", Vars::Colors::Bomb, 0, FColorPicker_Middle | FColorPicker_SameLine);
				FColorPicker("$$$", Vars::Colors::Money, 0, FColorPicker_Left);
				FColorPicker("Hallow33n", Vars::Colors::Halloween, 0, FColorPicker_Middle | FColorPicker_SameLine);

				FSlider("see if exists", Vars::ESP::ActiveAlpha, 0, 255, 5, "%d", FSlider_Clamp);
				FSlider("see if doesn't exist", Vars::ESP::DormantAlpha, 0, 255, 5, "%d", FSlider_Clamp);
				FSlider("how long if doesn't exist", Vars::ESP::DormantTime, 0.015f, 5.0f, 0.1f, "%.1f", FSlider_Left | FSlider_Clamp);
				FToggle("doesn't exist prior", Vars::ESP::DormantPriority, FToggle_Middle); Dummy({ 0, 8 });
			} EndSection();

			EndTable();
		}
		break;
	// Chams
	case 1:
		if (BeginTable("VisualsChamsTable", 2))
		{
			/* Column 1 */
			TableNextColumn();
			if (Section("not your enemies"))
			{
				FToggle("entities", Vars::Chams::Friendly::Players);
				FToggle("DEATH", Vars::Chams::Friendly::Ragdolls, FToggle_Middle);
				FToggle("architecture", Vars::Chams::Friendly::Buildings);
				FToggle("o kurwa rakietaaaaa", Vars::Chams::Friendly::Projectiles, FToggle_Middle);

				FMDropdown("matoriel if vosobvi", Vars::Chams::Friendly::VisibleMaterial, FDropdown_Left, 1);
				FColorPicker("coloUr if vosibo", Vars::Chams::Friendly::VisibleColor, 0, FColorPicker_Dropdown);
				FMDropdown("hidden team matoriel", Vars::Chams::Friendly::OccludedMaterial, FDropdown_Right, 1);
				FColorPicker("hidden team coloUr", Vars::Chams::Friendly::OccludedColor, 0, FColorPicker_Dropdown);
			} EndSection();
			if (Section("guys"))
			{
				FToggle("entities", Vars::Chams::Enemy::Players);
				FToggle("LIFE", Vars::Chams::Enemy::Ragdolls, FToggle_Middle);
				FToggle("hostile architecture smh", Vars::Chams::Enemy::Buildings);
				FToggle("rockets", Vars::Chams::Enemy::Projectiles, FToggle_Middle);

				FMDropdown("matoriel if vosobvi", Vars::Chams::Enemy::VisibleMaterial, FDropdown_Left, 1);
				FColorPicker("coloUr if vosibo", Vars::Chams::Enemy::VisibleColor, 0, FColorPicker_Dropdown);
				FMDropdown("hidden nigga matoriel", Vars::Chams::Enemy::OccludedMaterial, FDropdown_Right, 1);
				FColorPicker("hidden nigga coloUr", Vars::Chams::Enemy::OccludedColor, 0, FColorPicker_Dropdown);
			} EndSection();
			if (Section("text editor"))
			{
				FToggle("Personal Computer", Vars::Chams::World::NPCs);
				FToggle("Pickdown", Vars::Chams::World::Pickups, FToggle_Middle);
				FToggle("ICBMs", Vars::Chams::World::Bombs);
				FToggle("Helloweneer", Vars::Chams::World::Halloween, FToggle_Middle);

				FMDropdown("matoriel if vosobvi", Vars::Chams::World::VisibleMaterial, FDropdown_Left, 1);
				FColorPicker("coloUr if vosibo", Vars::Chams::World::VisibleColor, 0, FColorPicker_Dropdown);
				FMDropdown("hidden nigga matoriel", Vars::Chams::World::OccludedMaterial, FDropdown_Right, 1);
				FColorPicker("hidden nigga coloUr", Vars::Chams::World::OccludedColor, 0, FColorPicker_Dropdown);
			} EndSection();

			/* Column 2 */
			TableNextColumn();
			if (Section("tf2 cheating wtf"))
			{
				FToggle("Disablitiy", Vars::Chams::Backtrack::Active);
				SameLine(GetWindowSize().x / 2 + 4); SetCursorPosY(GetCursorPosY() - 24);
				FDropdown("make art", Vars::Chams::Backtrack::Draw, { "first", "first + random one", "Allah" }, {}, FDropdown_Left);

				FMDropdown("Maeroeial", Vars::Chams::Backtrack::VisibleMaterial, FDropdown_None, 1);
				FColorPicker("ColoUr", Vars::Chams::Backtrack::VisibleColor, 0, FColorPicker_Dropdown);
			} EndSection();
			if (Section("Real Angel"))
			{
				FToggle("guess", Vars::Chams::FakeAngle::Active);

				FMDropdown("M4t3r14L", Vars::Chams::FakeAngle::VisibleMaterial, FDropdown_None, 1);
				FColorPicker("ColoUr", Vars::Chams::FakeAngle::VisibleColor, 0, FColorPicker_Dropdown);
			} EndSection();
			if (Section("Seamodel"))
			{
				FToggle("the thingy you hit wit", Vars::Chams::Viewmodel::Weapon);
				FToggle("the thingy you hold wit", Vars::Chams::Viewmodel::Hands, FToggle_Middle);

				FMDropdown("Meriel?", Vars::Chams::Viewmodel::VisibleMaterial, FDropdown_None, 1);
				FColorPicker("ColoUr", Vars::Chams::Viewmodel::VisibleColor, 0, FColorPicker_Dropdown);
			} EndSection();

			EndTable();
		}
		break;
	// Glow
	case 2:
		if (BeginTable("VisualsGlowTable", 2))
		{
			/* Column 1 */
			TableNextColumn();
			if (Section("other guys"))
			{
				FToggle("Paloyeras", Vars::Glow::Friendly::Players);
				FToggle("DEATH", Vars::Glow::Friendly::Ragdolls, FToggle_Middle);
				FToggle("passive architecture", Vars::Glow::Friendly::Buildings);
				FToggle("roket", Vars::Glow::Friendly::Projectiles, FToggle_Middle);
				Dummy({ 0, 8 });

				FToggle("good glow", Vars::Glow::Friendly::Stencil);
				FToggle("bad glow", Vars::Glow::Friendly::Blur, FToggle_Middle);
				bTransparent = !FGet(Vars::Glow::Friendly::Stencil);
					FSlider("good glow scale## Friendly", Vars::Glow::Friendly::StencilScale, 1, 10, 1, "%d", FSlider_Clamp | FSlider_Left);
				bTransparent = !FGet(Vars::Glow::Friendly::Blur);
					FSlider("bad glow scale## Friendly", Vars::Glow::Friendly::BlurScale, 1, 10, 1, "%d", FSlider_Clamp | FSlider_Right);
				bTransparent = false;
			} EndSection();
			if (Section("guys"))
			{
				FToggle("total deathj", Vars::Glow::Enemy::Players);
				FToggle("LIFE", Vars::Glow::Enemy::Ragdolls, FToggle_Middle);
				FToggle("hostile architecute", Vars::Glow::Enemy::Buildings);
				FToggle("tekor", Vars::Glow::Enemy::Projectiles, FToggle_Middle);
				Dummy({ 0, 8 });

				FToggle("good glow", Vars::Glow::Enemy::Stencil);
				FToggle("bad glow", Vars::Glow::Enemy::Blur, FToggle_Middle);
				bTransparent = !FGet(Vars::Glow::Enemy::Stencil);
					FSlider("good glow scale## Enemy", Vars::Glow::Enemy::StencilScale, 1, 10, 1, "%d", FSlider_Clamp | FSlider_Left);
				bTransparent = !FGet(Vars::Glow::Enemy::Blur);
					FSlider("bad glow scale## Enemy", Vars::Glow::Enemy::BlurScale, 1, 10, 1, "%d", FSlider_Clamp | FSlider_Right);
				bTransparent = false;
			} EndSection();
			if (Section("text editor"))
			{
				FToggle("Personal Computer", Vars::Glow::World::NPCs);
				FToggle("bend over :3", Vars::Glow::World::Pickups, FToggle_Middle);
				FToggle("ICBMs", Vars::Glow::World::Bombs);
				FToggle("hallllllllowen", Vars::Glow::World::Halloween, FToggle_Middle);
				Dummy({ 0, 8 });

				FToggle("good glow", Vars::Glow::World::Stencil);
				FToggle("bad glow", Vars::Glow::World::Blur, FToggle_Middle);
				bTransparent = !FGet(Vars::Glow::World::Stencil);
					FSlider("good glow scale## World", Vars::Glow::World::StencilScale, 1, 10, 1, " % d", FSlider_Clamp | FSlider_Left);
				bTransparent = !FGet(Vars::Glow::World::Blur);
					FSlider("bad glow scale## World", Vars::Glow::World::BlurScale, 1, 10, 1, "%d", FSlider_Clamp | FSlider_Right);
				bTransparent = false;
			} EndSection();

			/* Column 2 */
			TableNextColumn();
			if (Section("epik glowie"))
			{
				FToggle("I WONDER WHY", Vars::Glow::Backtrack::Active);
				SameLine(GetWindowSize().x / 2 + 4); SetCursorPosY(GetCursorPosY() - 24);
				FDropdown("make art", Vars::Glow::Backtrack::Draw, { "First", "First + random", "Allah" }, {}, FDropdown_Left);
				Dummy({ 0, 8 });

				FToggle("good glow", Vars::Glow::Backtrack::Stencil);
				FToggle("bad glow", Vars::Glow::Backtrack::Blur, FToggle_Middle);
				bTransparent = !FGet(Vars::Glow::Backtrack::Stencil);
					FSlider("good glow scale## Backtrack", Vars::Glow::Backtrack::StencilScale, 1, 10, 1, "%d", FSlider_Clamp | FSlider_Left);
				bTransparent = !FGet(Vars::Glow::Backtrack::Blur);
					FSlider("bad glow scale## Backtrack", Vars::Glow::Backtrack::BlurScale, 1, 10, 1, "%d", FSlider_Clamp | FSlider_Right);
				bTransparent = false;
			} EndSection();
			if (Section("Fakey fakey"))
			{
				FToggle("-44 w stope", Vars::Glow::FakeAngle::Active);
				Dummy({ 0, 8 });

				FToggle("good glow", Vars::Glow::FakeAngle::Stencil);
				FToggle("bad glow", Vars::Glow::FakeAngle::Blur, FToggle_Middle);
				bTransparent = !FGet(Vars::Glow::FakeAngle::Stencil);
					FSlider("good glow scale## FakeAngle", Vars::Glow::FakeAngle::StencilScale, 1, 10, 1, "%d", FSlider_Clamp | FSlider_Left);
				bTransparent = !FGet(Vars::Glow::FakeAngle::Blur);
					FSlider("bad glow scale## FakeAngle", Vars::Glow::FakeAngle::BlurScale, 1, 10, 1, "%d", FSlider_Clamp | FSlider_Right);
				bTransparent = false;
			} EndSection();
			if (Section("Seamod3l"))
			{
				FToggle("weaposnyu", Vars::Glow::Viewmodel::Weapon);
				FToggle("legs", Vars::Glow::Viewmodel::Hands, FToggle_Middle);
				Dummy({ 0, 8 });

				FToggle("good glow", Vars::Glow::Viewmodel::Stencil);
				FToggle("bad glow", Vars::Glow::Viewmodel::Blur, FToggle_Middle);
				bTransparent = !FGet(Vars::Glow::Viewmodel::Stencil);
					FSlider("good glow scale## Viewmodel", Vars::Glow::Viewmodel::StencilScale, 1, 10, 1, "%d", FSlider_Clamp | FSlider_Left);
				bTransparent = !FGet(Vars::Glow::Viewmodel::Blur);
					FSlider("bad glow scale## Viewmodel", Vars::Glow::Viewmodel::BlurScale, 1, 10, 1, "%d", FSlider_Clamp | FSlider_Right);
				bTransparent = false;
			} EndSection();

			EndTable();
		}
		break;
	// Misc
	case 3:
		if (BeginTable("VisualsMiscTable", 2))
		{
			/* Column 1 */
			TableNextColumn();
			if (Section("annihilation"))
			{
				FToggle("scop", Vars::Visuals::Removals::Scope);
				FToggle("interpoland", Vars::Visuals::Removals::Interpolation, FToggle_Middle);
				FToggle("disgueses", Vars::Visuals::Removals::Disguises);
				FToggle("overals", Vars::Visuals::Removals::ScreenOverlays, FToggle_Middle);
				FToggle("tuant", Vars::Visuals::Removals::Taunts);
				FToggle("fireeeee", Vars::Visuals::Removals::ScreenEffects, FToggle_Middle);
				FToggle("vis recoil", Vars::Visuals::Removals::ViewPunch);
				FToggle("angel is forced", Vars::Visuals::Removals::AngleForcing, FToggle_Middle);
				FToggle("TODM", Vars::Visuals::Removals::MOTD);
				FToggle("nerdy shit1", Vars::Visuals::Removals::ConvarQueries, FToggle_Middle);
				FToggle("nerdy shit2", Vars::Visuals::Removals::PostProcessing);
				FToggle("nerdy shit3", Vars::Visuals::Removals::DSP, FToggle_Middle);
			} EndSection();
			if (Section("UI"))
			{
				FSlider("wide eyes", Vars::Visuals::UI::FieldOfView, 0, 160, 1, "%d");
				FSlider("wide eyes when snajper", Vars::Visuals::UI::ZoomFieldOfView, 0, 160, 1, "%d");
				FToggle("see dem classes", Vars::Visuals::UI::RevealScoreboard);
				FToggle("scorebaord plaeyrslist", Vars::Visuals::UI::ScoreboardPlayerlist, FToggle_Middle);
				FToggle("scoredbeong colaosur", Vars::Visuals::UI::ScoreboardColors);
				FToggle("I am not cheating! (only for steam)", Vars::Visuals::UI::CleanScreenshots, FToggle_Middle);
				FToggle("see where bots aim", Vars::Visuals::UI::SniperSightlines);
				FToggle("pokip timers", Vars::Visuals::UI::PickupTimers, FToggle_Middle);
			} EndSection();
			if (Section("seoameodl"))
			{
				FToggle("skill emulation", Vars::Visuals::Viewmodel::CrosshairAim);
				FToggle("skill emulation", Vars::Visuals::Viewmodel::ViewmodelAim, FToggle_Middle);
				FSlider("Offset K", Vars::Visuals::Viewmodel::OffsetX, -45, 45, 5, "%d", FSlider_Precision);
				FSlider("Offset Y", Vars::Visuals::Viewmodel::OffsetY, -45, 45, 5, "%d", FSlider_Precision);
				FSlider("Offset S", Vars::Visuals::Viewmodel::OffsetZ, -45, 45, 5, "%d", FSlider_Precision);
				FSlider("Barrel", Vars::Visuals::Viewmodel::Roll, -180, 180, 5, "%d", FSlider_Clamp | FSlider_Precision);
				FToggle("Swag", Vars::Visuals::Viewmodel::Sway);
				bTransparent = !FGet(Vars::Visuals::Viewmodel::Sway);
					FSlider("Swag scale", Vars::Visuals::Viewmodel::SwayScale, 0.01f, 5.f, 0.1f, "%.1f", FSlider_Left);
					FSlider("Swag bacltracl", Vars::Visuals::Viewmodel::SwayInterp, 0.01f, 1.f, 0.1f, "%.1f", FSlider_Right);
				bTransparent = false;
			} EndSection();
			if (Section("wowie!!"))
			{
				FSDropdown("where did my boolet go?", Vars::Visuals::Tracers::ParticleTracer, { "not here...", "here!", "here!", "here!", "here!", "here!", "here!", "here!", "here!" }, FSDropdown_Custom | FDropdown_Left);
				FSDropdown("where did my crit go?", Vars::Visuals::Tracers::ParticleTracerCrits, { "not here...", "here!", "here!", "here!", "here!", "here!", "here!", "here!", "here!" }, FSDropdown_Custom | FDropdown_Right);
			} EndSection();
			if (Section("DEATH"))
			{
				FToggle("!=DEATH", Vars::Visuals::Ragdolls::NoRagdolls);
				FToggle("!=gibby gibby", Vars::Visuals::Ragdolls::NoGib, FToggle_Middle);
				FToggle("modding in a style", Vars::Visuals::Ragdolls::Active);
				bTransparent = !FGet(Vars::Visuals::Ragdolls::Active);
					FToggle("other guys only", Vars::Visuals::Ragdolls::EnemyOnly, FToggle_Middle);
					FDropdown("DEATH effects", Vars::Visuals::Ragdolls::Effects, { "THE PAIN", "elektryka prond nie tyka", "from ashes you rise", "we have to cook jesse" }, {}, FDropdown_Multi | FDropdown_Left);
					FDropdown("DEATH models", Vars::Visuals::Ragdolls::Type, { "boring", "$$$", "Icy" }, {}, FDropdown_Right);
					FSlider("ragdol forc", Vars::Visuals::Ragdolls::Force, -10.f, 10.f, 0.5f, "%.1f", FSlider_Precision);
					FSlider("horizontal forc", Vars::Visuals::Ragdolls::ForceHorizontal, -10.f, 10.f, 0.5f, "%.1f", FSlider_Precision);
					FSlider("vertical forc", Vars::Visuals::Ragdolls::ForceVertical, -10.f, 10.f, 0.5f, "%.1f", FSlider_Precision);
				bTransparent = false;
			} EndSection();

			/* Column 2 */
			TableNextColumn();
			if (Section("Boollet"))
			{
				FColorPicker("coloUred booltets", Vars::Colors::BulletTracer);
				FToggle("bolterts yuh?", Vars::Visuals::Bullet::BulletTracer);
			} EndSection();
			if (Section("Realization"))
			{
				FColorPicker("the color of realization", Vars::Colors::PredictionColor, 1); FColorPicker("Projectile line color", Vars::Colors::ProjectileColor);
				FToggle("retarded", Vars::Visuals::Simulation::Enabled);
				FToggle("x from y for every x", Vars::Visuals::Simulation::Timed, FToggle_Middle);
				FToggle("lines of lines", Vars::Visuals::Simulation::Separators);
				bTransparent = !FGet(Vars::Visuals::Simulation::Separators);
					FSlider("lenght of lines of lines", Vars::Visuals::Simulation::SeparatorLength, 2, 16, 1, "%d", FSlider_Left);
					FSlider("lenght between lines of lines", Vars::Visuals::Simulation::SeparatorSpacing, 1, 16, 1, "%d", FSlider_Right);
				bTransparent = false;
				FColorPicker("cliped niggaz color", Vars::Colors::ClippedColor);
				FToggle("trajectory?", Vars::Visuals::Simulation::ProjectileTrajectory);
				FToggle("3rd eye", Vars::Visuals::Simulation::ProjectileCamera, FToggle_Middle);
				FToggle("trajectory on baim", Vars::Visuals::Simulation::TrajectoryOnShot);
				FToggle("#bloat", Vars::Visuals::Simulation::SwingLines, FToggle_Middle);
			} EndSection();
			if (Vars::Debug::Info.Value)
			{
				if (Section("debug"))
				{
					FToggle("overwrite", Vars::Visuals::Trajectory::Overwrite);
					FSlider("off x", Vars::Visuals::Trajectory::OffX, -25.f, 25.f, 0.5f, "%.1f", FSlider_Precision);
					FSlider("off y", Vars::Visuals::Trajectory::OffY, -25.f, 25.f, 0.5f, "%.1f", FSlider_Precision);
					FSlider("off z", Vars::Visuals::Trajectory::OffZ, -25.f, 25.f, 0.5f, "%.1f", FSlider_Precision);
					FToggle("pipes", Vars::Visuals::Trajectory::Pipes);
					FSlider("hull", Vars::Visuals::Trajectory::Hull, 0.f, 10.f, 0.5f, "%.1f", FSlider_Precision);
					FSlider("speed", Vars::Visuals::Trajectory::Speed, 0.f, 5000.f, 50.f, "%.0f", FSlider_Precision);
					FSlider("gravity", Vars::Visuals::Trajectory::Gravity, 0.f, 2.f, 0.1f, "%.1f", FSlider_Precision);
					FToggle("no spin", Vars::Visuals::Trajectory::NoSpin);
					FSlider("lifetime", Vars::Visuals::Trajectory::LifeTime, 0.f, 10.f, 0.1f, "%.1f", FSlider_Precision);
					FSlider("up vel", Vars::Visuals::Trajectory::UpVelocity, 0.f, 1000.f, 50.f, "%.0f", FSlider_Precision);
					FSlider("ang vel x", Vars::Visuals::Trajectory::AngVelocityX, -1000.f, 1000.f, 50.f, "%.0f", FSlider_Precision);
					FSlider("ang vel y", Vars::Visuals::Trajectory::AngVelocityY, -1000.f, 1000.f, 50.f, "%.0f", FSlider_Precision);
					FSlider("ang vel z", Vars::Visuals::Trajectory::AngVelocityZ, -1000.f, 1000.f, 50.f, "%.0f", FSlider_Precision);
					FSlider("drag", Vars::Visuals::Trajectory::Drag, 0.f, 2.f, 0.1f, "%.1f", FSlider_Precision);
					FSlider("drag x", Vars::Visuals::Trajectory::DragBasisX, 0.f, 0.1f, 0.01f, "%.2f", FSlider_Precision);
					FSlider("drag y", Vars::Visuals::Trajectory::DragBasisY, 0.f, 0.1f, 0.01f, "%.2f", FSlider_Precision);
					FSlider("drag z", Vars::Visuals::Trajectory::DragBasisZ, 0.f, 0.1f, 0.01f, "%.2f", FSlider_Precision);
					FSlider("ang drag x", Vars::Visuals::Trajectory::AngDragBasisX, 0.f, 0.1f, 0.01f, "%.2f", FSlider_Precision);
					FSlider("ang drag y", Vars::Visuals::Trajectory::AngDragBasisY, 0.f, 0.1f, 0.01f, "%.2f", FSlider_Precision);
					FSlider("ang drag z", Vars::Visuals::Trajectory::AngDragBasisZ, 0.f, 0.1f, 0.01f, "%.2f", FSlider_Precision);
					FSlider("max vel", Vars::Visuals::Trajectory::MaxVelocity, 0.f, 4000.f, 50.f, "%.0f", FSlider_Precision);
					FSlider("max ang vel", Vars::Visuals::Trajectory::MaxAngularVelocity, 0.f, 7200.f, 50.f, "%.0f", FSlider_Precision);
				} EndSection();
			}
			if (Section("3D boxes, yez"))
			{
				FColorPicker("edging coloUr", Vars::Colors::HitboxEdge, 1); FColorPicker("Face color", Vars::Colors::HitboxFace);
				FToggle("draw some artsy boxes?", Vars::Visuals::Hitbox::ShowHitboxes);
			} EndSection();
			if (Section("spy on yourself"))
			{
				FToggle("spy on yourself", Vars::Visuals::ThirdPerson::Active);
				FToggle("corsair for spying on yourself", Vars::Visuals::ThirdPerson::Crosshair, FToggle_Middle);
				FSlider("how sus you wanna be?", Vars::Visuals::ThirdPerson::Distance, 0.f, 500.f, 5.f, "%.0f", FSlider_Precision);
				FSlider("wher do you wanna be?", Vars::Visuals::ThirdPerson::Right, -500.f, 500.f, 5.f, "%.0f", FSlider_Precision);
				FSlider("how high are you vro <3", Vars::Visuals::ThirdPerson::Up, -500.f, 500.f, 5.f, "%.0f", FSlider_Precision);
			} EndSection();
			if (Section("see what they don't"))
			{
				FToggle("real?", Vars::Visuals::FOVArrows::Active);
				FSlider("Onstage", Vars::Visuals::FOVArrows::Offset, 0, 500, 25, "%d", FSlider_Precision);
				FSlider("raido", Vars::Visuals::FOVArrows::MaxDist, 0.f, 5000.f, 50.f, "%.0f", FSlider_Precision);
			} EndSection();
			if (Section("text editor"))
			{
				FSDropdown("weird texture ngl", Vars::Visuals::World::WorldTexture, { "paper", "plastic", "stone", "glass", "punk", "weight" }, FSDropdown_Custom);
				FDropdown("alter the reality", Vars::Visuals::World::Modulations, { "text", "lorem", "ipsum", "test", "exampol" }, { }, FDropdown_Left | FDropdown_Multi);
				static std::vector skyNames = {
					"Off", "sky_tf2_04", "sky_upward", "sky_dustbowl_01", "sky_goldrush_01", "sky_granary_01", "sky_well_01", "sky_gravel_01", "sky_badlands_01",
					"sky_hydro_01", "sky_night_01", "sky_nightfall_01", "sky_trainyard_01", "sky_stormfront_01", "sky_morningsnow_01","sky_alpinestorm_01",
					"sky_harvest_01", "sky_harvest_night_01", "sky_halloween", "sky_halloween_night_01", "sky_halloween_night2014_01", "sky_island_01", "sky_rainbow_01"
				};
				FSDropdown("change sky", Vars::Visuals::World::SkyboxChanger, skyNames, FSDropdown_Custom | FDropdown_Right);
				bTransparent = !(FGet(Vars::Visuals::World::Modulations) & 1 << 0);
					FColorPicker("word modul", Vars::Colors::WorldModulation, 0, FColorPicker_Left);
				bTransparent = !(FGet(Vars::Visuals::World::Modulations) & 1 << 1);
					FColorPicker("delete god", Vars::Colors::SkyModulation, 0, FColorPicker_Middle | FColorPicker_SameLine);
				bTransparent = !(FGet(Vars::Visuals::World::Modulations) & 1 << 2);
					FColorPicker("change vision", Vars::Colors::PropModulation, 0, FColorPicker_Left);
				bTransparent = !(FGet(Vars::Visuals::World::Modulations) & 1 << 3);
					FColorPicker("change air", Vars::Colors::ParticleModulation, 0, FColorPicker_Middle | FColorPicker_SameLine);
				bTransparent = !(FGet(Vars::Visuals::World::Modulations) & 1 << 4);
					FColorPicker("render distance = 4", Vars::Colors::FogModulation, 0, FColorPicker_Left);
				bTransparent = false;
				FToggle("be blind when close", Vars::Visuals::World::NearPropFade);
				FToggle("NUH UH", Vars::Visuals::World::NoPropFade, FToggle_Middle);
			} EndSection();

			EndTable();
		}
		break;
	// Radar
	case 4:
		if (BeginTable("VisualsRadarTable", 2))
		{
			/* Column 1 */
			TableNextColumn();
			if (Section("Main"))
			{
				FToggle("enaboling?", Vars::Radar::Main::Active);
				FToggle("let them edge", Vars::Radar::Main::AlwaysDraw, FToggle_Middle);
				FDropdown("got dat drip?", Vars::Radar::Main::Style, { "yah", "nah" });
				FSlider("ranged or melee", Vars::Radar::Main::Range, 50, 3000, 50, "%d", FSlider_Precision);
				FSlider("see or not", Vars::Radar::Main::BackAlpha, 0, 255, 1, "%d", FSlider_Clamp);
				FSlider("wanna see dem linez?", Vars::Radar::Main::LineAlpha, 0, 255, 1, "%d", FSlider_Clamp);
			} EndSection();
			if (Section("other geys"))
			{
				FToggle("enabolr", Vars::Radar::Players::Active);
				FToggle("whatz in da bag", Vars::Radar::Players::Background, FToggle_Middle);
				FDropdown("make art", Vars::Radar::Players::Draw, { "you :3", "geys", "your gays", "frens!", "Cloaked" }, {}, FDropdown_Multi | FDropdown_Left);
				FDropdown(".ico", Vars::Radar::Players::IconType, { "Icons", "Portraits", "Avatar" }, {}, FDropdown_Right);
				FSlider(".ico size## Player", Vars::Radar::Players::IconSize, 12, 30, 2);
				FToggle("DEATH bar", Vars::Radar::Players::Health);
				FToggle("you're tall nigga!", Vars::Radar::Players::Height, FToggle_Middle);
			} EndSection();

			/* Column 2 */
			TableNextColumn();
			if (Section("architecture"))
			{
				FToggle("ebansol", Vars::Radar::Buildings::Active);
				FToggle("bgbgbgbg", Vars::Radar::Buildings::Background, FToggle_Middle);
				FDropdown("make art", Vars::Radar::Buildings::Draw, { "Local", "Enemy", "Team", "Friends" }, {}, FDropdown_Multi);
				FSlider(".ico (32x32) size## Building", Vars::Radar::Buildings::IconSize, 12, 30, 2);
				FToggle("DEATH bar", Vars::Radar::Buildings::Health);
			} EndSection();
			if (Section("text editor"))
			{
				FToggle("ebola", Vars::Radar::World::Active);
				FToggle("babagirl", Vars::Radar::World::Background, FToggle_Middle);
				FDropdown("artsy shit", Vars::Radar::World::Draw, { "Health", "Ammo", "Money", "Bombs", "Halloween" }, {}, FDropdown_Multi);
				FSlider(".ico size## World", Vars::Radar::World::IconSize, 12, 30, 2);
			} EndSection();

			EndTable();
		}
		break;
	// Menu
	case 5:
	{
		if (BeginTable("MenuTable", 2))
		{
			/* Column 1 */
			TableNextColumn();
			if (Section("General"))
			{
				FColorPicker("french accent", Vars::Menu::Theme::Accent, 0, FColorPicker_Left);
				FColorPicker("straight up game changer", Vars::Menu::Theme::Foremost, 0, FColorPicker_Middle | FColorPicker_SameLine);
				FColorPicker("hidden nigga", Vars::Menu::Theme::Background, 0, FColorPicker_Left);
				FColorPicker("more coloUrs", Vars::Menu::Theme::Foreground, 0, FColorPicker_Middle | FColorPicker_SameLine);
				FColorPicker("Active", Vars::Menu::Theme::Active, 0, FColorPicker_Left);
				FColorPicker("deadita", Vars::Menu::Theme::Inactive, 0, FColorPicker_Middle | FColorPicker_SameLine);

				FSDropdown("change it, make gods angry", Vars::Menu::CheatName, {}, FSDropdown_AutoUpdate | FDropdown_Left);
				FSDropdown("change it, make gods very angry", Vars::Menu::CheatPrefix, {}, FDropdown_Right);
				FKeybind("keh", Vars::Menu::MenuPrimaryKey.Map["default"], FButton_Left | FKeybind_AllowMenu);
				FKeybind("more keheh", Vars::Menu::MenuSecondaryKey.Map["default"], FButton_Right | FButton_SameLine | FKeybind_AllowMenu);
				if (Vars::Menu::MenuPrimaryKey.Map["default"] == VK_LBUTTON)
					Vars::Menu::MenuPrimaryKey.Map["default"] = VK_INSERT;
				if (Vars::Menu::MenuSecondaryKey.Map["default"] == VK_LBUTTON)
					Vars::Menu::MenuSecondaryKey.Map["default"] = VK_F3;
			} EndSection();
			if (Section("information her"))
			{
				FDropdown("information her", Vars::Menu::Indicators, { "lags", "bucket", "fams", "miliseconds", "conbibions", "cum pred" }, {}, FDropdown_Multi);
				if (FSlider("DPI", Vars::Menu::DPI, 0.8f, 1.8f, 0.2f, "%.1f", FSlider_Precision))
					g_Draw.RemakeFonts(Vars::Menu::DPI.Map["default"]);
			} EndSection();

			/* Column 2 */
			TableNextColumn();

			EndTable();
		}
	}
	}
}

/* Tab: Misc */
void CMenu::MenuMisc()
{
	using namespace ImGui;

	if (BeginTable("MiscTable", 2))
	{
		/* Column 1 */
		TableNextColumn();
		if (Section("Movement"))
		{
			FDropdown("blyat hop", Vars::Misc::Movement::AutoStrafe, { "gamor mod", "closet fag", "closet semi rager mode" });
			bTransparent = FGet(Vars::Misc::Movement::AutoStrafe) != 2;
				FSlider("blyatttt", Vars::Misc::Movement::AutoStrafeTurnScale, 0.f, 1.f, 0.1f, "%.1f", FSlider_Clamp | FSlider_Precision);
			bTransparent = false;
			FToggle("jump jump jump...", Vars::Misc::Movement::Bunnyhop);
			FToggle("DO NOT USE IT, pwease :3", Vars::Misc::Movement::AutoJumpbug, FToggle_Middle); // this is unreliable without setups, do not depend on it!
			FToggle("1 tick and you're high as fuck", Vars::Misc::Movement::AutoRocketJump);
			FToggle("crouch stab", Vars::Misc::Movement::AutoCTap, FToggle_Middle);
			FToggle("HALT", Vars::Misc::Movement::FastStop);
			FToggle("meth mode", Vars::Misc::Movement::FastAccel, FToggle_Middle);
			FToggle("directional meth mode", Vars::Misc::Movement::FastStrafe);
			FToggle("DO NOT rape me", Vars::Misc::Movement::NoPush, FToggle_Middle);
			FToggle("kaczka", Vars::Misc::Movement::CrouchSpeed);
		} EndSection();
		if (Vars::Debug::Info.Value)
		{
			if (Section("debug"))
			{
				FSlider("timing offset", Vars::Misc::Movement::TimingOffset, -1, 1);
				FSlider("apply timing offset above", Vars::Misc::Movement::ApplyAbove, 0, 8);
			} EndSection();
		}
		if (Section("it's not a bug, it's a feature"))
		{
			FToggle("anti anti cheat", Vars::Misc::Exploits::CheatsBypass);
			FToggle("bypass meth purity", Vars::Misc::Exploits::BypassPure, FToggle_Middle);
			FToggle("fuck cl_cmdrate", Vars::Misc::Exploits::PingReducer);
			bTransparent = !FGet(Vars::Misc::Exploits::PingReducer);
				FSlider("ping goes vrrrr", Vars::Misc::Exploits::PingTarget, 0, 200, FSlider_Right | FSlider_Clamp);
			bTransparent = false;
			SetCursorPosY(GetCursorPosY() - 8);
			FToggle("hattt", Vars::Misc::Exploits::EquipRegionUnlock);
		} EndSection();
		if (Vars::Debug::Info.Value)
		{
			if (Section("Convar spoofer"))
			{
				static std::string sName = "", sValue = "";

				FSDropdown("Convar", &sName, {}, FDropdown_Left);
				FSDropdown("Value", &sValue, {}, FDropdown_Right);
				if (FButton("Send"))
				{
					CNetChannel* netChannel = I::EngineClient->GetNetChannelInfo();
					if (netChannel)
					{
						Utils::ConLog("Convar", std::format("Sent {} as {}", sName, sValue).c_str(), VecToColor(Accent));
						NET_SetConVar cmd(sName.c_str(), sValue.c_str());
						netChannel->SendNetMsg(cmd);

						//sName = "";
						//sValue = "";
					}
				}
			} EndSection();
		}
		if (Section("manualization"))
		{
			FToggle("fuck spies", Vars::Misc::Automation::AntiBackstab);
			FToggle("get kicked bozo", Vars::Misc::Automation::AntiAFK, FToggle_Middle);
			FToggle("retry, RETRY", Vars::Misc::Automation::AntiAutobalance);
			FToggle("lazy motherfucker", Vars::Misc::Automation::AcceptItemDrops, FToggle_Middle);
			FToggle("control your life", Vars::Misc::Automation::TauntControl);
			FToggle("da fuq is this shit", Vars::Misc::Automation::KartControl, FToggle_Middle);
			FToggle("im rich vro <3", Vars::Misc::Automation::BackpackExpander);
		} EndSection();
		if (Section("Sound"))
		{
			FDropdown("Start", Vars::Misc::Sound::Block, { "Feet", "Earrape" }, {}, FDropdown_Multi);
			FToggle("Giant women fetish", Vars::Misc::Sound::GiantWeaponSounds);
		} EndSection();
		if (Section("Cheater vs Machine"))
		{
			FToggle("respown", Vars::Misc::MannVsMachine::InstantRespawn);
			FToggle("revaiv", Vars::Misc::MannVsMachine::InstantRevive, FToggle_Middle);
		} EndSection();

		/* Column 2 */
		TableNextColumn();
		if (Section("Board"))
		{
			FToggle("rape your sister", Vars::Misc::Game::NetworkFix);
			FToggle("rape your prediction", Vars::Misc::Game::PredictionErrorJitterFix);
			FToggle("Bonez :)", Vars::Misc::Game::SetupBonesOptimization, FToggle_Middle);
		} EndSection();
		if (Section("Queueueueueuing"))
		{
			FDropdown("Poland?", Vars::Misc::Queueing::ForceRegions,
				{ "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland", "Poland" },
				{ DC_ATL,	 DC_ORD,	DC_LAX,		   DC_EAT,		 DC_SEA,	DC_IAD,		DC_DFW,		  DC_AMS,	   DC_FRA,		DC_LHR,	  DC_MAD,	DC_PAR,	 DC_STO,	  DC_VIE,	DC_WAW,	  DC_EZE,		  DC_LIM, DC_SCL,	  DC_GRU,	   DC_MAA,	  DC_DXB,  DC_CAN,		DC_HKG,		 DC_BOM,   DC_SEO,	DC_SHA,		DC_SGP,		 DC_TSN,	DC_TYO,	 DC_SYD,   DC_JNB },
				FDropdown_Multi
			);
			FDropdown("are you lazy?", Vars::Misc::Queueing::AutoCasualQueue, { "no", "yes", "very" });
			FToggle("giga chad sigma?", Vars::Misc::Queueing::FreezeQueue);
		} EndSection();
		if (Section("is this real chat?"))
		{
			FToggle("real tagging", Vars::Misc::Chat::Tags);
		} EndSection();
		if (Section("Pasted Shit"))
		{
			FDropdown("spam", Vars::Misc::Paste::ChatSpam, { "Off", "SalmonPaste", "BigPackets", "Lmaobox" });
			FSlider("spam delay", Vars::Misc::Paste::SpamInterval, 3.f, 20.f);
			FToggle("followbot yes?", Vars::Misc::Paste::Followbot);
			FToggle("followbot pookies?", Vars::Misc::Paste::Followbotpookies);
			FToggle("Medik botik?", Vars::Misc::Paste::MedigunSwitch, FToggle_Middle);
			FSlider("followbotik distance", Vars::Misc::Paste::Followbotdistance, 50, 400);
			FToggle("Watermark", Vars::Misc::Paste::Watermark);
		} EndSection();

		EndTable();
	}
}

/* Tab: Settings */
void CMenu::MenuLogs()
{
	using namespace ImGui;

	switch (CurrentLogsTab)
	{
	// Logs
	case 0:
		// Eventually put all logs here, regardless of any settings
		break;
	// Settings
	case 1:
		if (BeginTable("ConfigSettingsTable", 2))
		{
			/* Column 1 */
			TableNextColumn();
			if (Section("share info"))
			{
				FDropdown("logoring", Vars::Logging::Logs, { "yes", "no", "no", "toats", "fuck you nigga", "bots" }, {}, FDropdown_Multi);
				FSlider("half life", Vars::Logging::Lifetime, 0.5f, 5.f, 0.5f, "%.1f");
			} EndSection();
			if (Section("stawp"))
			{
				bTransparent = !(FGet(Vars::Logging::Logs) & 1 << 0);
					FDropdown("send schizos to", Vars::Logging::VoteStart::LogTo, { "Toasts", "Chat", "Pookies", "Console" }, { 1 << 0, 1 << 1, 1 << 2, 1 << 3 }, FDropdown_Multi);
				bTransparent = false;
			} EndSection();
			if (Section("tots"))
			{
				bTransparent = !(FGet(Vars::Logging::Logs) & 1 << 1);
					FDropdown("send schizos to", Vars::Logging::VoteCast::LogTo, { "Toasts", "Chat", "Pookies", "Console" }, { 1 << 0, 1 << 1, 1 << 2, 1 << 3 }, FDropdown_Multi);
				bTransparent = false;
			} EndSection();
			if (Section("#bloat"))
			{
				bTransparent = !(FGet(Vars::Logging::Logs) & 1 << 2);
					FDropdown("send schizos to", Vars::Logging::ClassChange::LogTo, { "Toasts", "Chat", "Pookies", "Console" }, { 1 << 0, 1 << 1, 1 << 2, 1 << 3 }, FDropdown_Multi);
				bTransparent = false;
			} EndSection();

			/* Column 2 */
			TableNextColumn();
			if (Section("own niggaz"))
			{
				bTransparent = !(FGet(Vars::Logging::Logs) & 1 << 3);
					FDropdown("send schizos to", Vars::Logging::Damage::LogTo, { "Toasts", "Chat", "Pookies", "Console" }, { 1 << 0, 1 << 1, 1 << 2, 1 << 3 }, FDropdown_Multi);
				bTransparent = false;
			} EndSection();
			if (Section("zupa ASMR"))
			{
				bTransparent = !(FGet(Vars::Logging::Logs) & 1 << 4);
					FDropdown("send schizos to", Vars::Logging::CheatDetection::LogTo, { "Toasts", "Chat", "Pookies", "Console" }, { 1 << 0, 1 << 1, 1 << 2, 1 << 3 }, FDropdown_Multi);
				bTransparent = false;
			} EndSection();
			if (Section("botty shotty"))
			{
				bTransparent = !(FGet(Vars::Logging::Logs) & 1 << 5);
					FDropdown("send schizos to", Vars::Logging::Tags::LogTo, { "Toasts", "Chat", "Pookies", "Console" }, { 1 << 0, 1 << 1, 1 << 2, 1 << 3 }, FDropdown_Multi);
				bTransparent = false;
			} EndSection();

			EndTable();
		}
	}
}

/* Tab: Config */
void CMenu::MenuSettings()
{
	using namespace ImGui;

	switch (CurrentConfigTab)
	{
	// Settings
	case 0:
		if (BeginTable("ConfigSettingsTable", 2))
		{
			/* Column 1 */
			TableNextColumn();
			if (Section("shit cfg section"))
			{
				if (FButton("hell", FButton_Left))
					ShellExecuteA(NULL, NULL, F::ConfigManager.GetConfigPath().c_str(), NULL, NULL, SW_SHOWNORMAL);
				if (FButton("viscual hell", FButton_Right | FButton_SameLine))
					ShellExecuteA(NULL, NULL, F::ConfigManager.GetVisualsPath().c_str(), NULL, NULL, SW_SHOWNORMAL);

				FTabs({ "shit", "more shit", }, &CurrentConfigType, { GetColumnWidth() / 2 + 2, SubTabSize.y }, { 6, GetCursorPos().y }, false);

				switch (CurrentConfigType)
				{
				// General
				case 0:
				{
					static std::string newName;
					FSDropdown("retarded name", &newName, {}, FSDropdown_AutoUpdate | FDropdown_Left);
					if (FButton("destroy", FButton_Fit | FButton_SameLine | FButton_Large) && newName.length() > 0)
					{
						if (!std::filesystem::exists(F::ConfigManager.GetConfigPath() + "\\" + newName))
							F::ConfigManager.SaveConfig(newName);
						newName.clear();
					}

					for (const auto& entry : std::filesystem::directory_iterator(F::ConfigManager.GetConfigPath()))
					{
						if (!entry.is_regular_file() || entry.path().extension() != F::ConfigManager.ConfigExtension)
							continue;

						std::string configName = entry.path().filename().string();
						configName.erase(configName.end() - F::ConfigManager.ConfigExtension.size(), configName.end());

						const auto current = GetCursorPos().y;

						SetCursorPos({ 14, current + 11 });
						TextColored(configName == F::ConfigManager.GetCurrentConfig() ? Active.Value : Inactive.Value, configName.c_str());

						int o = 26;

						SetCursorPos({ GetWindowSize().x - o, current + 9 });
						if (IconButton(ICON_MD_DELETE))
							OpenPopup(std::format("Confirmation## DeleteConfig{}", configName).c_str());
						o += 25;

						SetCursorPos({ GetWindowSize().x - o, current + 9 });
						if (IconButton(ICON_MD_SAVE))
						{
							if (configName != F::ConfigManager.GetCurrentConfig() || F::ConfigManager.GetCurrentVisuals() != "")
								OpenPopup(std::format("Confirmation## SaveConfig{}", configName).c_str());
							else
								F::ConfigManager.SaveConfig(configName);
						}
						o += 25;

						SetCursorPos({ GetWindowSize().x - o, current + 9 });
						if (IconButton(ICON_MD_DOWNLOAD))
							F::ConfigManager.LoadConfig(configName);

						// Dialogs
						{
							// Save config dialog
							if (BeginPopupModal(std::format("Confirmation## SaveConfig{}", configName).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysUseWindowPadding))
							{
								Text(std::format("are you sure vro?", configName).c_str());

								if (FButton("Yah!", FButton_Left))
								{
									F::ConfigManager.SaveConfig(configName);
									CloseCurrentPopup();
								}
								if (FButton("Nah", FButton_Right | FButton_SameLine))
									CloseCurrentPopup();

								EndPopup();
							}

							// Delete config dialog
							if (BeginPopupModal(std::format("Confirmation## DeleteConfig{}", configName).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysUseWindowPadding))
							{
								Text(std::format("are you sure vro?", configName).c_str());

								if (FButton("Yah", FButton_Left))
								{
									F::ConfigManager.RemoveConfig(configName);
									CloseCurrentPopup();
								}
								if (FButton("Nah", FButton_Right | FButton_SameLine))
									CloseCurrentPopup();

								EndPopup();
							}
						}

						SetCursorPos({ 6, current }); DebugDummy({ 0, 28 });
					}
					break;
				}
				// Visuals
				case 1:
				{
					static std::string newName;
					FSDropdown("cgfg nam", &newName, {}, FSDropdown_AutoUpdate | FDropdown_Left);
					if (FButton("destroy", FButton_Fit | FButton_SameLine | FButton_Large) && newName.length() > 0)
					{
						if (!std::filesystem::exists(F::ConfigManager.GetVisualsPath() + "\\" + newName))
							F::ConfigManager.SaveVisual(newName);
						newName.clear();
					}

					for (const auto& entry : std::filesystem::directory_iterator(F::ConfigManager.GetVisualsPath()))
					{
						if (!entry.is_regular_file() || entry.path().extension() != F::ConfigManager.ConfigExtension)
							continue;

						std::string configName = entry.path().filename().string();
						configName.erase(configName.end() - F::ConfigManager.ConfigExtension.size(), configName.end());

						const auto current = GetCursorPos().y;

						SetCursorPos({ 14, current + 11 });
						TextColored(configName == F::ConfigManager.GetCurrentVisuals() ? Active.Value : Inactive.Value, configName.c_str());

						int o = 26;

						SetCursorPos({ GetWindowSize().x - o, current + 9 });
						if (IconButton(ICON_MD_DELETE))
							OpenPopup(std::format("Confirmation## DeleteVisual{}", configName).c_str());
						o += 25;

						SetCursorPos({ GetWindowSize().x - o, current + 9 });
						if (IconButton(ICON_MD_SAVE))
						{
							if (configName != F::ConfigManager.GetCurrentVisuals())
								OpenPopup(std::format("Confirmation## SaveVisual{}", configName).c_str());
							else
								F::ConfigManager.SaveVisual(configName);
						}
						o += 25;

						SetCursorPos({ GetWindowSize().x - o, current + 9 });
						if (IconButton(ICON_MD_DOWNLOAD))
							F::ConfigManager.LoadVisual(configName);

						// Dialogs
						{
							// Save config dialog
							if (BeginPopupModal(std::format("Confirmation## SaveVisual{}", configName).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysUseWindowPadding))
							{
								Text(std::format("are you sure vro?", configName).c_str());

								if (FButton("Yah", FButton_Left))
								{
									F::ConfigManager.SaveVisual(configName);
									CloseCurrentPopup();
								}
								if (FButton("Nah", FButton_Right | FButton_SameLine))
									CloseCurrentPopup();

								EndPopup();
							}

							// Delete config dialog
							if (BeginPopupModal(std::format("Confirmation## DeleteVisual{}", configName).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysUseWindowPadding))
							{
								Text(std::format("are you sure vro?", configName).c_str());

								if (FButton("Yah", FButton_Left))
								{
									F::ConfigManager.RemoveVisual(configName);
									CloseCurrentPopup();
								}
								if (FButton("Nah", FButton_Right | FButton_SameLine))
									CloseCurrentPopup();

								EndPopup();
							}
						}

						SetCursorPos({ 6, current }); DebugDummy({ 0, 28 });
					}
				}
				}
			} EndSection();
			SetCursorPosX(GetCursorPosX() + 8);
			PushStyleColor(ImGuiCol_Text, Inactive.Value);
			Text("Built @ %s, %s", __DATE__, __TIME__);
			PopStyleColor();

			/* Column 2 */
			TableNextColumn();
			if (Section("Debug"))
			{
				FToggle("Debug info", Vars::Debug::Info);
				FToggle("Debug logging", Vars::Debug::Logging, FToggle_Middle);
				FToggle("Allow secure servers", I::AllowSecureServers);
				bool* m_bPendingPingRefresh = reinterpret_cast<bool*>(I::TFGCClientSystem + 828);
				if (m_bPendingPingRefresh)
					FToggle("Pending ping refresh", m_bPendingPingRefresh, FToggle_Middle);
				FToggle("Show server hitboxes", Vars::Debug::ServerHitbox); HelpMarker("localhost servers");
				FToggle("Anti aim lines", Vars::Debug::AntiAimLines, FToggle_Middle);
				static std::string particleName = "ping_circle";
				FSDropdown("Particle name", &particleName, {}, FDropdown_Left);
				const auto& pLocal = g_EntityCache.GetLocal();
				if (FButton("Dispatch", FButton_Right | FButton_SameLine | FButton_Large) && pLocal)
					Particles::DispatchParticleEffect(particleName.c_str(), pLocal->GetAbsOrigin(), { });
			} EndSection();
			if (Section("Extra"))
			{
				if (FButton("cl_fullupdate", FButton_Left))
					I::EngineClient->ClientCmd_Unrestricted("cl_fullupdate");
				if (FButton("retry", FButton_Right | FButton_SameLine))
					I::EngineClient->ClientCmd_Unrestricted("retry");
				if (FButton("Console", FButton_Left))
					I::EngineClient->ClientCmd_Unrestricted("showconsole");
				if (FButton("Fix Chams", FButton_Right | FButton_SameLine))
					F::Materials.ReloadMaterials();
#ifdef DEBUG
				if (FButton("Dump Classes", FButton_Left))
					F::Misc.DumpClassIDS();
#endif
				if (!I::EngineClient->IsConnected())
				{
					if (FButton("Unlock achievements", FButton_Left))
						OpenPopup("Unlock achievements?");
					if (FButton("Lock achievements", FButton_Right | FButton_SameLine))
						OpenPopup("Lock achievements?");

					if (BeginPopupModal("Unlock achievements?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
					{
						Text("Do you really want to unlock all achievements?");

						Separator();
						if (Button("Yes, unlock", ImVec2(150, 0)))
						{
							F::Misc.UnlockAchievements();
							CloseCurrentPopup();
						}
						SameLine();
						if (Button("No", ImVec2(150, 0)))
							CloseCurrentPopup();
						EndPopup();
					}
					if (BeginPopupModal("Lock achievements?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
					{
						Text("Do you really want to lock all achievements?");

						Separator();
						if (Button("Yes, lock", ImVec2(150, 0)))
						{
							F::Misc.LockAchievements();
							CloseCurrentPopup();
						}
						SameLine();
						if (Button("No", ImVec2(150, 0)))
							CloseCurrentPopup();
						EndPopup();
					}
				}
				if (Vars::Debug::Info.Value)
				{
					if (FButton("Reveal bullet lines", FButton_Left))
						F::Visuals.RevealBulletLines();
					if (FButton("Reveal prediction lines", FButton_Right | FButton_SameLine))
						F::Visuals.RevealSimLines();
					if (FButton("Reveal boxes", FButton_Left))
						F::Visuals.RevealBoxes();
					if (FButton("Print hashes", FButton_Right | FButton_SameLine))
						Hash::PrintHash();
				}
			} EndSection();

			EndTable();
		}
		break;
	// Conditions
	case 1:
		if (Section("setnginggs"))
		{
			FToggle("alzheimer hits hard man...", Vars::Menu::ShowBinds);
			FToggle("alzheimer hits hard man...", Vars::Menu::MenuShowsBinds, FToggle_Middle);
		} EndSection();
		if (Section("conds my beloved"))
		{
			static std::string sName = "";
			static Condition_t tCond = {};

			if (BeginTable("ConditionsTable", 2))
			{
				/* Column 1 */
				TableNextColumn(); SetCursorPos({ GetCursorPos().x - 8, GetCursorPos().y - 8 });
				if (BeginChild("ConditionsTableTable1", { GetColumnWidth() + 4, 104 }, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoBackground))
				{
					FSDropdown("Nam", &sName, {}, FSDropdown_AutoUpdate | FDropdown_Left);
					FSDropdown("father", &tCond.Parent, {}, FSDropdown_AutoUpdate | FDropdown_Right);
					FDropdown("suck a nigga dick", &tCond.Type, { "on key", "with class", "being a certain type" }, {}, FDropdown_Left);
					switch (tCond.Type)
					{
					case 0: tCond.Info = std::min(tCond.Info, 2); FDropdown("how?", &tCond.Info, { "hurt", "not as much", "psycho" }, {}, FDropdown_Right); break;
					case 1: tCond.Info = std::min(tCond.Info, 8); FDropdown("glass", &tCond.Info, { "Scunt", "projectile", "PyroFag", "N-...", "sandviscg", "Engibor", "Mewdick", "bot", "french" }, {}, FDropdown_Right); break;
					case 2: tCond.Info = std::min(tCond.Info, 2); FDropdown("Weapon type", &tCond.Info, { "Hitscan", "Projectile", "Melee" }, {}, FDropdown_Right); break;
					}
				} EndChild();

				/* Column 2 */
				TableNextColumn(); SetCursorPos({ GetCursorPos().x - 4, GetCursorPos().y - 8 });
				if (BeginChild("ConditionsTableTable2", { GetColumnWidth() + 8, 104 }, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoBackground))
				{
					SetCursorPos({ 8, 24 });
					FToggle("Visible", &tCond.Visible);
					FToggle("Not", &tCond.Not, FToggle_Middle);
					if (tCond.Type == 0)
					{
						SetCursorPos({ 8, 56 });
						FKeybind("Key", tCond.Key, FButton_Large, -96);
					}

					// create/modify button
					bool bCreate = false, bClear = false, bMatch = false, bParent = true;
					if (tCond.Parent != "")
						bParent = F::Conditions.Exists(tCond.Parent);

					SetCursorPos({ GetWindowSize().x - 96, 64 });
					PushStyleColor(ImGuiCol_Button, Foremost.Value);
					PushStyleColor(ImGuiCol_ButtonActive, Foremost.Value);
					if (sName != "" && sName != "default" && bParent && (!tCond.Type ? tCond.Key : true))
					{
						PushStyleColor(ImGuiCol_ButtonHovered, ForemostLight.Value);
						bCreate = Button("##CreateButton", { 40, 40 });
					}
					else
					{
						PushStyleColor(ImGuiCol_ButtonHovered, Foremost.Value);
						Button("##CreateButton", { 40, 40 });
					}
					PopStyleColor(3);
					SetCursorPos({ GetWindowSize().x - 83, 76 });
					if (sName != "" && sName != "default" && bParent && (!tCond.Type ? tCond.Key : true))
					{
						bMatch = F::Conditions.Exists(sName);
						IconImage(bMatch ? ICON_MD_SETTINGS : ICON_MD_ADD);
					}
					else
					{
						bTransparent = true;
						IconImage(ICON_MD_ADD);
						bTransparent = false;
					}

					// clear button
					SetCursorPos({ GetWindowSize().x - 48, 64 });
					PushStyleColor(ImGuiCol_Button, Foremost.Value);
					PushStyleColor(ImGuiCol_ButtonHovered, ForemostLight.Value);
					PushStyleColor(ImGuiCol_ButtonActive, Foremost.Value);
					bClear = Button("##ClearButton", { 40, 40 });
					PopStyleColor(3);
					SetCursorPos({ GetWindowSize().x - 35, 76 });
					IconImage(ICON_MD_CLEAR);

					if (bCreate)
						F::Conditions.AddCondition(sName, tCond);
					if (bCreate || bClear)
					{
						sName = "";
						tCond = {};
					}
				} EndChild();

				EndTable();
			}

			PushStyleColor(ImGuiCol_Text, Inactive.Value);
			SetCursorPos({ 14, 128 }); FText("Conditions");
			PopStyleColor();

			std::function<int(std::string, int, int)> getConds = [&](std::string parent, int x, int y)
				{
					for (auto& sCond : F::Conditions.vConditions)
					{
						auto& cCond = F::Conditions.mConditions[sCond];
						if (cCond.Parent != parent)
							continue;

						y++;

						std::string info; std::string state;
						switch (cCond.Type)
						{
							// key
						case 0:
							switch (cCond.Info)
							{
							case 0: { info = "hold"; break; }
							case 1: { info = "toggle"; break; }
							case 2: { info = "double"; break; }
							}
							state = VK2STR(cCond.Key);
							break;
							// class
						case 1:
							info = "class";
							switch (cCond.Info)
							{
							case 0: { state = "scout"; break; }
							case 1: { state = "soldier"; break; }
							case 2: { state = "pyro"; break; }
							case 3: { state = "demoman"; break; }
							case 4: { state = "heavy"; break; }
							case 5: { state = "engineer"; break; }
							case 6: { state = "medic"; break; }
							case 7: { state = "sniper"; break; }
							case 8: { state = "spy"; break; }
							}
							break;
							// weapon type
						case 2:
							info = "weapon";
							switch (cCond.Info)
							{
							case 0: { state = "hitscan"; break; }
							case 1: { state = "projectile"; break; }
							case 2: { state = "melee"; break; }
							}
						}
						if (cCond.Not)
							info = std::format("not {}", info);
						std::string str = std::format("{}, {}", info, state);

						bool bClicked = false, bDelete = false, bEdit = false;

						const ImVec2 restorePos = { 8.f + 28 * x, 108.f + 36.f * y };

						// background
						const float width = GetWindowSize().x - 16 - 28 * x; const auto winPos = GetWindowPos();
						GetWindowDrawList()->AddRectFilled({ winPos.x + restorePos.x, winPos.y + restorePos.y }, { winPos.x + restorePos.x + width, winPos.y + restorePos.y + 28 }, Foremost, 3);

						// text
						SetCursorPos({ restorePos.x + 10, restorePos.y + 7 });
						TextUnformatted(sCond.c_str());

						SetCursorPos({ restorePos.x + width / 2 - CalcTextSize(str.c_str()).x / 2, restorePos.y + 7 });
						TextUnformatted(std::format("{}", str).c_str());

						// buttons
						SetCursorPos({ restorePos.x + width - 22, restorePos.y + 5 });
						bDelete = IconButton(ICON_MD_DELETE);

						SetCursorPos({ restorePos.x + width - 47, restorePos.y + 5 });
						bEdit = IconButton(ICON_MD_EDIT);

						SetCursorPos(restorePos);
						bClicked = Button(std::format("##{}", sCond).c_str(), { width, 28 });

						if (bClicked)
						{
							sName = sCond;
							tCond = cCond;
						}
						if (bDelete)
							OpenPopup(std::format("Confirmation## DeleteCond{}", sCond).c_str());
						if (BeginPopupModal(std::format("Confirmation## DeleteCond{}", sCond).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysUseWindowPadding))
						{
							const bool bChildren = F::Conditions.HasChildren(sCond);
							Text(std::format("Do you really want to delete '{}'{}?", sCond, bChildren ? " and all of its children" : "").c_str());

							if (FButton("Yes", FButton_Left))
							{
								F::Conditions.RemoveCondition(sCond);
								CloseCurrentPopup();
							}
							if (FButton("No", FButton_Right | FButton_SameLine))
								CloseCurrentPopup();

							EndPopup();
						}
						if (bEdit)
						{
							if (sCondition != sCond)
								sCondition = sCond;
							else
								sCondition = "default";
						}

						y = getConds(sCond, x + 1, y);
					}

					return y;
				};
			getConds("", 0, 0);
		} EndSection();
		break;
	// PlayerList
	case 2:
		if (Section("rape list"))
		{
			if (I::EngineClient->IsInGame())
			{
				std::lock_guard lock(F::PlayerUtils.mutex);
				const auto& playerCache = F::PlayerUtils.vPlayerCache;

				auto getTeamColor = [](int team, bool alive)
					{
						switch (team)
						{
						case 3: return Color_t(50, 75, 100, alive ? 255 : 127);
						case 2: return Color_t(125, 50, 50, alive ? 255 : 127);
						}
						return Color_t(100, 100, 100, 255);
					};
				auto drawPlayer = [getTeamColor](const ListPlayer& player, int x, int y)
					{
						bool bClicked = false, bAdd = false, bPitch = false, bYaw = false;

						const Color_t teamColor = getTeamColor(player.Team, player.Alive);
						const ImColor imColor = ColorToVec(teamColor);

						const ImVec2 restorePos = { x ? GetWindowSize().x / 2 + 4.f : 8.f, 32.f + 36.f * y };

						// background
						const float width = GetWindowSize().x / 2 - 12; const auto winPos = GetWindowPos();
						GetWindowDrawList()->AddRectFilled({ winPos.x + restorePos.x, winPos.y + restorePos.y }, { winPos.x + restorePos.x + width, winPos.y + restorePos.y + 28 }, imColor, 3);

						// text + icons
						if (player.Local)
						{
							SetCursorPos({ restorePos.x + 7, restorePos.y + 5 });
							IconImage(ICON_MD_PERSON);
						}
						else if (player.Friend)
						{
							SetCursorPos({ restorePos.x + 7, restorePos.y + 5 });
							IconImage(ICON_MD_GROUP);
						}
						int lOffset = player.Local || player.Friend ? 29 : 10;
						SetCursorPos({ restorePos.x + lOffset, restorePos.y + 7});
						TextUnformatted(player.Name);
						lOffset += CalcTextSize(player.Name).x + 8;

						// buttons
						if (!player.Fake)
						{
							// right
							SetCursorPos({ restorePos.x + width - 22, restorePos.y + 5 });
							bAdd = IconButton(ICON_MD_ADD);
							if (Vars::AntiHack::Resolver::Resolver.Value && !player.Local)
							{
								SetCursorPos({ restorePos.x + width - 42, restorePos.y + 5 });
								bYaw = IconButton(ICON_MD_ARROW_FORWARD);

								SetCursorPos({ restorePos.x + width - 62, restorePos.y + 5 });
								bPitch = IconButton(ICON_MD_ARROW_UPWARD);
							}

							// tag bar
							SetCursorPos({ restorePos.x + lOffset, restorePos.y });
							if (BeginChild(std::format("TagBar{}", player.FriendsID).c_str(), { width - lOffset - (Vars::AntiHack::Resolver::Resolver.Value ? 68 : 28), 28}, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground))
							{
								PushFont(F::Menu.FontSmall);

								const auto childPos = GetWindowPos();
								float tOffset = 0;
								for (const auto& sTag : G::PlayerTags[player.FriendsID])
								{
									PriorityLabel_t plTag;
									if (!F::PlayerUtils.GetTag(sTag, &plTag))
										continue;

									const ImColor tagColor = ColorToVec(plTag.Color);
									const float tagWidth = CalcTextSize(sTag.c_str()).x + 25;
									const ImVec2 tagPos = { tOffset, 4 };

									PushStyleColor(ImGuiCol_Text, IsColorBright(tagColor) ? ImVec4{ 0, 0, 0, 1 } : ImVec4{ 1, 1, 1, 1 });

									GetWindowDrawList()->AddRectFilled({ childPos.x + tagPos.x, childPos.y + tagPos.y }, { childPos.x + tagPos.x + tagWidth, childPos.y + tagPos.y + 20 }, tagColor, 3);
									SetCursorPos({ tagPos.x + 5, tagPos.y + 4 });
									TextUnformatted(sTag.c_str());
									SetCursorPos({ tagPos.x + tagWidth - 18, tagPos.y + 2 });
									if (IconButton(ICON_MD_CANCEL))
										F::PlayerUtils.RemoveTag(player.FriendsID, sTag, true, player.Name);

									PopStyleColor();

									tOffset += tagWidth + 4;
								}
								PopFont();
							} EndChild();

							if (!player.Local)
							{
								//bClicked = IsItemClicked();
								bClicked = IsItemHovered() && IsMouseClicked(ImGuiMouseButton_Right) || bClicked;

								SetCursorPos(restorePos);
								/*bClicked = */Button(std::format("##{}", player.Name).c_str(), { width, 28 }) || bClicked;
								bClicked = IsItemHovered() && IsMouseClicked(ImGuiMouseButton_Right) || bClicked;
							}
						}

						SetCursorPos(restorePos);
						DebugDummy({ 0, 28 });

						if (bClicked)
							OpenPopup(std::format("Clicked{}", player.FriendsID).c_str());
						else if (bAdd)
							OpenPopup(std::format("Add{}", player.FriendsID).c_str());
						else if (bPitch)
							OpenPopup(std::format("Pitch{}", player.FriendsID).c_str());
						else if (bYaw)
							OpenPopup(std::format("Yaw{}", player.FriendsID).c_str());
						
						// popups
						if (FBeginPopup(std::format("Clicked{}", player.FriendsID).c_str()))
						{
							if (FSelectable("Profile"))
								g_SteamInterfaces.Friends->ActivateGameOverlayToUser("steamid", CSteamID(0x0110000100000000ULL + player.FriendsID));

							if (FSelectable("Votekick"))
								I::EngineClient->ClientCmd_Unrestricted(std::format("callvote kick {}", player.UserID).c_str());

							FEndPopup();
						}
						else if (FBeginPopup(std::format("Add{}", player.FriendsID).c_str()))
						{
							for (const auto& [sTag, plTag] : F::PlayerUtils.mTags)
							{
								if (!plTag.Assignable || F::PlayerUtils.HasTag(player.FriendsID, sTag))
									continue;

								auto imColor = ColorToVec(plTag.Color);
								PushStyleColor(ImGuiCol_Text, imColor);
								imColor.x /= 3; imColor.y /= 3; imColor.z /= 3;
								if (FSelectable(sTag.c_str(), imColor))
									F::PlayerUtils.AddTag(player.FriendsID, sTag, true, player.Name);
								PopStyleColor();
							}

							FEndPopup();
						}
						else if (FBeginPopup(std::format("Pitch{}", player.FriendsID).c_str()))
						{
							for (size_t i = 0; i < F::PlayerUtils.vListPitch.size(); i++)
							{
								if (FSelectable(F::PlayerUtils.vListPitch[i]))
									F::Resolver.mResolverMode[player.FriendsID].second = i;
							}

							FEndPopup();
						}
						else if (FBeginPopup(std::format("Yaw{}", player.FriendsID).c_str()))
						{
							for (size_t i = 0; i < F::PlayerUtils.vListYaw.size(); i++)
							{
								if (FSelectable(F::PlayerUtils.vListYaw[i]))
									F::Resolver.mResolverMode[player.FriendsID].second = i;
							}

							FEndPopup();
						}
					};

				// display players
				int iBlu = 0, iRed = 0;
				for (const auto& player : playerCache)
				{
					switch (player.Team)
					{
					case 3:
						drawPlayer(player, 0, iBlu);
						iBlu++; break;
					case 2:
						drawPlayer(player, 1, iRed);
						iRed++; break;
					}
				}
				int iOther = 0; const int iMax = std::max(iBlu, iRed);
				for (const auto& player : playerCache)
				{
					if (player.Team != 3 && player.Team != 2)
					{
						drawPlayer(player, iOther % 2, iMax + iOther / 2);
						iOther++;
					}
				}
			}
			else
			{
				SetCursorPos({ 18, 39 });
				Text("you're not in game dummy! :3");
				DebugDummy({ 0, 8 });
			}
		} EndSection();
		if (Section("Tags"))
		{
			static std::string sName = "";
			static PriorityLabel_t tTag = {};

			if (BeginTable("TagTable", 2))
			{
				/* Column 1 */
				TableNextColumn(); SetCursorPos({ GetCursorPos().x - 8, GetCursorPos().y - 8 });
				if (BeginChild("TagTable1", { GetColumnWidth() + 4, 56 }, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoBackground))
				{
					FSDropdown("Name", &sName, {}, FSDropdown_AutoUpdate | FDropdown_Left, 1);
					FColorPicker("Color", &tTag.Color, 0, FColorPicker_Dropdown);

					bDisabled = sName == "Default" || sName == "Ignored";
						int iLabel = bDisabled ? 0 : tTag.Label;
						FDropdown("Type", &iLabel, { "Priority", "Label" }, {}, FDropdown_Right);
						tTag.Label = iLabel;
						if (bDisabled)
							tTag.Label = false;
					bDisabled = false;
				} EndChild();

				/* Column 2 */
				TableNextColumn(); SetCursorPos({ GetCursorPos().x - 4, GetCursorPos().y - 8 });
				if (BeginChild("TagTable2", { GetColumnWidth() + 8, 56 }, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoBackground))
				{
					bTransparent = tTag.Label; // transparent if we want a label, user can still use to sort
						SetCursorPosY(GetCursorPos().y + 12);
						FSlider("Priority", &tTag.Priority, -10, 10, 1, "%d", FSlider_Left);
					bTransparent = false;

					// create/modify button
					bool bCreate = false, bClear = false;

					SetCursorPos({ GetWindowSize().x - 96, 16 });
					PushStyleColor(ImGuiCol_Button, Foremost.Value);
					PushStyleColor(ImGuiCol_ButtonActive, Foremost.Value);
					if (sName != "")
					{
						PushStyleColor(ImGuiCol_ButtonHovered, ForemostLight.Value);
						bCreate = Button("##CreateButton", { 40, 40 });
					}
					else
					{
						PushStyleColor(ImGuiCol_ButtonHovered, Foremost.Value);
						Button("##CreateButton", { 40, 40 });
					}
					PopStyleColor(3);
					SetCursorPos({ GetWindowSize().x - 83, 28 });
					if (sName != "")
					{
						bool bMatch = false;
						for (const auto& [sTag, _] : F::PlayerUtils.mTags)
						{
							if (bMatch)
								break;
							if (sTag == sName)
								bMatch = true;
						}
						IconImage(bMatch ? ICON_MD_SETTINGS : ICON_MD_ADD);
					}
					else
					{
						bTransparent = true;
							IconImage(ICON_MD_ADD);
						bTransparent = false;
					}

					// clear button
					SetCursorPos({ GetWindowSize().x - 48, 16 });
					PushStyleColor(ImGuiCol_Button, Foremost.Value);
					PushStyleColor(ImGuiCol_ButtonHovered, ForemostLight.Value);
					PushStyleColor(ImGuiCol_ButtonActive, Foremost.Value);
					bClear = Button("##ClearButton", { 40, 40 });
					PopStyleColor(3);
					SetCursorPos({ GetWindowSize().x - 35, 28 });
					IconImage(ICON_MD_CLEAR);

					if (bCreate)
					{
						F::PlayerUtils.bSaveTags = true;
						F::PlayerUtils.mTags[sName].Color = tTag.Color;
						F::PlayerUtils.mTags[sName].Priority = tTag.Priority;
						F::PlayerUtils.mTags[sName].Label = tTag.Label;
					}
					if (bCreate || bClear)
					{
						sName = "";
						tTag = {};
					}
				} EndChild();

				EndTable();
			}

			auto drawTag = [](const std::string sTag, const PriorityLabel_t& plTag, int y)
				{
					bool bClicked = false, bDelete = false;

					ImColor imColor = ColorToVec(plTag.Color);
					imColor.Value.x /= 3; imColor.Value.y /= 3; imColor.Value.z /= 3;

					const ImVec2 restorePos = { plTag.Label ? GetWindowSize().x * 2 / 3 + 4.f : 8.f, 96.f + 36.f * y };

					// background
					const float width = GetWindowSize().x * (plTag.Label ? 1.f / 3 : 2.f / 3) - 12; const auto winPos = GetWindowPos();
					GetWindowDrawList()->AddRectFilled({ winPos.x + restorePos.x, winPos.y + restorePos.y }, { winPos.x + restorePos.x + width, winPos.y + restorePos.y + 28 }, imColor, 3);

					// text
					SetCursorPos({ restorePos.x + 10, restorePos.y + 7 });
					TextUnformatted(sTag.c_str());

					if (!plTag.Label)
					{
						SetCursorPos({ restorePos.x + width / 2, restorePos.y + 7 });
						TextUnformatted(std::format("{}", plTag.Priority).c_str());
					}

					// buttons
					if (!plTag.Locked)
					{
						SetCursorPos({ restorePos.x + width - 22, restorePos.y + 5 });
						bDelete = IconButton(ICON_MD_DELETE);
					}

					SetCursorPos(restorePos);
					bClicked = Button(std::format("##{}", sTag).c_str(), { width, 28 });

					if (bClicked)
					{
						sName = sTag;
						tTag.Color = plTag.Color;
						tTag.Priority = plTag.Priority;
						tTag.Label = plTag.Label;
					}
					if (bDelete)
					{
						F::PlayerUtils.mTags.erase(sTag);
						F::PlayerUtils.bSaveTags = true;
						if (sName == sTag)
						{
							sName = "";
							tTag = {};
						}
					}
				};

			PushStyleColor(ImGuiCol_Text, Inactive.Value);
			SetCursorPos({ 14, 80 }); FText("Priorities");
			SetCursorPos({ GetWindowSize().x * 2 / 3 + 10, 80 }); FText("Labels");
			PopStyleColor();

			std::vector<std::pair<std::string, PriorityLabel_t>> vPriorities = {}, vLabels = {};
			for (const auto& [sTag, plTag] : F::PlayerUtils.mTags)
			{
				if (!plTag.Label)
					vPriorities.push_back({ sTag, plTag });
				else
					vLabels.push_back({ sTag, plTag });
			}

			std::sort(vPriorities.begin(), vPriorities.end(), [&](const auto& a, const auto& b) -> bool
				{
					// override for default tag
					if (a.first == "Default")
						return true;
					if (b.first == "Default")
						return false;

					// sort by priority if unequal
					if (a.second.Priority != b.second.Priority)
						return a.second.Priority > b.second.Priority;

					return a.first < b.first;
				});
			std::sort(vLabels.begin(), vLabels.end(), [&](const auto& a, const auto& b) -> bool
				{
					// sort by priority if unequal
					if (a.second.Priority != b.second.Priority)
						return a.second.Priority > b.second.Priority;

					return a.first < b.first;
				});

			// display tags
			int iPriorities = 0, iLabels = 0;
			for (const auto& pair : vPriorities)
			{
				drawTag(pair.first, pair.second, iPriorities);
				iPriorities++;
			}
			for (const auto& pair : vLabels)
			{
				drawTag(pair.first, pair.second, iLabels);
				iLabels++;
			}
			SetCursorPos({ 0, 60.f + 36.f * std::max(iPriorities, iLabels)}); DebugDummy({ 0, 28 });
		} EndSection();
		break;
	// MaterialManager
	case 3:
		if (BeginTable("MaterialsTable", 2))
		{
			/* Column 1 */
			TableNextColumn();
			if (Section("Manager"))
			{
				static std::string newName;
				FSDropdown("Material name", &newName, {}, FSDropdown_AutoUpdate | FDropdown_Left);
				if (FButton("Create", FButton_Fit | FButton_SameLine | FButton_Large) && newName.length() > 0)
				{
					F::Materials.AddMaterial(newName);
					newName.clear();
				}

				if (FButton("Folder", FButton_Fit | FButton_SameLine | FButton_Large))
					ShellExecuteA(nullptr, "open", MaterialFolder.c_str(), nullptr, nullptr, SW_SHOWDEFAULT);

				std::vector<std::pair<std::string, Material_t>> vMaterials;
				for (auto const& [sName, mat] : F::Materials.mChamMaterials)
					vMaterials.push_back({ sName, mat });

				std::sort(vMaterials.begin(), vMaterials.end(), [&](const auto& a, const auto& b) -> bool
					{
						// override for none material
						if (a.first == "None")
							return true;
						if (b.first == "None")
							return false;

						// keep locked materials higher
						if (a.second.bLocked && !b.second.bLocked)
							return true;
						if (!a.second.bLocked && b.second.bLocked)
							return false;

						return a.first < b.first;
					});

				for (auto const& pair : vMaterials)
				{
					const auto current = GetCursorPos().y;

					SetCursorPos({ 14, current + 11 });
					TextColored(pair.second.bLocked ? Inactive.Value : Active.Value, pair.first.c_str());

					int o = 26;

					if (!pair.second.bLocked)
					{
						SetCursorPos({ GetWindowSize().x - o, current + 9 });
						if (IconButton(ICON_MD_DELETE))
							OpenPopup(std::format("Confirmation## DeleteMat{}", pair.first).c_str());
						if (BeginPopupModal(std::format("Confirmation## DeleteMat{}", pair.first).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysUseWindowPadding))
						{
							Text(std::format("Do you really want to delete '{}'?", pair.first).c_str());

							if (FButton("Yes", FButton_Left))
							{
								F::Materials.RemoveMaterial(pair.first);
								CloseCurrentPopup();
							}
							if (FButton("No", FButton_Right | FButton_SameLine))
								CloseCurrentPopup();

							EndPopup();
						}
						o += 25;
					}

					SetCursorPos({ GetWindowSize().x - o, current + 9 });
					if (IconButton(ICON_MD_EDIT))
					{
						CurrentMaterial = pair.first;
						LockedMaterial = pair.second.bLocked;

						TextEditor.SetText(F::Materials.GetVMT(CurrentMaterial));
						TextEditor.SetReadOnly(LockedMaterial);
					}

					SetCursorPos({ 6, current }); DebugDummy({ 0, 28 });
				}
			} EndSection();

			/* Column 2 */
			TableNextColumn();
			if (CurrentMaterial != "")
			{
				auto count = std::ranges::count(TextEditor.GetText(), '\n'); // doesn't account for text editor size otherwise
				if (Section("Editor", 81 + 15 * count, true))
				{
					// Toolbar
					if (!LockedMaterial)
					{
						if (FButton("Save", FButton_Fit))
						{
							auto text = TextEditor.GetText();
							text.erase(text.end() - 1, text.end()); // get rid of random newline
							F::Materials.EditMaterial(CurrentMaterial, text);
						}
						SameLine();
					}
					if (FButton("Close", FButton_Fit))
						CurrentMaterial = "";
					SameLine(); SetCursorPosY(GetCursorPosY() + 27);
					PushStyleColor(ImGuiCol_Text, Inactive.Value);
					FText(LockedMaterial ? std::format("Viewing: {}", CurrentMaterial).c_str() : std::format("Editing: {}", CurrentMaterial).c_str(), FText_Right);
					PopStyleColor();

					// Text editor
					Dummy({ 0, 8 });

					PushFont(FontMono);
					TextEditor.Render("TextEditor");
					PopFont();
				} EndSection();
			}

			EndTable();
		}
		break;
	}
}
#pragma endregion

void CMenu::AddDraggable(const char* szTitle, ConfigVar<DragBox_t>& var, bool bShouldDraw)
{
	using namespace ImGui;

	if (!bShouldDraw)
		return;

	static std::unordered_map<const char*, std::pair<DragBox_t, float>> old = {};
	DragBox_t info = FGet(var);
	const float sizeX = 100.f * Vars::Menu::DPI.Map["default"], sizeY = 40.f * Vars::Menu::DPI.Map["default"];
	SetNextWindowSize({ sizeX, sizeY }, ImGuiCond_Always);
	if (!old.contains(szTitle) || info != old[szTitle].first || sizeX != old[szTitle].second)
		SetNextWindowPos({ float(info.x - sizeX / 2), float(info.y) }, ImGuiCond_Always);

	PushStyleColor(ImGuiCol_WindowBg, {});
	PushStyleColor(ImGuiCol_Border, Active.Value);
	PushStyleVar(ImGuiStyleVar_WindowRounding, 3);
	PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1);
	PushStyleVar(ImGuiStyleVar_WindowMinSize, { sizeX, sizeY });
	if (Begin(szTitle, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing))
	{
		const auto winPos = GetWindowPos();

		info.x = winPos.x + sizeX / 2; info.y = winPos.y; old[szTitle] = { info, sizeX };
		FSet(var, info);

		PushFont(FontBlack);
		auto size = CalcTextSize(szTitle);
		SetCursorPos({ (sizeX - size.x) * 0.5f, (sizeY - size.y) * 0.5f });
		Text(szTitle);
		PopFont();

		End();
	}
	PopStyleVar(3);
	PopStyleColor(2);
}

void CMenu::DrawBinds()
{
	using namespace ImGui;

	if ((IsOpen ? !FGet(Vars::Menu::ShowBinds) : !Vars::Menu::ShowBinds.Value) || !IsOpen && I::EngineVGui->IsGameUIVisible())
		return;

	static DragBox_t old = {};
	DragBox_t info = IsOpen ? FGet(Vars::Menu::BindsDisplay) : Vars::Menu::BindsDisplay.Value;
	if (info != old)
		SetNextWindowPos({ float(info.x), float(info.y) }, ImGuiCond_Always);

	std::vector<bool> actives;
	std::vector<std::string> titles;
	std::vector<std::string> infos;
	std::vector<std::string> states;
	float titleWidth = 0;
	float infoWidth = 0;
	float stateWidth = 0;

	PushFont(FontSmall);
	std::function<void(std::string)> getConds = [&](std::string parent)
		{
			for (auto& sCond : F::Conditions.vConditions)
			{
				auto& tCond = F::Conditions.mConditions[sCond];
				if (tCond.Parent != parent)
					continue;

				if (tCond.Visible)
				{
					std::string info; std::string state;
					switch (tCond.Type)
					{
					// key
					case 0:
						switch (tCond.Info)
						{
						case 0: { info = "hold"; break; }
						case 1: { info = "toggle"; break; }
						case 2: { info = "double"; break; }
						}
						state = VK2STR(tCond.Key);
						break;
					// class
					case 1:
						info = "class";
						switch (tCond.Info)
						{
						case 0: { state = "scout"; break; }
						case 1: { state = "soldier"; break; }
						case 2: { state = "pyro"; break; }
						case 3: { state = "demoman"; break; }
						case 4: { state = "heavy"; break; }
						case 5: { state = "engineer"; break; }
						case 6: { state = "medic"; break; }
						case 7: { state = "sniper"; break; }
						case 8: { state = "spy"; break; }
						}
						break;
					// weapon type
					case 2:
						info = "weapon";
						switch (tCond.Info)
						{
						case 0: { state = "hitscan"; break; }
						case 1: { state = "projectile"; break; }
						case 2: { state = "melee"; break; }
						}
					}
					if (tCond.Not)
						info = std::format("not {}", info);

					actives.push_back(tCond.Active);
					titles.push_back(sCond);
					infos.push_back(info);
					states.push_back(state);
					titleWidth = std::max(titleWidth, CalcTextSize(sCond.c_str()).x);
					infoWidth = std::max(infoWidth, CalcTextSize(info.c_str()).x);
					stateWidth = std::max(stateWidth, CalcTextSize(state.c_str()).x);
				}

				if (tCond.Active)
					getConds(sCond);
			}
		};
	getConds("");

	SetNextWindowSize({ std::max(titleWidth + infoWidth + stateWidth + 42, 56.f), 18.f * actives.size() + 38 });
	PushStyleVar(ImGuiStyleVar_WindowMinSize, { 40.f, 40.f });
	if (Begin("Binds", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
	{
		const auto winPos = GetWindowPos();

		info.x = winPos.x; info.y = winPos.y; old = info;
		if (IsOpen)
			FSet(Vars::Menu::BindsDisplay, info);

		PushFont(FontLarge);
		SetCursorPos({ 11, 9 });
		Text("Binds");
		PopFont();

		const float width = std::max(titleWidth + infoWidth + stateWidth + 42, 56.f);
		GetWindowDrawList()->AddRectFilled({ winPos.x + 8, winPos.y + 26 }, { winPos.x + width - 8, winPos.y + 27 }, Accent, 3);

		for (size_t i = 0; i < actives.size(); i++)
		{
			SetCursorPos({ 12, 18.f * i + 35 });
			PushStyleColor(ImGuiCol_Text, actives[i] ? Accent.Value : Inactive.Value);
			Text(titles[i].c_str());
			PopStyleColor();

			SetCursorPos({ titleWidth + 22, 18.f * i + 35 });
			PushStyleColor(ImGuiCol_Text, actives[i] ? Active.Value : Inactive.Value);
			Text(infos[i].c_str());

			SetCursorPos({ titleWidth + infoWidth + 32, 18.f * i + 35 });
			Text(states[i].c_str());
			PopStyleColor();
		}

		End();
	}
	PopStyleVar();
	PopFont();
}

/* Window for the camera feature */
void CMenu::DrawCameraWindow()
{
	using namespace ImGui;

	if (!FGet(Vars::Visuals::Simulation::ProjectileCamera))
		return;

	static WindowBox_t old = {};
	WindowBox_t info = FGet(Vars::Visuals::Simulation::ProjectileWindow);
	if (info != old)
	{
		SetNextWindowPos({ float(info.x), float(info.y) }, ImGuiCond_Always);
		SetNextWindowSize({ float(info.w), float(info.h) }, ImGuiCond_Always);
	}

	PushStyleColor(ImGuiCol_WindowBg, {});
	PushStyleColor(ImGuiCol_Border, Active.Value);
	PushStyleVar(ImGuiStyleVar_WindowRounding, 3);
	PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1);
	PushStyleVar(ImGuiStyleVar_WindowMinSize, { 100.f, 100.f });
	if (Begin("Camera", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing))
	{
		const auto winPos = GetWindowPos();
		const auto winSize = GetWindowSize();

		info.x = winPos.x; info.y = winPos.y; info.w = winSize.x; info.h = winSize.y; old = info;
		FSet(Vars::Visuals::Simulation::ProjectileWindow, info);

		PushFont(FontBlack);
		auto size = CalcTextSize("Camera");
		SetCursorPos({ (winSize.x - size.x) * 0.5f, (winSize.y - size.y) * 0.5f });
		Text("Camera");
		PopFont();

		End();
	}
	PopStyleVar(3);
	PopStyleColor(2);
}

static void SquareConstraints(ImGuiSizeCallbackData* data) { data->DesiredSize.x = data->DesiredSize.y = std::max(data->DesiredSize.x, data->DesiredSize.y); }
void CMenu::DrawRadar()
{
	using namespace ImGui;

	if (!FGet(Vars::Radar::Main::Active))
		return;

	static WindowBox_t old = {};
	WindowBox_t info = FGet(Vars::Radar::Main::Window);
	if (info != old)
	{
		SetNextWindowPos({ float(info.x), float(info.y) }, ImGuiCond_Always);
		SetNextWindowSize({ float(info.w), float(info.w) }, ImGuiCond_Always);
	}

	PushStyleColor(ImGuiCol_WindowBg, {});
	PushStyleColor(ImGuiCol_Border, Active.Value);
	PushStyleVar(ImGuiStyleVar_WindowRounding, 3);
	PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1);
	SetNextWindowSizeConstraints({ 100.f, 100.f }, { 1000.f, 1000.f }, SquareConstraints);
	if (Begin("Radar", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing))
	{
		const ImVec2 winPos = GetWindowPos();
		const ImVec2 winSize = GetWindowSize();

		info.x = winPos.x; info.y = winPos.y; info.w = winSize.x; old = info;
		FSet(Vars::Radar::Main::Window, info);

		PushFont(FontBlack);
		auto size = CalcTextSize("Radar");
		SetCursorPos({ (winSize.x - size.x) * 0.5f, (winSize.y - size.y) * 0.5f });
		Text("Radar");
		PopFont();

		End();
	}
	PopStyleVar(2);
	PopStyleColor(2);
}

void CMenu::Render(IDirect3DDevice9* pDevice)
{
	using namespace ImGui;

	if (!ConfigLoaded)
		return;

	static std::once_flag initFlag;
	std::call_once(initFlag, [&]
		{
			Init(pDevice);
		});

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

	if (Utils::IsGameWindowInFocus() && (F::KeyHandler.Pressed(Vars::Menu::MenuPrimaryKey.Value)) || F::KeyHandler.Pressed(Vars::Menu::MenuSecondaryKey.Value))
		I::MatSystemSurface->SetCursorAlwaysVisible(IsOpen = !IsOpen);
	G::InKeybind = false;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	NewFrame();

	LoadColors();
	PushFont(FontRegular);
	F::Visuals.DrawTickbaseBars();
	DrawBinds();

	if (IsOpen)
	{
		KeyHandler();

		DrawMenu();

		DrawCameraWindow();
		DrawRadar();

		AddDraggable("Ticks", Vars::Menu::TicksDisplay, FGet(Vars::Menu::Indicators) & (1 << 0));
		AddDraggable("Crit hack", Vars::Menu::CritsDisplay, FGet(Vars::Menu::Indicators) & (1 << 1));
		AddDraggable("Spectators", Vars::Menu::SpectatorsDisplay, FGet(Vars::Menu::Indicators) & (1 << 2));
		AddDraggable("Ping", Vars::Menu::PingDisplay, FGet(Vars::Menu::Indicators) & (1 << 3));
		AddDraggable("Conditions", Vars::Menu::ConditionsDisplay, FGet(Vars::Menu::Indicators) & (1 << 4));
		AddDraggable("Seed prediction", Vars::Menu::SeedPredictionDisplay, FGet(Vars::Menu::Indicators) & (1 << 5));

		Cursor = GetMouseCursor();
	}
	else
		mActives.clear();

	PopFont();

	EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(GetDrawData());
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
}

void CMenu::LoadColors()
{
	using namespace ImGui;

	Accent = ColorToVec(Vars::Menu::Theme::Accent.Value);
	AccentLight = ImColor(Accent.Value.x * 1.1f, Accent.Value.y * 1.1f, Accent.Value.z * 1.1f, Accent.Value.w);
	Background = ColorToVec(Vars::Menu::Theme::Background.Value);
	Foreground = ColorToVec(Vars::Menu::Theme::Foreground.Value);
	Foremost = ColorToVec(Vars::Menu::Theme::Foremost.Value);
	ForemostLight = ImColor(Foremost.Value.x * 1.1f, Foremost.Value.y * 1.1f, Foremost.Value.z * 1.1f, Foremost.Value.w);
	Inactive = ColorToVec(Vars::Menu::Theme::Inactive.Value);
	Active = ColorToVec(Vars::Menu::Theme::Active.Value);

	ImVec4* colors = GetStyle().Colors;
	colors[ImGuiCol_Button] = {};
	colors[ImGuiCol_ButtonHovered] = {};
	colors[ImGuiCol_ButtonActive] = {};
	colors[ImGuiCol_FrameBg] = Foremost;
	colors[ImGuiCol_FrameBgHovered] = ForemostLight;
	colors[ImGuiCol_FrameBgActive] = Foremost;
	colors[ImGuiCol_Header] = {};
	colors[ImGuiCol_HeaderHovered] = ForemostLight;
	colors[ImGuiCol_HeaderActive] = {};
	colors[ImGuiCol_ModalWindowDimBg] = { Background.Value.x, Background.Value.y, Background.Value.z, 0.4f };
	colors[ImGuiCol_PopupBg] = ForemostLight;
	colors[ImGuiCol_ResizeGrip] = {};
	colors[ImGuiCol_ResizeGripActive] = {};
	colors[ImGuiCol_ResizeGripHovered] = {};
	colors[ImGuiCol_ScrollbarBg] = {};
	colors[ImGuiCol_SliderGrab] = Accent;
	colors[ImGuiCol_SliderGrabActive] = AccentLight;
	colors[ImGuiCol_Text] = Active;
	colors[ImGuiCol_WindowBg] = Background;
}

void CMenu::LoadStyle()
{
	using namespace ImGui;

	auto& style = GetStyle();
	style.ButtonTextAlign = { 0.5f, 0.5f }; // Center button text
	style.CellPadding = { 4, 0 };
	style.ChildBorderSize = 0.f;
	style.ChildRounding = 0.f;
	style.FrameBorderSize = 0.f;
	style.FramePadding = { 0, 0 };
	style.FrameRounding = 3.f;
	style.ItemInnerSpacing = { 0, 0 };
	style.ItemSpacing = { 8, 8 };
	style.PopupBorderSize = 0.f;
	style.PopupRounding = 3.f;
	style.ScrollbarSize = 9.f;
	style.ScrollbarRounding = 0.f;
	style.WindowBorderSize = 0.f;
	style.WindowMinSize = { 100, 100 };
	style.WindowPadding = { 0, 0 };
	style.WindowRounding = 3.f;
}

// i think this will occasionally fail and only draw half of the menu w/o any input
void CMenu::Init(IDirect3DDevice9* pDevice)
{
	// Initialize ImGui and device
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(FindWindowA(nullptr, "Team Fortress 2"));
	ImGui_ImplDX9_Init(pDevice);

	// Fonts
	{
		const auto& io = ImGui::GetIO();

		ImFontConfig fontConfig;
		fontConfig.OversampleH = 2;
		constexpr ImWchar fontRange[]{ 0x0020, 0x00FF, 0x0400, 0x044F, 0 }; // Basic Latin, Latin Supplement and Cyrillic

		FontSmall = io.Fonts->AddFontFromMemoryCompressedTTF(RobotoMedium_compressed_data, RobotoMedium_compressed_size, 11.f, &fontConfig, fontRange);
		FontRegular = io.Fonts->AddFontFromMemoryCompressedTTF(RobotoMedium_compressed_data, RobotoMedium_compressed_size, 13.f, &fontConfig, fontRange);
		FontBold = io.Fonts->AddFontFromMemoryCompressedTTF(RobotoBold_compressed_data, RobotoBold_compressed_size, 13.f, &fontConfig, fontRange);
		FontLarge = io.Fonts->AddFontFromMemoryCompressedTTF(RobotoMedium_compressed_data, RobotoMedium_compressed_size, 15.f, &fontConfig, fontRange);
		FontBlack = io.Fonts->AddFontFromMemoryCompressedTTF(RobotoBlack_compressed_data, RobotoBlack_compressed_size, 15.f, &fontConfig, fontRange);
		FontTitle = io.Fonts->AddFontFromMemoryCompressedTTF(RobotoMedium_compressed_data, RobotoMedium_compressed_size, 20.f, &fontConfig, fontRange);
		FontMono = io.Fonts->AddFontFromMemoryCompressedTTF(CascadiaMono_compressed_data, CascadiaMono_compressed_size, 15.f, &fontConfig, fontRange);

		ImFontConfig iconConfig;
		iconConfig.PixelSnapH = true;
		constexpr ImWchar iconRange[]{ ICON_MIN_MD, ICON_MAX_MD, 0 };

		IconFontRegular = io.Fonts->AddFontFromMemoryCompressedTTF(MaterialIcons_compressed_data, MaterialIcons_compressed_size, 15.f, &iconConfig, iconRange);
		IconFontLarge = io.Fonts->AddFontFromMemoryCompressedTTF(MaterialIcons_compressed_data, MaterialIcons_compressed_size, 16.f, &iconConfig, iconRange);

		io.Fonts->Build();
	}

	LoadStyle();
}