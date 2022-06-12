#pragma once

#include "GWCA/GWCA.hpp"

#include <variant>


namespace April::Module {

	class ShowKitUses {
	public:
		struct Config {
			bool active = true;
		};

		using ItemPacket =
			std::variant<
				GW::Packet::StoC::ItemGeneral_FirstID,
				GW::Packet::StoC::ItemGeneral_ReuseID>;


		ShowKitUses( std::shared_ptr<Config const> );

		void Update( GW::Packet::StoC::InstanceLoadStart const& );
		void Update( GW::Packet::StoC::ItemGeneral_FirstID& );
		void Update( GW::Packet::StoC::ItemGeneral_ReuseID& );

		auto GetUnchanged() const -> auto const& { return unchanged; }


	private:
		std::shared_ptr<Config const> config;

		std::vector<ItemPacket> unchanged{};
	};

}
