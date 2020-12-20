#pragma once

#include "April/Utility/DrawStructures.h"

#include "Dependencies/ImGui.hpp"

#include <chrono>
#include <string>
#include <vector>


namespace April::Gui {

	class Skillbar {
	public:
		struct Config {
			struct Threshold {
				std::chrono::milliseconds	time;
				RGBA						color;
			};

			ImFont*					font;
			RGBA					text_color;
			RGBA					border_color;
			XY						spacing;

			std::vector<Threshold>	thresholds;
			RGBA					color_inactive;

			std::string				window_name;
			ImGuiWindowFlags		window_flags;

			static auto LoadDefault() -> Config;
		};


		Skillbar( Config const& config );

		void Display() const;


	private:
		Config const& config;
	};

}
