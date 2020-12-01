#pragma once

#include "April/Utility/DrawStructures.h"

#include "Dependencies/ImGui.hpp"


namespace April::Gui::HealthEnergyConfig {

	namespace health {

		inline constexpr auto window_name = "Healthbar";
		inline constexpr auto window_flags = 
			ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoScrollWithMouse
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoMouseInputs
			| ImGuiWindowFlags_NoFocusOnAppearing
			| ImGuiWindowFlags_NoBringToFrontOnFocus
			| ImGuiWindowFlags_NoNavInputs
			| ImGuiWindowFlags_NoNavFocus;

		inline constexpr auto font_path = 
			"C:\\Windows\\Fonts\\Consola.ttf";
		inline constexpr auto font_size = 14;

		inline constexpr auto text_color = White();
		inline constexpr auto background = RGBA{ 0.1f, 0.1f, 0.1f, 1 };
		inline constexpr auto border_color = White();

		struct HealthColors {
			RGBA standard;
			RGBA degen_hexed;
			RGBA deep_wounded;
			RGBA bleeding;
			RGBA poisoned;
		};
		inline constexpr auto colors = HealthColors{
			{ 0.70f, 0.00f, 0.00f, 1 },
			{ 0.55f, 0.00f, 0.55f, 1 },
			{ 0.50f, 0.50f, 0.50f, 1 },
			{ 0.77f, 0.57f, 0.38f, 1 },
			{ 0.50f, 0.50f, 0.00f, 1 }
		};
	
	}

	namespace energy {

		inline constexpr auto window_name = "Energybar";
		inline constexpr auto window_flags = 
			ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoScrollWithMouse
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoMouseInputs
			| ImGuiWindowFlags_NoFocusOnAppearing
			| ImGuiWindowFlags_NoBringToFrontOnFocus
			| ImGuiWindowFlags_NoNavInputs
			| ImGuiWindowFlags_NoNavFocus;

		inline constexpr auto font_path =
			"C:\\Windows\\Fonts\\Consola.ttf";
		inline constexpr auto font_size = 14;

		inline constexpr auto text_color = White();
		inline constexpr auto background = RGBA{ 0.1f, 0.1f, 0.1f, 1 };
		inline constexpr auto border_color = White();
		inline constexpr auto color = RGBA{ 0, 0, 0.7f, 1 };

	}

}
