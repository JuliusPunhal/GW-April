#pragma once

namespace April {

	// Based off of https://github.com/HasKha/GWToolboxpp.git


	class ChatFilter {
	public:
		struct Config {
			bool self_drop_rare;
			bool self_drop_common;
			bool self_pickup;

			bool ally_drop_common;
			bool ally_drop_rare;
			bool ally_pickup_common;
			bool ally_pickup_rare;

			bool away;
			bool dnd;
			bool noone_hears_you;

			bool player_joined;
			bool player_achieved_title;
			bool you_have_been_playing_for;
			bool favor_of_gods;
			bool screenshot_taken;

			bool gain_exp;
			bool self_gain_skillpoint;
			bool ally_gain_skillpoint;
			bool quest_updated;
			bool quest_reward;

			bool invalid_target;
			bool not_enough_energy;

			bool item_cannot_be_used;
			bool summon_in_instance;
			bool inventory_is_full;
			bool item_already_identified;
			bool salvage_results;

			bool challenge_mission_messages;
			bool faction_gain;
			bool skill_to_pvp;
			bool hall_of_heros;

			bool emotes;
			bool guild_announcement;

			bool chest_reveal;
			bool opening_chest_messages;

			bool lunars;
			bool ninerings;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\ChatFilter.ini";
		};


		ChatFilter() = default;

		bool should_suppress( wchar_t const* msg, Config const& ) const;
	};

}
