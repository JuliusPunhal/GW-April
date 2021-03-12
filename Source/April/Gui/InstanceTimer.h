#pragma once

#include "April/Framework/Command.h"
#include "April/Utility/DrawStructures.h"


namespace April::Gui {

	class InstanceTimer {
	public:
		struct Config {
			FontInfo			font;
			RGBA				text_color;
			RGBA				shadow_color;
			XY					shadow_offset;

			Window window;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\Gui_InstanceTimer.ini";
		};


		InstanceTimer( Config const& );

		auto Display( Config const& ) const -> Command;


	private:
		ImFont* font;
	};

}
