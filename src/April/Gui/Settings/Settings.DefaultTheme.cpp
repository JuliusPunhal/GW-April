
#include "April/Gui/Settings/Settings.DefaultTheme.h"

#include "April/Utility/SettingsWidgets.h"

#include "ImGui/ImGui.hpp"


April::Gui::Settings_DefaultTheme::Settings_DefaultTheme(
	std::shared_ptr<DefaultTheme::Config> config,
	std::shared_ptr<FontAtlas const>      fonts )
	: config{ config }, fonts{ fonts }
{
}

void April::Gui::Settings_DefaultTheme::Draw()
{
	auto& style = config->style;

	ImGui::TextWrapped(
		"Use the settings below to adjust the global theme for all the "
		"elements that cannot be configurated separately." );

	ImGui::SeparatorSpaced();

	DrawFontSelector( "Default Font", config->font, *fonts );

	ImGui::SeparatorSpaced();

	ImGui::Text("Main");
	ImGui::SliderFloat2(
		"WindowPadding", &style.WindowPadding.x, 0, 20, "%.0f" );
	ImGui::SliderFloat2(
		"FramePadding", &style.FramePadding.x, 0, 20, "%.0f" );
	ImGui::SliderFloat2(
		"CellPadding", &style.CellPadding.x, 0, 20, "%.0f" );
	ImGui::SliderFloat2(
		"ItemSpacing", &style.ItemSpacing.x, 0, 20, "%.0f" );
	ImGui::SliderFloat2(
		"ItemInnerSpacing", &style.ItemInnerSpacing.x, 0, 20, "%.0f" );
	ImGui::SliderFloat(
		"IndentSpacing", &style.IndentSpacing, 0, 30, "%.0f" );
	ImGui::SliderFloat(
			"ScrollbarSize", &style.ScrollbarSize, 1, 20, "%.0f" );
	ImGui::SliderFloat(
			"GrabMinSize", &style.GrabMinSize, 1, 20, "%.0f" );

	ImGui::Text("Borders");
	ImGui::SliderFloat(
		"WindowBorderSize", &style.WindowBorderSize, 0, 1, "%.0f" );
	ImGui::SliderFloat(
		"ChildBorderSize", &style.ChildBorderSize, 0, 1, "%.0f" );
	ImGui::SliderFloat(
		"PopupBorderSize", &style.PopupBorderSize, 0, 1, "%.0f" );
	ImGui::SliderFloat(
		"FrameBorderSize", &style.FrameBorderSize, 0, 1, "%.0f" );
	ImGui::SliderFloat(
		"TabBorderSize", &style.TabBorderSize, 0, 1, "%.0f" );

	ImGui::Text("Rounding");
	ImGui::SliderFloat(
		"WindowRounding", &style.WindowRounding, 0, 12, "%.0f" );
	ImGui::SliderFloat(
		"ChildRounding", &style.ChildRounding, 0, 12, "%.0f" );
	ImGui::SliderFloat(
		"FrameRounding", &style.FrameRounding, 0, 12, "%.0f" );
	ImGui::SliderFloat(
		"PopupRounding", &style.PopupRounding, 0, 12, "%.0f" );
	ImGui::SliderFloat(
		"ScrollbarRounding", &style.ScrollbarRounding, 0, 12, "%.0f" );
	ImGui::SliderFloat(
		"GrabRounding", &style.GrabRounding, 0, 12, "%.0f" );
	ImGui::SliderFloat(
		"LogSliderDeadzone", &style.LogSliderDeadzone, 0, 12, "%.0f" );
	ImGui::SliderFloat(
		"TabRounding", &style.TabRounding, 0, 12, "%.0f" );

	ImGui::Text("Alignment");
	ImGui::SliderFloat2(
		"WindowTitleAlign", &style.WindowTitleAlign.x, 0, 1, "%.2f" );

	constexpr auto options = "None\0Left\0Right\0";
	auto pos = style.WindowMenuButtonPosition + 1;
	if ( ImGui::Combo( "WindowMenuButton", &pos, options ) )
		style.WindowMenuButtonPosition = pos - 1;

	ImGui::SliderFloat2(
		"ButtonTextAlign", &style.ButtonTextAlign.x, 0, 1, "%.2f" );

	ImGui::SliderFloat2(
		"SelectableTextAlign",
		&style.SelectableTextAlign.x, 0, 1, "%.2f" );

	ImGui::SeparatorSpaced();
	ImGui::Text( "Colors" );
	for ( auto it = 0; it < ImGuiCol_COUNT; ++it )
	{
		ImGui::PushID( it );

		auto const name = ImGui::GetStyleColorName( it );
		ImGui::ColorEdit4( name, &style.Colors[it].x );

		ImGui::PopID();
	}

	ImGui::SeparatorSpaced();
	ImGui::Checkbox(
		"Move windows only from titlebar",
		&config->move_from_titlebar );
}
