#pragma once

#include "April/Utility/DrawStructures.h"

#include "Dependencies/GWCA.hpp"

#include <array>
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
			RGBA	hsr_color;
			RGBA	border_color;
			XY		spacing;
			int		skills_per_row;

			std::vector<Threshold>	thresholds;
			RGBA					color_inactive;

			Window window;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\Gui_Skillbar.ini";
		};


		Skillbar( Config const& );

		void Display( Config const& ) const;

		void UpdateHSR( GW::Packet::StoC::SkillRecharge const& );
		void UpdateHSR( GW::Packet::StoC::SkillRecharged const& );
		void UpdateHSR( GW::Packet::StoC::MapLoaded const& );


	private:
		ImFont* font_cooldown;
		ImFont* font_uptime;

		std::array<bool, 8> hsr{};
	};

}
