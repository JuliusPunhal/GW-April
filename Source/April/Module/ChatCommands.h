#pragma once

#include "April/Module/AgentFilter.h"
#include "April/Module/ConsumablesMgr.h"
#include "April/Utility/DrawStructures.h"

#include "Dependencies/GWCA.hpp"

#include <string>
#include <vector>


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


		ChatCommands() = default;

		void OnMessage(
			GW::HookStatus*,
			GW::Chat::Channel,
			wchar_t const*,
			AgentFilter&,
			ConsumablesMgr&,
			std::vector<Window*>&,
			bool& terminate,
			Config const& ) const;


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
	};

}
