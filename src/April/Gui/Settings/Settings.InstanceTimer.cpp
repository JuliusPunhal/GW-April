
#include "April/Gui/Settings/Settings.InstanceTimer.h"

#include "April/Utility/SettingsWidgets.h"

#include "ImGui/ImGui.hpp"


April::Gui::Settings_InstanceTimer::Settings_InstanceTimer(
	std::shared_ptr<InstanceTimer::Config> config,
	std::shared_ptr<FontAtlas const>       fonts )
	: config{ config }, fonts{ fonts }
{
}

void April::Gui::Settings_InstanceTimer::Draw()
{
	ImGui::TextWrapped(
		"The Instance Timer Window is a simple display of the current "
		"instance time.\n\n"
		"You can send the time to Team Chat by clicking on it." );

	ImGui::SeparatorSpaced();

	DrawWindowSettings( config->window );

	ImGui::SeparatorSpaced();
	DrawFontSelector( "Font", config->font, *fonts );

	ImGui::ColorEdit4( "Color", &config->label_color.x );
	ImGui::ColorEdit4( "Shadow Color", &config->shadow_color.x );
	ImGui::SliderFloat2(
		"Shadow Offset", &config->shadow_offset.x, -20, 20, "%.f" );
}
