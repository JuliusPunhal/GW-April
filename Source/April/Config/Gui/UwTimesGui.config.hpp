#pragma once

#include "April/Utility/TimeFormatting.h"

#include "Dependencies/ImGui.hpp"

#include <array>


namespace April::Gui::UwTimesGuiConfig {

	inline constexpr auto window_name = "Underworld Times";
	inline constexpr auto window_flags = ImGuiWindowFlags_None;

	inline constexpr auto quest_names = std::array{
		"Chamber", "Restore", "Escort", "UWG", "Vale",
		"Wastes", "Pits", "Plains", "Mnts", "Pools",
		"Dhuum"
	};
	inline constexpr auto column_names = std::array{ 
		"Pop", "Take", "Done", "Time" 
	};


	inline constexpr auto instance_time_fmt = HMMSS;
	inline constexpr auto default_instance_time = "--:--:--";

	inline constexpr auto quest_time_fmt = MMSS;
	inline constexpr auto default_quest_time = "--:--";

}
