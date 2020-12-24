#pragma once

#include "April/Module/ChainedSoul.h"
#include "April/Utility/DrawStructures.h"

#include <memory>


namespace April::Gui {

	class ChainedSoulGui {
	public:
		struct Config {
			Window window;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\Gui_ChainedSoul.ini";
		};


		ChainedSoulGui( std::shared_ptr<ChainedSoul const>, Config const& );

		void Display() const;


	private:
		std::shared_ptr<ChainedSoul const> state;
		Config const& config;
	};

}
