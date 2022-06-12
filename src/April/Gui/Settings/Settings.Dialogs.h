#pragma once

#include "GWCA/GWCA.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>


namespace April::Gui {

	class Settings_Dialogs {
	public:
		struct NamedDialog {
			std::string  description;
			GW::DialogID dialog;
		};
		using NamedDialogs = std::vector<NamedDialog>;

		struct Config {
			NamedDialogs dialogs = {
				{ "Take Quest: Four Horsemen",
					( GW::Constants::QuestID::UW::Planes << 8 ) | 0x800001 },
				{ "Take Quest: Slayer",
					( GW::Constants::QuestID::UW::Mnt << 8 ) | 0x800001 }
			};
		};


		Settings_Dialogs( std::shared_ptr<Config> );

		void Draw();


	private:
		std::shared_ptr<Config> config;

		std::optional<NamedDialog> clicked = {};
		NamedDialog                new_dialog = {};
	};

}
