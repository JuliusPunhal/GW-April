#pragma once

#include "April/Utility/DrawStructures.h"


namespace April::Gui {

	class TargetInfo {
	public:
		struct Config {
			Window window;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\Gui_TargetInfo.ini";
		};


		TargetInfo( Config const& );

		void Display() const;
	

	private:
		Config const& config;
	};

}
