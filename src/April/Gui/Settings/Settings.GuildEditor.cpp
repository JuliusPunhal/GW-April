
#include "April/Gui/Settings/Settings.GuildEditor.h"

#include "April/Utility/SettingsWidgets.h"

#include "GWCA/GWCA.hpp"

#include "ImGui/ImGui.hpp"

#include <array>

using namespace April::Module;
using namespace GW::Packet::StoC;

using CapeStyle = April::Module::GuildEditor::CapeStyle;


namespace {

	bool draw_id_selector(
		const char* label, int& id, int const min, int const max )
	{
		auto value_changed = false;
		if ( ImGui::InputInt( label, &id ) )
		{
			value_changed = true;
			if ( id < min ) id = max;
			if ( id > max ) id = min;
		}

		return value_changed;
	}

}

namespace {

	struct ColorInfo {
		const char* name;
		int         hue;
	};

	constexpr auto colors = std::array<ColorInfo, 16>{
		"Red", 0,
		"Orange", 1,
		"Brown", 2,
		"Dark Yellow", 3,
		"Dark Green", 4,
		"Green", 5,
		"Yellow", 6,
		"Cyan", 7,
		"Blue", 8,
		"Dark Blue", 9,
		"Turquoise", 10,
		"Purple", 11,
		"Violet", 12,
		"Magenta", 13,
		"Pink",  14,
		"Black/White", 15
	};


	bool draw_hue( int& hue, const char* label )
	{
		auto is_selected =
			[&]( ColorInfo const& info ) { return info.hue == hue; };

		auto const selected =
			std::find_if( colors.begin(), colors.end(), is_selected );

		auto const preview =
			(selected != colors.end()) ? selected->name : "Unknown";

		auto value_changed = false;
		if ( ImGui::BeginCombo( label, preview ) )
		{
			for ( auto const& col : colors )
			{
				if ( ImGui::Selectable( col.name, hue == col.hue ) )
				{
					hue = col.hue;
					value_changed = true;
				}
			}

			ImGui::EndCombo();
		}

		return value_changed;
	}

}

namespace {

	bool draw_color(
		int& color, const char* label_hue, const char* label_brightness )
	{
		auto hue = (color & 0xF0) >> 4;
		auto brightness = color & 0x0F;

		ImGui::PushID( &color );

		auto value_changed = false;
		if ( draw_hue( hue, label_hue ) )
			value_changed = true;

		if ( ImGui::SliderInt( label_brightness, &brightness, 0, 15 ) )
			value_changed = true;

		ImGui::PopID();

		color = (hue << 4) | brightness;

		return value_changed;
	}

}

namespace {

	struct TrimInfo {
		const char* name;
		int         id;
	};

	constexpr auto trims = std::array<TrimInfo, 14>{
		"No Trim", 0,
		"Gold", 2,
		"Silver 1", 1,
		"Silver 2", 12,
		"Silver 3", 10,
		"Bronze", 3,
		"Obsidian", 9,
		"Red", 4,
		"Blue", 5,
		"Green", 6,
		"Violet", 7,
		"Orange", 8,
		"Pink", 11,
		"White", 13
	};


	bool draw_trim_selector( CapeStyle& config )
	{
		auto is_selected = [&]( TrimInfo const& info )
		{
			return info.id == config.trim;
		};

		auto const selected =
			std::find_if( trims.begin(), trims.end(), is_selected );

		auto const preview =
			(selected != trims.end()) ? selected->name : "Unknown";

		auto value_changed = false;
		if ( ImGui::BeginCombo( "Cape Trim", preview ) )
		{
			for ( auto const& trim : trims )
			{
				auto const highlighted = config.trim == trim.id;
				if ( ImGui::Selectable( trim.name, highlighted ) )
				{
					config.trim = trim.id;
					value_changed = true;
				}
			}

			ImGui::EndCombo();
		}

		return value_changed;
	}

}

namespace {

	void toggle_cape()
	{
		if ( GW::GetInstanceType() == GW::InstanceType::Loading )
			return;

		auto const* player = GW::GetCharacter();
		if ( player == nullptr || player->GetIsHidingCape() )
			return;

		GW::EmulatePacket( CapeVisibility{ player->agent_id, 0u } );
		GW::EmulatePacket( CapeVisibility{ player->agent_id, 1u } );
	}

	void apply_name(
		GuildEditor const& guild_editor, UpdateGuildInfo packet_cpy )
	{
		guild_editor.ApplyNameToPacket( packet_cpy );
		GW::EmulatePacket<UpdateGuildInfo>( packet_cpy );
		// TODO: toggle guild window
	}

	void apply_tag(
		GuildEditor const& guild_editor, UpdateGuildInfo packet_cpy )
	{
		guild_editor.ApplyTagToPacket( packet_cpy );
		GW::EmulatePacket<UpdateGuildInfo>( packet_cpy );
		// TODO: toggle guild window
		// TODO: toggle name tags
	}

