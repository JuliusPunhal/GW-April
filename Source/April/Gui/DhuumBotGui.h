#pragma once

#include "April/Module/DhuumBot.h"
#include "April/Utility/DrawStructures.h"

#include <memory>


namespace April::Gui {

	class DhuumBotGui {
	public:
		struct Config {
			Window window;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\Gui_DhuumBot.ini";
		};


		DhuumBotGui( std::shared_ptr<DhuumBot>, Config const& );

		void Display() const;


	private:
		std::shared_ptr<DhuumBot> bot;
		Config const& config;
	};

}
