#pragma once

#include "GWCA/GWCA.hpp"


namespace April::Module {

	class ShowKitUses {
	public:
		struct Config {
			bool active = true;
		};


		ShowKitUses( std::shared_ptr<Config const> );

		void Update( GW::Packet::StoC::ItemGeneral& ) const;
		void Update( GW::Packet::StoC::ItemGeneral_ReuseID& ) const;

	private:
		std::shared_ptr<Config const> config;
	};

}
