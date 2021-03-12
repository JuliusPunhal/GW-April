#pragma once

#include "April/Framework/Command.h"
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


		ChainedSoulGui() = default;

		auto Display( ChainedSoul::SoulState const&, Config const& ) const
			-> Command;
	};

}
