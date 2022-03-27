#pragma once

#include "April/Utility/Mouse.h"
#include "April/Utility/FontAtlas.h"

#include "ImGui/ImGui.hpp"

#include <memory>


namespace April::Gui {

	class InstanceTimer {
	public:
		struct Config {
			Font   font = FontInfo{ "ABeeZee-Regular.ttf", 40 };
			ImVec4 label_color = White();
			ImVec4 shadow_color = Black();
			ImVec2 shadow_offset = { 2, 2 };
			Window window = {
				"Instance Timer",
				false,
				ImGuiWindowFlags_NoTitleBar
				| ImGuiWindowFlags_NoResize
				| ImGuiWindowFlags_NoMove
				| ImGuiWindowFlags_NoScrollbar
				| ImGuiWindowFlags_NoScrollWithMouse
				| ImGuiWindowFlags_NoCollapse
				| ImGuiWindowFlags_AlwaysAutoResize
				| ImGuiWindowFlags_NoBackground
				| ImGuiWindowFlags_NoFocusOnAppearing
				| ImGuiWindowFlags_NoBringToFrontOnFocus
			};
		};


		InstanceTimer(
			std::shared_ptr<Config>,
			std::shared_ptr<FontAtlas const>,
			std::shared_ptr<Mouse> );

		void Display();


	private:
		std::shared_ptr<Config>          config;
		std::shared_ptr<FontAtlas const> fonts;
		std::shared_ptr<Mouse>           mouse;
	};

}
