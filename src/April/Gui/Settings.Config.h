#pragma once

#include "April/Utility/FontAtlas.h"

#include "ImGui/ImGui.hpp"


namespace April::Gui {

	struct Settings_Config {
		Font   heading = FontInfo{ "ABeeZee-Regular.ttf", 30 };
		Window window = { "Settings", true, 0 };
	};

}
