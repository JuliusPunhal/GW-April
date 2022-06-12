
#include "April/Gui/Settings/Settings.ChatFilter.h"

#include "April/Utility/SettingsWidgets.h"

#include "ImGui/ImGui.hpp"


April::Gui::Settings_ChatFilter::Settings_ChatFilter(
	std::shared_ptr<Module::ChatFilter::Config> config )
	: config{ config }
{
}

void April::Gui::Settings_ChatFilter::Draw()
{
	static_assert( sizeof( Module::ChatFilter::Config ) == 21 );

	ImGui::TextWrapped(
		"When activated, this module will suppress the selected chat-"
		"messages." );

	ImGui::SeparatorSpaced();

	DrawActivateButton( config->active );

	ImGui::SeparatorSpaced(); // items for user
	ImGui::Checkbox(
		"Common item was dropped for user", &config->user_drop_common );
	ImGui::Checkbox(
		"Common item was picked up by user", &config->user_pickup_common );
	ImGui::Checkbox(
		"Rare item was dropped for user", &config->user_drop_rare );
	ImGui::Checkbox(
		"Rare item was picked up by user", &config->user_pickup_rare );

	ImGui::SeparatorSpaced(); // items for ally
	ImGui::Checkbox(
		"Common item was dropped for ally", &config->ally_drop_common );
	ImGui::Checkbox(
		"Common item was picked up by ally", &config->ally_pickup_common );
	ImGui::Checkbox(
		"Rare item was dropped for ally", &config->ally_drop_rare );
	ImGui::Checkbox(
		"Rare item was picked up by ally", &config->ally_pickup_rare );

	ImGui::SeparatorSpaced(); // noise
	ImGui::Checkbox(
		"Messages when popping Lunar Fortunes", &config->lunars );
	ImGui::Checkbox(
		"Target is immune to bleeding/disease/poison",
		&config->target_is_immune );
	ImGui::Checkbox(
		"A skill has been updated on your Skill Bar for PvP.",
		&config->skill_updated_for_pvp );
	ImGui::Checkbox(
		"Player might not reply because his or her status is to away.",
		&config->whisper_target_away );
	ImGui::Checkbox(
		"Nine Rings Event messages", &config->nine_rings );
	ImGui::Checkbox(
		"No one hears you.", &config->no_one_hears_you );
	ImGui::Checkbox(
		"Someone has achieved some title.",
		&config->player_has_achieved_title );
	ImGui::Checkbox(
		"Someone's team has won a battle in the Hall of Heroes",
		&config->hall_of_heros );
	ImGui::Checkbox(
		"The gods have blessed the world with their favor", &config->favor );
	ImGui::Checkbox(
		"You gain a skill point!", &config->skill_points );
	ImGui::Checkbox(
		"You have been playing for x hours.",
		&config->you_have_been_playing_for );
	ImGui::Checkbox(
		"You have guest privileges with N guilds.",
		&config->guest_privileges );
}
