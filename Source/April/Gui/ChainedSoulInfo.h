#pragma once

#include "April/Module/ChainedSoul.h"
#include "April/Utility/DrawStructures.h"


namespace April::Gui {

	class ChainedSoulGui {
	public:
		struct Config {
			Window window;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\Gui_ChainedSoul.ini";
		};


		ChainedSoulGui( Config const& );

		void Display( ChainedSoul const& ) const;


	private:
		Config const& config;
	};

}
