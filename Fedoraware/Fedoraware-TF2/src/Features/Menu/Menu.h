#pragma once
#include "../Feature.h"

#include <ImGui/imgui_impl_dx9.h>
#include <ImGui/imgui_color_gradient.h>
#include <ImGui/TextEditor.h>

class CMenu
{
	void DrawMenu();

	void MenuAimbot();
	void MenuVisuals();
	void MenuMisc();
	void MenuLogs();
	void MenuSettings();

	void AddDraggable(const char* szTitle, ConfigVar<DragBox_t>& info, bool bShouldDraw);
	void DrawBinds();
	void DrawCameraWindow();
	void DrawRadar();

	void LoadColors();
	void LoadStyle();

	int CurrentTab = 0;

	int CurrentAimbotTab = 0;
	int CurrentVisualsTab = 0;
	int CurrentLogsTab = 0;
	int CurrentConfigTab = 0;
	int CurrentConfigType = 0;
	int CurrentAboutType = 0;

	int ChamsConfig{ 0 }; // Real, Fake, Enemy, Team, Friend, Target, Backtrack, ViewmodelArms, ViewmodelWeapon, LocalBuildings, EnemyBuildings, TeamBuildings, FriendBuildings, Health, Ammo, Projectiles, Objective [payload, intelligence, etc], NPC [includes boss], Pickups [lunchbox, money, spell, powerup], Ragdoll
	int CurrentChamsFilter = 0;
	int CurrentChamsTab = 0;

	int GlowConfig{ 0 }; // above^
	int CurrentGlowFilter = 0;

	ImVec2 TabSize = { 65, 72 };
	ImVec2 SubTabSize = { 90, 48 };

	// material editor stuff
	TextEditor TextEditor;
	std::string CurrentMaterial;
	bool LockedMaterial;

public:
	void Render(IDirect3DDevice9* pDevice);
	void Init(IDirect3DDevice9* pDevice);

	bool IsOpen = false;
	int Cursor = 2;
	bool ConfigLoaded = false;
	bool Unload = false;

	std::string FeatureHint;

	// Colors
	ImColor Accent = { 255, 101, 101 };
	ImColor AccentLight = { 255, 111, 111 };
	ImColor Background = { 23, 23, 23, 250 };
	ImColor Foreground = { 11, 11, 11, 250 };
	ImColor Foremost = { 23, 23, 23, 250 };
	ImColor ForemostLight = { 25, 25, 25, 250 };
	ImColor Inactive = { 150, 150, 150 };
	ImColor Active = { 255, 255, 255 };

	// Fonts
	ImFont* FontSmall = nullptr;
	ImFont* FontRegular = nullptr;
	ImFont* FontBold = nullptr;
	ImFont* FontLarge = nullptr;
	ImFont* FontBlack = nullptr;
	ImFont* FontTitle = nullptr;
	ImFont* FontMono = nullptr;

	ImFont* IconFontRegular = nullptr;
	ImFont* IconFontLarge = nullptr;
};

ADD_FEATURE(CMenu, Menu)