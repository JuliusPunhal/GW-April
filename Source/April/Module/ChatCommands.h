#pragma once

#include "April/Module/ConsumablesMgr.h"

#include "Dependencies/GWCA.hpp"

#include <string>


namespace April {

	class ChatCommands {
	public:
		struct Config {
			struct Abbreviation {
				std::string abbreviation;
				std::string original;
			};

			std::vector<Abbreviation> abbreviations;

			std::string sendchat;		// send to teamchat
			std::string writechat;		// only for user
			std::string openxunlai;
			std::string activate_pcons;
			std::string deactivate_pcons;
			std::string activate_persistent;
			std::string deactivate_persistent;
			std::string set_deactivating_objective;

			static auto LoadDefault() -> Config;
		};


		ChatCommands( std::shared_ptr<ConsumablesMgr>, Config const& );


	private:
		std::shared_ptr<ConsumablesMgr> consumables;
		Config const& config;
	};

}
