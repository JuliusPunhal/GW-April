#pragma once

#include "GWCA/GWCA.hpp"

#include <memory>


namespace April::Module {

	class ReturnToOutpost {
	public:
		struct Config {
			bool active = true;
		};


		ReturnToOutpost( std::shared_ptr<Config const> );

		void Update( GW::Packet::StoC::PartyDefeated const& ) const;
		void Update( GW::Packet::StoC::PlayerIsPartyLeader const& ) const;


	private:
		std::shared_ptr<Config const> config;
	};

}
