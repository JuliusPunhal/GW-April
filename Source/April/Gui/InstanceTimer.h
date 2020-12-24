#pragma once

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

		void Display() const;


	private:
		Config const&	config;
		ImFont*			font;
	};

}