	void apply_cape(
		GuildEditor const& guild_editor, UpdateGuildInfo packet_cpy )
	{
		guild_editor.ApplyCapeToPacket( packet_cpy );
		GW::EmulatePacket<UpdateGuildInfo>( packet_cpy );
		toggle_cape();
	}

}


April::Gui::Settings_GuildEditor::Settings_GuildEditor(
	std::shared_ptr<Module::GuildEditor>         guild_editor,
	std::shared_ptr<Module::GuildEditor::Config> config )
	: guild_editor{ guild_editor }, config{ config }
{
}

void April::Gui::Settings_GuildEditor::Draw()
{
	ImGui::TextWrapped(
		"When activated, this module will change the name, tag, and cape of "
		"the specified guild." );

	ImGui::SeparatorSpaced();

	ImGui::InputText( "Guild to edit", &config->guild_to_edit );
	ImGui::SameLine();
	ImGui::HelpMarker(
		"Enter the full Guild Name of the guild you want to change." );

	ImGui::SeparatorSpaced();

	auto const& unchanged = guild_editor->Unmodified();
	if ( unchanged == std::nullopt )
	{
		ImGui::TextWrapped(
			"Please zone into another instance to enable these "
			"configurations." );

		return;
	}

	DrawActivateButton(
		config->active_name,
		"Deactivate Custom Name",
		"Activate Custom Name" );

	DrawActivateButton(
		config->active_tag,
		"Deactivate Custom Tag",
		"Activate Custom Tag" );

	DrawActivateButton(
		config->active_cape,
		"Deactivate Custom Cape",
		"Activate Custom Cape" );

	ImGui::SeparatorSpaced();

	auto const width = ImGui::CalcItemWidth();
	if ( ImGui::Button( "Reset in-game changes", { width, 0 } ) )
	{
		GW::EmulatePacket<UpdateGuildInfo>( *unchanged );
		toggle_cape();
	}
	ImGui::SameLine();
	ImGui::HelpMarker(
		"Reverts all in-game-changes to show the unmodified guild but does "
		"not alter your configuration in April." );

	ImGui::SeparatorSpaced();

	if ( ImGui::InputText( "New Guild Name", &config->new_name ) )
		apply_name( *guild_editor, *unchanged );

	if ( ImGui::InputText( "New Guild Tag", &config->new_tag ) )
		apply_tag( *guild_editor, *unchanged );

	ImGui::SeparatorSpaced();

	auto apply_id_selector = [&](
		const char* label, int& id, int const min, int const max )
	{
		if ( draw_id_selector( label, id, min, max ) )
			apply_cape( *guild_editor, *unchanged );
	};
	auto apply_color = [&](
		int& color, const char* label_hue, const char* label_brightness )
	{
		if ( draw_color( color, label_hue, label_brightness ) )
			apply_cape( *guild_editor, *unchanged );
	};
	auto apply_trim_selector = [&]( CapeStyle& cape )
	{
		if ( draw_trim_selector( cape ) )
			apply_cape( *guild_editor, *unchanged );
	};

	ImGui::Text( "Custom Guild Cape" );
	constexpr auto label = "Set cape-configuration to in-game cape";
	if ( ImGui::Button( label, { ImGui::CalcItemWidth(), 0 } ) )
	{
		auto& cfg = config->new_cape;
		cfg.background_color = unchanged->cape_background_color;
		cfg.detail_color =     unchanged->cape_detail_color;
		cfg.emblem_color =     unchanged->cape_emblem_color;
		cfg.shape =            unchanged->cape_shape;
		cfg.detail =           unchanged->cape_detail;
		cfg.emblem =           unchanged->cape_emblem;
		cfg.trim =             unchanged->cape_trim;
	}

	ImGui::MoveCursorPosY( ImGui::GetFrameHeight() * 0.5f );
	apply_id_selector( "Shape", config->new_cape.shape, 0, 8 );

	ImGui::MoveCursorPosY( ImGui::GetFrameHeight() * 0.5f );
	apply_color(
		config->new_cape.background_color,
		"Background Color",
		"Background Brightness" );

	ImGui::MoveCursorPosY( ImGui::GetFrameHeight() * 0.5f );
	apply_id_selector( "Detail", config->new_cape.detail, 0, 31 );
	apply_color(
		config->new_cape.detail_color, "Detail Color", "Detail Brightness" );

	ImGui::MoveCursorPosY( ImGui::GetFrameHeight() * 0.5f );
	apply_id_selector( "Emblem", config->new_cape.emblem, 0, 173 );
	apply_color(
		config->new_cape.emblem_color, "Emblem Color", "Emblem Brightness" );

	ImGui::MoveCursorPosY( ImGui::GetFrameHeight() * 0.5f );
	apply_trim_selector( config->new_cape );
}
