
#include "April/Gui/Settings/Settings.Skillbar.h"

#include "April/Utility/SettingsWidgets.h"

#include "April/Utility/TimeFormatting.h"

#include "ImGui/ImGui.hpp"

#include <algorithm>


namespace {

	constexpr auto popup_id = "New Threshold";


	auto layout_string( April::Gui::Skillbar::Config const& config )
	{
		if ( config.skills_per_row == 8 ) return "1 row";
		if ( config.skills_per_row == 4 ) return "2 rows";
		if ( config.skills_per_row == 2 ) return "2 columns";
		if ( config.skills_per_row == 1 ) return "1 column";
		return "Please select your layout";
	};

}


April::Gui::Settings_Skillbar::Settings_Skillbar(
	std::shared_ptr<Skillbar::Config> config,
	std::shared_ptr<FontAtlas const>  fonts )
	: config{ config }, fonts{ fonts }
{
}

void April::Gui::Settings_Skillbar::Draw()
{
	ImGui::TextWrapped(
		"The Skillbar Overlay is supposed to be an immovable overlay over "
		"your skillbar. It will display more precise information for your "
		"skill-cooldowns and the remaining effect-uptime of your skillbar-"
		"skills." );

	ImGui::SeparatorSpaced();

	DrawWindowSettings( config->window );

	ImGui::SeparatorSpaced();

	ImGui::Text( "Layout & Size" );
	if ( ImGui::BeginCombo( "Layout", layout_string( *config ) ) )
	{
		if ( ImGui::Selectable( "1 row", config->skills_per_row == 8 ) )
			config->skills_per_row = 8;
		if ( ImGui::Selectable( "2 rows", config->skills_per_row == 4 ) )
			config->skills_per_row = 4;
		if ( ImGui::Selectable( "2 columns", config->skills_per_row == 2 ) )
			config->skills_per_row = 2;
		if ( ImGui::Selectable( "1 column", config->skills_per_row == 1 ) )
			config->skills_per_row = 1;

		ImGui::EndCombo();
	}

	if (
		ImGui::DragFloat(
			"Skill Slot Size", &config->skill_size.x, .01f, 0, 0, "%.0f" ) )
	{
		config->skill_size.y = config->skill_size.x;
	}

	ImGui::SeparatorSpaced();

	ImGui::Text( "Skill Cooldown" );
	ImGui::ColorEdit4( "Cooldown", &config->color_cooldown.x );
	ImGui::ColorEdit4( "Reduced Cooldown", &config->color_reduced_cd.x );
	ImGui::SameLine();
	ImGui::HelpMarker(
		"Different color to indicate the recharge time was reduced by "
		"any means other than Essence of Celerity and/or Fast Casting." );
	DrawFontSelector( "Font", config->font, *fonts );

	ImGui::SeparatorSpaced();

	ImGui::Text( "Effect Uptime" );
	auto& thresholds = config->thresholds;
	auto const rect = ImGui::GetFrameHeight();
	auto const& color = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
	auto const space = ImVec2{
		ImGui::GetStyle().ItemInnerSpacing.x,
		ImGui::GetStyle().ItemSpacing.y
	};

	ImGui::PushStyleColor( ImGuiCol_Button, color );
	ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, space );
	for ( auto it = thresholds.begin(); it != thresholds.end(); /**/ )
	{
		ImGui::PushID( &*it );

		auto const label =
			"When longer than " + to_string_sff( it->uptime ) + "s";

		auto new_color = it->color;
		if ( ImGui::ColorEdit4( "", &new_color.x ) )
			thresholds.replace( it, { it->uptime, new_color } );

		ImGui::SameLine();
		if ( ImGui::Button( "X", { rect, rect } ) )
		{
			it = thresholds.erase( it );
		}
		else ++it;

		ImGui::SameLine();
		ImGui::Text( label );

		ImGui::PopID();
	}
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();

	if ( ImGui::Button( "Add new Threshold", { ImGui::CalcItemWidth(), 0 } ) )
		ImGui::OpenPopup( popup_id );

	if ( ImGui::BeginPopup( popup_id ) )
	{
		ImGui::InputDuration(
			"when longer than ... milliseconds",
			new_threshold.uptime, 50, 1000 );
		ImGui::ColorEdit4( "Color", &new_threshold.color.x );

		if ( ImGui::Button( "Add", { ImGui::CalcItemWidth(), 0 } ) )
		{
			thresholds.push_back( new_threshold );
			new_threshold = make_threshold();
		}

		ImGui::EndPopup();
	}
}

auto April::Gui::Settings_Skillbar::make_threshold()
	-> Threshold
{
	return { GW::ms32{ 0 }, White( .25f ) };
}
