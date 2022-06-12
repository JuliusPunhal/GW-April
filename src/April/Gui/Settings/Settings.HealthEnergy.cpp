
#include "April/Gui/Settings/Settings.HealthEnergy.h"

#include "April/Utility/SettingsWidgets.h"


April::Gui::Settings_Healthbar::Settings_Healthbar(
	std::shared_ptr<Healthbar::Config> config,
	std::shared_ptr<FontAtlas const>   fonts )
	: config{ config }, fonts{ fonts }
{
}

void April::Gui::Settings_Healthbar::Draw()
{
	ImGui::TextWrapped(
		"The Healthbar Overlay is inspired by the in-game Healthbar; will, "
		"however, additionally display the maximum health points and the "
		"number of regeneration and degeneration pips.\n"
		"It is supposed to be used as a Overlay covering the in-game "
		"Healthbar." );

	ImGui::SeparatorSpaced();

	DrawWindowSettings( config->window );
	DrawFontSelector( "Font", config->font, *fonts );

	ImGui::SeparatorSpaced();

	ImGui::DragFloat2( "Healthbar Size", &config->size.x, .01f, 0, 0, "%.f" );

	ImGui::SeparatorSpaced();

	ImGui::ColorEdit4( "Health Color", &config->colors.standard.x );
	ImGui::ColorEdit4( "Background", &config->background_color.x );
	ImGui::ColorEdit4( "Degen Hexed", &config->colors.degen_hexed.x );
	ImGui::ColorEdit4( "Deep Wounded", &config->colors.deep_wounded.x );
	ImGui::ColorEdit4( "Bleeding", &config->colors.bleeding.x );
	ImGui::ColorEdit4( "Poisoned", &config->colors.poisoned.x );
	ImGui::ColorEdit4( "Border Color", &config->border_color.x );

	ImGui::SeparatorSpaced();

	ImGui::SliderFloat2(
		"Degen-Pips Alignment", &config->alignment.left.x, 0, 1, "%.2f" );

	ImGui::SliderFloat2(
		"Regen-Pips Alignment", &config->alignment.right.x, 0, 1, "%.2f" );
}

April::Gui::Settings_Energybar::Settings_Energybar(
	std::shared_ptr<Energybar::Config> config,
	std::shared_ptr<FontAtlas const>   fonts )
	: config{ config }, fonts{ fonts }
{
}

void April::Gui::Settings_Energybar::Draw()
{
	ImGui::TextWrapped(
		"The Energybar Overlay is inspired by the in-game Energybar; will, "
		"however, additionally display the maximum energy points and the "
		"number of regeneration and degeneration pips.\n"
		"It is supposed to be used as a Overlay covering the in-game "
		"Energybar." );

	ImGui::SeparatorSpaced();

	DrawWindowSettings( config->window );
	DrawFontSelector( "Font", config->font, *fonts );

	ImGui::SeparatorSpaced();

	ImGui::DragFloat2( "Energybar Size", &config->size.x, .01f, 0, 0, "%.f" );

	ImGui::SeparatorSpaced();

	ImGui::ColorEdit4( "Energy Color", &config->color.x );
	ImGui::ColorEdit4( "Background", &config->background_color.x );
	ImGui::ColorEdit4( "Overcast", &config->color_overcast.x );
	ImGui::ColorEdit4( "Border Color", &config->border_color.x );

	ImGui::SeparatorSpaced();

	ImGui::SliderFloat2(
		"Degen-Pips Alignment", &config->alignment.left.x, 0, 1, "%.2f" );
	ImGui::SliderFloat2(
		"Regen-Pips Alignment", &config->alignment.right.x, 0, 1, "%.2f" );
}
