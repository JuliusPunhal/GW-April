#pragma once

#include "GWCA/GWCA.hpp"

#include <memory>
#include <optional>
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

		void Update( GW::Packet::StoC::MapLoaded const& );
		void Update( GW::Packet::StoC::UpdateGuildInfo& );

		auto Unmodified() const -> auto const& { return unmodified; }

		void ApplyNameToPacket( GW::Packet::StoC::UpdateGuildInfo& ) const;
		void ApplyTagToPacket( GW::Packet::StoC::UpdateGuildInfo& ) const;
		void ApplyCapeToPacket( GW::Packet::StoC::UpdateGuildInfo& ) const;


	private:
		std::optional<GW::Packet::StoC::UpdateGuildInfo> unmodified;
		std::shared_ptr<Config const> config;
	};

}
