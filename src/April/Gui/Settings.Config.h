#pragma once

#include "April/Utility/FontAtlas.h"

#include "ImGui/ImGui.hpp"

#include <string>


namespace April::Gui {

	struct Settings_Config {
		Font   heading = FontInfo{ "ABeeZee-Regular.ttf", 30 };
		Window window = { "Settings", true, 0 };

		std::string selected_feature = ""; // not saved to disk!
	};

}
