
#include "April/Gui/Settings/Settings.DhuumBot.h"

#include "April/Utility/SettingsWidgets.h"

#include "ImGui/ImGui.hpp"


April::Gui::Settings_DhuumBot::Settings_DhuumBot(
	std::shared_ptr<Module::DhuumBot> bot )
	: bot{ bot }
{
}

void April::Gui::Settings_DhuumBot::Draw()
{
	ImGui::TextWrapped(
		"When activated, this module will automatically use Dhuum's Rest and "
		"Ghostly Fury during the fight against Dhuum.\n"
		"It will chose which skill to use depending on Dhuum's Rest; however, "
		"does not adjust your position, use consumables, heal other players, "
		"etc. When the fight is over, it will be deactivated automatically.\n"
		"Dhuum Bot can only be activated inside the Hall of Judgement." );

	ImGui::SeparatorSpaced();

	auto active = bot->is_active();
	DrawActivateButton( active );
	bot->activate( active );
}
