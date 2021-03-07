#pragma once

#include "April/Utility/DrawStructures.h"


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
			FontInfo			font;
			Window				window;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\Gui_Healthbar.ini";
		};


		Healthbar( Config const& );

		void Display( Config const& ) const;


	private:
		ImFont* font;
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
			FontInfo			font;
			Window				window;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\Gui_Energybar.ini";
		};


 		Energybar( Config const& );

		void Display( Config const& ) const;


	private:
		ImFont* font;
	};

}
