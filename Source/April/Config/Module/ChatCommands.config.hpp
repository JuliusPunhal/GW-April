#pragma once

#include <string_view>


namespace April::ChatCommandsConfig {

	inline constexpr auto sendchat = "/sendchat";		// send to teamchat
	inline constexpr auto writechat = "/writechat";	// only for user

	inline constexpr auto openxunlai = "/x";
	
	inline constexpr auto activate_pcons= "/s";
	inline constexpr auto deactivate_pcons= "/s_off";

	inline constexpr auto activate_persistent = "/sp";
	inline constexpr auto deactivate_persistent = "/sp_off";
	
	inline constexpr auto set_deactivating_objective = "/q";


	struct Abbreviation {
		char const* abbreviation;
		char const* original;
	};

	inline constexpr Abbreviation abbreviations[]{
		// Consumables
		"cupcake",		"22269",
		"apple",		"28431",
		"corn",			"28432",
		"pie",			"28436",
		"egg",			"22752",
		"warsupply",	"35121",
		"lunars",		"29434", // rooster only
		"lunar",		"lunars",
		"soup",			"17061",
		"kabob",		"17060",
		"salad",		"17062",
		"bu",			"24859",
		"grail",		"24861",
		"armor",		"24860",
		"rrc",			"31153",
		"brc",			"31151",
		"grc",			"31152",
		// Objectives
		"restore",		"147",
		"escort",		"148",
		"uwg",			"149",
		"vale",			"150",
		"wastes",		"151",
		"pits",			"152",
		"plains",		"153",
		"mnts",			"154",
		"pools",		"155",
		"dhuum",		"157",
		// Util
		"base",			"cupcake apple corn pie egg warsupply",
		"cons",			"bu grail armor",
		"miku",			"36451",
		"ident",		"5899",
		"perma",		"/sp miku ident",
		"/t4",			"/s base lunars /q plains"
	};

}
