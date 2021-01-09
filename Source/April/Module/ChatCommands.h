#pragma once

#include "April/Module/AgentFilter.h"
#include "April/Module/ConsumablesMgr.h"
namespace April { struct ModuleConfigurations; }

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
			std::string toggle_gui;
			std::string show_suppressed_items;
			std::string exit;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\ChatCommands.ini";
		};


		ChatCommands(
			std::shared_ptr<AgentFilter>,
			std::shared_ptr<ConsumablesMgr>,
			ModuleConfigurations&,
			Config const& );


	private:
		std::shared_ptr<AgentFilter> agent_filter;
		std::shared_ptr<ConsumablesMgr> consumables;
		ModuleConfigurations& configs;
		Config const& config;
	};

}
