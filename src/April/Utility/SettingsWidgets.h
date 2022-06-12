#pragma once

#include "April/Utility/FontAtlas.h"

#include "ImGui/ImGui.hpp"


namespace April {

	bool DrawActivateButton(
		bool& active,
		const char* when_active = "Deactivate",
		const char* when_inactive = "Activate" );

	void DrawFontSelector(
		const char* label, Font& current, FontAtlas const& );

	void DrawWindowSettings( Window& );

}
