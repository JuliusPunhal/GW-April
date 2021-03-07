#pragma once

#include "Dependencies/GWCA.hpp"

#include <unordered_map>
#include <vector>


namespace April {

	class AgentFilter {
	public:
		struct Config {
			bool active;
			std::vector<GW::ItemID> visible_user_items;
			std::vector<GW::Rarity> visible_user_rarities;
			std::vector<GW::ItemID> visible_party_items;
			std::vector<GW::Rarity> visible_party_rarities;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\AgentFilter.ini";
		};


		AgentFilter() = default;

		void DisplaySuppressedItems();

		auto size() const noexcept { return suppressed_packets.size(); }


		void OnSpawn(
			GW::HookStatus*,
			GW::Packet::StoC::AgentAdd const&,
			Config const& );

		void OnDespawn(
			GW::HookStatus*,
			GW::Packet::StoC::AgentRemove const& );


		void UpdateOwner( GW::Packet::StoC::UpdateItemOwner const& );
		void DeleteOwner( GW::Packet::StoC::ItemGeneral_ReuseID const& );
		void Reset();


	private:
		std::vector<GW::Packet::StoC::AgentAdd> suppressed_packets;
		std::unordered_map<GW::ItemID, GW::AgentID> item_owners;
	};

}
