#pragma once

#include "April/Utility/DrawStructures.h"

#include <chrono>
#include <vector>


namespace April::Gui {

	class Skillbar {
	public:
		struct Config {
			struct Threshold {
				std::chrono::milliseconds	time;
				RGBA						color;
			};

			FontInfo	font_cooldown;
			FontInfo	font_uptime;
			bool		show_uptime;
			XY			uptime_offset;

			RGBA	text_color;
			RGBA	border_color;
			XY		spacing;
			int		skills_per_row;

			std::vector<Threshold>	thresholds;
			RGBA					color_inactive;

			Window window;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\Gui_Skillbar.ini";
		};


		Skillbar( Config const& config );

		void Display() const;


	private:
		Config const& config;
		ImFont* font_cooldown;
		ImFont* font_uptime;
	};

}
