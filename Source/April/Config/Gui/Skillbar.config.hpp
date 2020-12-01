#pragma once

#include "April/Gui/Skillbar.h"

#include "April/Utility/DrawStructures.h"

#include "Dependencies/ImGui.hpp"

#include <array>
#include <chrono>


namespace April::Gui::SkillbarConfig {

	inline constexpr auto window_name = "Skillbar";
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

	inline constexpr auto font_path = "C:\\Windows\\Fonts\\Gothic.ttf";
	inline constexpr auto font_size = 30;

	inline constexpr auto spacing = XY{ -1, -1 };
	inline constexpr auto text_color = White();
	inline constexpr auto border_color = White();

	
	struct Threshold {
		std::chrono::milliseconds	time;
		RGBA						color;
	};

	inline constexpr auto thresholds = std::array<Threshold, 4>{
		std::chrono::seconds{ 4 }, Green( 0.25f ),
		std::chrono::seconds{ 3 }, Yellow( 0.25f ),
		std::chrono::seconds{ 1 }, Red( 0.25f ),
		std::chrono::seconds{ 0 }, Red( 0.25f )
	};
	inline constexpr auto color_inactive = Invisible();

}
