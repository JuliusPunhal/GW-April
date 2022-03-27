#pragma once

#include "April/Utility/FontAtlas.h"
#include "April/Utility/ReducedSkillRecharge.h"
#include "April/Utility/sorted_vector.h"

#include "GWCA/GWCA.hpp"

#include "ImGui/ImGui.hpp"

#include <memory>


namespace April::Gui {

	class Skillbar {
	public:
		struct Config {
			struct Threshold {
				GW::ms32 uptime;
				ImVec4   color;

				bool operator>( Threshold const& ) const;
			};
			using Thresholds =
				sorted_vector<Threshold, std::greater<Threshold>>;

			Font       font = FontInfo{ "ABeeZee-Regular.ttf", 30 };
			int        skills_per_row = 8;
			ImVec2     skill_size = { 51, 51 };
			ImVec4     color_cooldown = White();
			ImVec4     color_reduced_cd = Green();
			Thresholds thresholds = {
				{ GW::sec32{ 4 }, Green( .25f ) },
				{ GW::sec32{ 3 }, Yellow( .25f ) },
				{ GW::sec32{ 1 }, Red( .25f ) },
				{ GW::sec32{ 0 }, Red( .25f ) }
			};
			Window window = {
				"Skillbar",
				false,
				ImGuiWindowFlags_NoDecoration
				| ImGuiWindowFlags_NoScrollWithMouse
				| ImGuiWindowFlags_AlwaysAutoResize
				| ImGuiWindowFlags_NoMove
				| ImGuiWindowFlags_NoBackground
				| ImGuiWindowFlags_NoFocusOnAppearing
				| ImGuiWindowFlags_NoBringToFrontOnFocus
			};
		};


		Skillbar(
			std::shared_ptr<Config>,
			std::shared_ptr<FontAtlas const>,
			std::shared_ptr<ReducedSkillRecharge const> );

		void Display();


	private:
		std::shared_ptr<Config>                     config;
		std::shared_ptr<FontAtlas const>            fonts;
		std::shared_ptr<ReducedSkillRecharge const> recharges;
	};

}
