#pragma once

#include "April/Framework/Command.h"
#include "April/Module/UwTimer.h"
#include "April/Utility/DrawStructures.h"

#include <array>
#include <string>


namespace April::Gui {

	class UwTimesGui {
	public:
		struct Config {
			std::array<std::string, 11> quest_names;
			std::array<std::string, 4>	column_names;

			Window window;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\Gui_UwTimes.ini";
		};


		UwTimesGui() = default;

		auto Display( UwTimer const&, Config const& ) const
			-> Command;
	};

}
