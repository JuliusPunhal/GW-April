#pragma once

#include "April/Utility/DrawStructures.h"
#include "April/Utility/TimeFormatting.h"

#include "Dependencies/ImGui.hpp"


namespace April::Config::Gui::InstanceTimer {

	inline constexpr auto window_name = "Instance Time";
	inline constexpr auto window_flags = 
		ImGuiWindowFlags_NoTitleBar 
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoScrollWithMouse
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_AlwaysAutoResize
		| ImGuiWindowFlags_NoBackground
		| ImGuiWindowFlags_NoFocusOnAppearing
		| ImGuiWindowFlags_NoBringToFrontOnFocus;

	inline constexpr auto font_path = "C:\\Windows\\Fonts\\Consola.ttf";
	inline constexpr auto font_size = 30;

	inline constexpr auto text_color = White();
	inline constexpr auto shadow_offset = XY{ 1, 1 };
	inline constexpr auto shadow_color = Black();

	inline constexpr auto time_format = HMMSS;

}
