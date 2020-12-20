#pragma once

#include "April/Utility/DrawStructures.h"

#include "Dependencies/ImGui.hpp"

#include <string>


namespace April::Gui {

	class Healthbar {
	public:
		struct Config {
			struct Colors {
				RGBA standard;
				RGBA degen_hexed;
				RGBA deep_wounded;
				RGBA bleeding;
				RGBA poisoned;
			};
			struct PipsAlignment {
				XY left;
				XY right;
			};

			RGBA				border_color;
			RGBA				background_color;
			Colors				colors;
			PipsAlignment		alignment;
			ImFont*				font;
			std::string			window_name;
			ImGuiWindowFlags	window_flags;

			static auto LoadDefault() -> Config;
		};


		Healthbar( Config const& );

		void Display() const;


	private:
		Config const& config;
	};


 	class Energybar {
 	public:
		struct Config {
			struct Colors {
				RGBA standard;
				// TODO: coloring when overcast
			};
			struct PipsAlignment {
				XY left;
				XY right;
			};

			RGBA				border_color;
			RGBA				background_color;
			Colors				colors;
			PipsAlignment		alignment;
			ImFont*				font;
			std::string			window_name;
			ImGuiWindowFlags	window_flags;

			static auto LoadDefault() -> Config;
		};


 		Energybar( Config const& );

		void Display() const;


	private:
		Config const& config;
	};

}
