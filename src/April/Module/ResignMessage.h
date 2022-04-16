#pragma once

#include "GWCA/GWCA.hpp"

#include <memory>


namespace April::Module {

	class ResignMessage {
	public:
		struct Config {
			bool active = true;
		};


		ResignMessage( std::shared_ptr<Config const> );

		void Update( GW::Packet::StoC::MessageCore& ) const;


	private:
		std::shared_ptr<Config const> config;
	};

}
