#pragma once

namespace April {
	struct Instance; // avoid cyclic include
}

#include "April/Utility/DrawStructures.h"


namespace April::Gui {

	class Settings {
	public:
		struct Config {
			Window window;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\Gui_Settings.ini";
		};


		Settings() = default;

		void Display( Instance& ) const;
	};

}
