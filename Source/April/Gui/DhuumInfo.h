#pragma once

#include "April/Framework/Command.h"
#include "April/Module/DhuumsJudgement.h"
#include "April/Utility/DrawStructures.h"


namespace April::Gui {

	class DhuumInfo {
	public:
		struct Config {
			Window window;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\Gui_DhuumInfo.ini";
		};


		DhuumInfo() = default;

		auto Display( DhuumsJudgement const&, Config const& ) const
			-> Command;
	};

}
