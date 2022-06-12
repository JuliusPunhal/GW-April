
#include "April/Gui/Settings/Settings.Settings.h"

#include "April/Utility/SettingsWidgets.h"

#include "ImGui/ImGui.hpp"


April::Gui::Settings_Settings::Settings_Settings(
	std::shared_ptr<Settings_Config> config,
	std::shared_ptr<FontAtlas const> fonts )
	: config{ config }, fonts{ fonts }
{
}

void April::Gui::Settings_Settings::Draw()
{
	ImGui::TextWrapped(
		"The Settings Window is this very window.\n"
		"Use the [Save] button in the sidebar to save all changes to disk in "
		"order to load them at the next startup. Use the [Discard] button to "
		"overwrite all changes since your last save; this cannot be undone!" );

	ImGui::SeparatorSpaced();

	DrawWindowSettings( config->window );
	DrawFontSelector( "Header", config->heading, *fonts );
}
