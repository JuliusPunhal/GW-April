#pragma once

#include "April/Utility/DrawStructures.h"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"

#include <chrono>


namespace April::Gui::DialogsConfig {

	using namespace GW::Constants;


	inline constexpr auto window_name = "Dialogs";
	inline constexpr auto window_flags = ImGuiWindowFlags_None;

	inline constexpr auto confirm_time = std::chrono::seconds{ 3 };
	inline constexpr auto confirm_color = Red();
	

	struct Dialog {
		const char* description;
		int			dialog;
	};

	inline constexpr Dialog dialogs[]{
		"Take Quest: Slayer",			( QuestID::UW::Mnt << 8 ) | 0x800001,
		"Take Quest: Four Horsemen",	( QuestID::UW::Planes << 8 ) | 0x800001
	};

}
