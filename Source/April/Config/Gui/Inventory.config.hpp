#pragma once

#include "April/Gui/Inventory.h"

#include "Dependencies/ImGui.hpp"


namespace April::Gui::InventoryConfig {

	inline constexpr auto window_name = "Inventory";
	inline constexpr auto window_flags = 
		ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoScrollWithMouse
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoBackground
		| ImGuiWindowFlags_NoFocusOnAppearing
		| ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoNavInputs
		| ImGuiWindowFlags_NoNavFocus;

	inline constexpr auto font_path = "C:\\Windows\\Fonts\\Gothic.ttf";
	inline constexpr auto font_size = 30;

	inline constexpr auto slot_size = WH{ 36, 44 };
	inline constexpr auto item_spacing = XY{ 1, 1 };
	inline constexpr auto show_border = true;
	inline constexpr auto button_alpha = 0.f;

	inline constexpr auto no_item = Invisible();
	inline constexpr auto unknown_item = Invisible();
	inline constexpr auto wrong_instance_type = Blue();
	inline constexpr auto need_not_use = Green();
	inline constexpr auto can_not_use = White();
	inline constexpr auto suggest_use = Red();
	inline constexpr auto special_state = Yellow();

	inline constexpr char label_no_item = '\0';
	inline constexpr char label_unknown_item = '\0';
	inline constexpr char label_persistent = 'P';
	inline constexpr char label_until_load = 'L';
	inline constexpr char label_until_objective = 'Q';
	inline constexpr char label_inactive = '\0';

}
