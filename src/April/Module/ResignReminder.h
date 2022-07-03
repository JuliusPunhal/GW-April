#pragma once

#include "GWCA/GWCA.hpp"

#include <memory>


namespace April::Module {

	class ResignReminder {
	public:
		struct Config {
			bool active = false;
		};


		ResignReminder( std::shared_ptr<Config const> );

		void Update( GW::Packet::StoC::MessageCore& ) const;


	private:
		std::shared_ptr<Config const> config;
	};

}
