#pragma once

#include "April/Gui/InstanceTimer.h"
#include "April/Gui/Inventory.h"
#include "April/Gui/Skillbar.h"
#include "April/Gui/UwTimer.h"
#include "April/Utility/FileIO.BasicTypes.h"

#include "nlohmann/json.hpp"


namespace April::Gui {

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
		InstanceTimer::Config,
		font, label_color, shadow_color, shadow_offset, window )

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
		Inventory::Config,
		items_per_row, collapsed_view, slot_size, bag_spacing,
		no_item, unknown_item, wrong_instance_type, suggest_use, need_not_use,
		can_not_use, special_state,
		font, window )

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
		Skillbar::Config,
		font, skills_per_row, skill_size, color_cooldown, color_reduced_cd,
		thresholds, window )

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
		UwTimer::Config, window, auto_set_visibility )

}
