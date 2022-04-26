#pragma once

#include "GWCA/GWCA.hpp"

#include <memory>


namespace April::Module {

	class ChatFilter {
	public:
		struct Config {
			bool active = false;

			bool ally_drop_common   = true;
			bool ally_drop_rare     = true;
			bool ally_pickup_common = true;
			bool ally_pickup_rare   = true;
			bool user_drop_common   = true;
			bool user_drop_rare     = false;
			bool user_pickup_common = true;
			bool user_pickup_rare   = true;

			bool favor                     = true;
			bool guest_privileges          = true;
			bool hall_of_heros             = true;
			bool lunars                    = true;
			bool nine_rings                = true;
			bool no_one_hears_you          = false;
			bool player_has_achieved_title = true;
			bool skill_points              = true;
			bool skill_updated_for_pvp     = true;
			bool target_is_immune          = false;
			bool whisper_target_away       = true;
			bool you_have_been_playing_for = true;
		};


		ChatFilter( std::shared_ptr<Config const> );

		void Update(
			GW::HookStatus&, GW::Packet::StoC::MessageGlobal const& ) const;

		void Update(
			GW::HookStatus&, GW::Packet::StoC::MessageLocal const& ) const;

		void Update(
			GW::HookStatus&, GW::Packet::StoC::MessageServer const& ) const;

		void Update( GW::HookStatus&, GW::LocalMessageInfo ) const;


	private:
		std::shared_ptr<Config const> config;
	};

}
