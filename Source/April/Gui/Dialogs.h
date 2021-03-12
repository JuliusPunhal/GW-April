#pragma once

#include "April/Framework/Command.h"
#include "April/Utility/DrawStructures.h"

#include <chrono>
#include <string>
#include <vector>


namespace April::Gui {

	class Dialogs {
	public:
		struct Config {
			struct Dialog {
				std::string description;
				int			dialog;
			};

			std::vector<Dialog>			dialogs;
			std::chrono::milliseconds	confirm_time;
			RGBA						confirm_color;

			Window window;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\Gui_Dialogs.ini";
		};


		Dialogs() = default;

		auto Display( Config const& ) -> Command;


	private:
		struct LastClick {
			unsigned 								button;
			std::chrono::steady_clock::time_point	time;
		};
		std::optional<LastClick> awaiting_confirmation = std::nullopt;
	};

}
