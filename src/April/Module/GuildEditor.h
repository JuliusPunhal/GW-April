#pragma once

#include "GWCA/GWCA.hpp"

#include <memory>
#include <string>


namespace April::Module {

	class GuildEditor {
	public:
		struct CapeStyle {
			int background_color;
			int detail_color;
			int emblem_color;
			int shape;
			int detail;
			int emblem;
			int trim;
		};
		struct Config {
			std::string guild_to_edit;

			bool        active_cape;
			CapeStyle   new_cape;

			bool        active_name;
			std::string new_name;

			bool        active_tag;
			std::string new_tag;
		};


		GuildEditor( std::shared_ptr<Config const> );

		void Update( GW::Packet::StoC::UpdateGuildInfo& );


	private:
		std::shared_ptr<Config const> config;
	};

}
