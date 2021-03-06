#pragma once

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


		UwTimesGui( Config const& );

		void Display( UwTimer const& ) const;


	private:
		Config const& config;
	};

}
