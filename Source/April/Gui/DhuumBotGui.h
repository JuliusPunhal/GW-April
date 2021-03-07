#pragma once

#include "April/Module/DhuumBot.h"
#include "April/Utility/DrawStructures.h"


namespace April::Gui {

	class DhuumBotGui {
	public:
		struct Config {
			Window window;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\Gui_DhuumBot.ini";
		};


		DhuumBotGui() = default;

		void Display( DhuumBot&, Config const& ) const;
	};

}
