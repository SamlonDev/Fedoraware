#pragma once

#include <array>
#include <optional>

namespace ICONS
{
	inline constexpr int WIDHT = 64;
	inline constexpr int HEIGHT = 64;
	inline constexpr int UNDEFINED = -1;

	constexpr std::size_t TEXTURE_AMOUNT = 32;

	using TextureArray = std::array<std::string, TEXTURE_AMOUNT>;
	constexpr TextureArray TEXTURE = {
		"hud/leaderboard_class_scout.vtf",
		"hud/leaderboard_class_sniper.vtf",
		"hud/leaderboard_class_soldier.vtf",
		"hud/leaderboard_class_demo.vtf",
		"hud/leaderboard_class_medic.vtf",
		"hud/leaderboard_class_heavy.vtf",
		"hud/leaderboard_class_pyro.vtf",
		"hud/leaderboard_class_spy.vtf",
		"hud/leaderboard_class_engineer.vtf",
		"vgui/class_portraits/scout_alpha.vtf",
		"vgui/class_portraits/sniper_alpha.vtf",
		"vgui/class_portraits/soldier_alpha.vtf",
		"vgui/class_portraits/demoman_alpha.vtf",
		"vgui/class_portraits/medic_alpha.vtf",
		"vgui/class_portraits/heavy_alpha.vtf",
		"vgui/class_portraits/pyro_alpha.vtf",
		"vgui/class_portraits/spy_alpha.vtf",
		"vgui/class_portraits/engineer_alpha.vtf",
		"vgui/class_portraits/scout_blue_alpha.vtf",
		"vgui/class_portraits/sniper_blue_alpha.vtf",
		"vgui/class_portraits/soldier_blue_alpha.vtf",
		"vgui/class_portraits/demoman_blue_alpha.vtf",
		"vgui/class_portraits/medic_blue_alpha.vtf",
		"vgui/class_portraits/heavy_blue_alpha.vtf",
		"vgui/class_portraits/pyro_blue_alpha.vtf",
		"vgui/class_portraits/spy_blue_alpha.vtf",
		"vgui/class_portraits/engineer_blue_alpha.vtf",
		"hud/hud_obj_status_sentry_1.vtf",
		"hud/hud_obj_status_sentry_2.vtf",
		"hud/hud_obj_status_sentry_3.vtf",
		"hud/hud_obj_status_dispenser.vtf",
		"hud/hud_obj_status_tele_entrance.vtf",
		"hud/hud_obj_status_tele_exit.vtf",
		"sprites/healbeam.vtf",
		"hud/hud_obj_status_ammo_64.vtf",
		"hud/mvm_cash.vtf",
		"hud/ico_demolish.vtf",
		"vgui/spellbook_book.vtf",
		"backpack/player/items/crafting/hwn2015_gargoyle_stone.vtf"
	};

	using IDArray = std::array<std::optional<int>, TEXTURE_AMOUNT>;
	IDArray initializeIDs() {
		IDArray ids;
		std::fill(ids.begin(), ids.end(), UNDEFINED);
		return ids;
	}

	IDArray ID = initializeIDs();
}
