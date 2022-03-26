#pragma once

#include "ImGui/ImGui.hpp"

#include <string_view>


namespace April {

	// If provided, check full path, else check (1) active directory,
	// (2) AppData/Roaming/GW-April/, and (3) Windows/Fonts/
	auto LoadFont( std::string_view file_or_path, int size ) -> ImFont*;

}
