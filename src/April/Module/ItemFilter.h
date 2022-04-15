#pragma once

#include "GWCA/GWCA.hpp"

#include <memory>
#include <vector>


namespace April::Module {

	class ItemFilter {
	public:
		struct Config {
			bool active = false;

			std::vector<GW::ItemModelID> visible_user_items = user_items();
			std::vector<GW::ItemModelID> visible_party_items = party_items();
			std::vector<GW::Rarity> visible_user_rarities{
				GW::Rarity::Gold
			};
			std::vector<GW::Rarity> visible_party_rarities{};

		private:
			static auto user_items() -> std::vector<GW::ItemModelID>;
			static auto party_items() -> std::vector<GW::ItemModelID>;
		};


		ItemFilter( std::shared_ptr<Config const> );

		void SpawnSuppressedItems();
		auto size() const { return suppressed_packets.size(); }

		void Update( GW::HookStatus&, GW::Packet::StoC::AgentAdd const& );
		void Update( GW::HookStatus&, GW::Packet::StoC::AgentRemove const& );
		void Update( GW::Packet::StoC::MapLoaded const& );


	private:
		std::shared_ptr<Config const> config;
		std::vector<GW::Packet::StoC::AgentAdd> suppressed_packets;
	};

}
