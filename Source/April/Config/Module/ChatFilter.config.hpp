#pragma once


namespace April::ChatFilterConfig {

	// set to true to block the message
	

	inline constexpr bool self_drop_rare = true;
	inline constexpr bool self_drop_common = true;
	inline constexpr bool self_pickup = true;

	inline constexpr bool ally_drop_common = true;
	inline constexpr bool ally_drop_rare = true;
	inline constexpr bool ally_pickup_common = true;
	inline constexpr bool ally_pickup_rare = true;

	inline constexpr bool away = true;
	inline constexpr bool dnd = false;
	inline constexpr bool noone_hears_you = true;
	
	inline constexpr bool player_joined = true;
	inline constexpr bool player_achieved_title = true;
	inline constexpr bool you_have_been_playing_for = true;
	inline constexpr bool favor_of_gods = true;
	inline constexpr bool screenshot_taken = true;

	inline constexpr bool gain_exp = true;
	inline constexpr bool self_gain_skillpoint = true;
	inline constexpr bool ally_gain_skillpoint = true;
	inline constexpr bool quest_updated = false;
	inline constexpr bool quest_reward = false;

	inline constexpr bool invalid_target = false;
	inline constexpr bool not_enough_energy = false;
	
	inline constexpr bool item_cannot_be_used = false;
	inline constexpr bool summon_in_instance = true;
	inline constexpr bool inventory_is_full = false;
	inline constexpr bool item_already_identified = true;
	inline constexpr bool salvage_results = true;

	inline constexpr bool challenge_mission_messages = true;
	inline constexpr bool faction_gain = true;
	inline constexpr bool skill_to_pvp = true;
	inline constexpr bool hall_of_heros = true;
	
	inline constexpr bool emotes = false;
	inline constexpr bool guild_announcement = false;

	inline constexpr bool chest_reveal = true;
	inline constexpr bool opening_chest_messages = true;

	inline constexpr bool lunars = true;
	inline constexpr bool ninerings = true;

}
