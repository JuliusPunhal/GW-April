#pragma once

#include "April/Gui/InstanceTimer.h"
#include "April/Gui/Inventory.h"
#include "April/Gui/Skillbar.h"
#include "April/Gui/UwTimer.h"
#include "April/Module/ChatCommands.h"
#include "April/Module/GuildEditor.h"
#include "April/Module/ItemFilter.h"
#include "April/Module/NotifyOnEffect.h"
#include "April/Module/ResignMessage.h"
#include "April/Utility/FileIO.BasicTypes.h"

#include "nlohmann/json.hpp"


namespace April::Gui {

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
		Energybar::Config,
		size, border_color, background_color, color, color_overcast, alignment,
		font, window )

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
		Healthbar::Config,
		size, border_color, background_color, colors, alignment, font, window )

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

namespace April::Module {

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( ChatCommands::Config, abbreviations )

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
		GuildEditor::Config,
		guild_to_edit,
		active_cape, new_cape,
		active_name, new_name,
		active_tag, new_tag )

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
		ItemFilter::Config,
		active,
		visible_user_items, visible_party_items, visible_user_rarities,
		visible_party_rarities )

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
		NotifyOnEffect::Config, active, on_gain, on_loss )

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( ResignMessage::Config, active )

}
