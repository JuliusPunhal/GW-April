#pragma once

#include "ImGui/ImGui.hpp"

#include <memory>


namespace April::Gui {

	class Inventory {
	public:
		struct Config {
			int    items_per_row = 5;
			bool   collapsed_view = true;
			ImVec2 slot_size = { 36, 44 };

			// vertical space between two bags when collapsed_view == false
			int    bag_spacing = 27;

			ImVec4 no_item = White();
			ImVec4 unknown_item = White();
			ImVec4 wrong_instance_type = Blue();
			ImVec4 suggest_use = Red();
			ImVec4 need_not_use = Green();
			ImVec4 can_not_use = White();
			ImVec4 special_state = Yellow();

			Window window = {
				"Inventory",
				false,
				ImGuiWindowFlags_NoTitleBar
				| ImGuiWindowFlags_NoResize
				| ImGuiWindowFlags_NoScrollbar
				| ImGuiWindowFlags_NoScrollWithMouse
				| ImGuiWindowFlags_NoCollapse
				| ImGuiWindowFlags_AlwaysAutoResize
				| ImGuiWindowFlags_NoFocusOnAppearing
				| ImGuiWindowFlags_NoBringToFrontOnFocus
			};
		};


		Inventory( std::shared_ptr<Config> );

		void Display();


	private:
		std::shared_ptr<Config> config;
	};

}
