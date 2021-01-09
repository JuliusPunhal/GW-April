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

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\ChatCommands.ini";
		};


		ChatCommands(
			std::shared_ptr<AgentFilter>,
			std::shared_ptr<ConsumablesMgr>,
			ModuleConfigurations&,
			Config const& );


		static constexpr auto cmd_sendchat =		"/sendchat";
		static constexpr auto cmd_writechat =		"/writechat";
		static constexpr auto cmd_openxunlai =		"/xunlai";
		static constexpr auto cmd_pcons_on =		"/pcons_on";
		static constexpr auto cmd_pcons_off =		"/pcons_off";
		static constexpr auto cmd_persistent_on =	"/persistent_on";
		static constexpr auto cmd_persistent_off =	"/persistent_off";
		static constexpr auto cmd_pcons_objective = "/objective";
		static constexpr auto cmd_toggle_gui =		"/gui";
		static constexpr auto cmd_show_suppressed = "/show_suppressed";
		static constexpr auto cmd_exit =			"/exit";


	private:
		std::shared_ptr<AgentFilter> agent_filter;
		std::shared_ptr<ConsumablesMgr> consumables;
		ModuleConfigurations& configs;
		Config const& config;
	};

}
