#pragma once

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

		void Display( DhuumsJudgement const&, Config const& ) const;
	};

}
