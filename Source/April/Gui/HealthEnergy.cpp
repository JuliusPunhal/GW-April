
#include "April/Gui/HealthEnergy.h"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"

#include <string>

using April::RGBA;
using April::XY;
using April::WH;
using April::Gui::Healthbar;
using April::Gui::Energybar;


namespace {

	struct Label {
		std::string curr_max;
		std::string pips;
	};

	struct Style {
		ImFont* font;

		RGBA	bar_color;
		RGBA	background_color;
		RGBA	border_color;

		float	percent_filled; // 0 < percent_filled < 1

		XY		pips_alignment;
	};


	void Draw( Label const& label, Style const& style )
	{
		using namespace April;

		ImGui::PushFont( style.font );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, Invisible() );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, Invisible() );

		// Bar
		auto const cursor = ImGui::GetCursorPos();
		ImGui::PushStyleColor( ImGuiCol_Border, style.border_color );
		ImGui::PushStyleColor( ImGuiCol_FrameBg, style.background_color );
		ImGui::PushStyleColor( ImGuiCol_PlotHistogram, style.bar_color );
		ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1 );
		{
			ImGui::ProgressBar( style.percent_filled, { -1, -1 }, "" );
		}
		ImGui::PopStyleVar();
		ImGui::PopStyleColor( 3 );

		// Label
		ImGui::SetCursorPos( cursor );
		ImGui::PushStyleColor( ImGuiCol_Button, Invisible() );
		{
			ImGui::Button( label.curr_max.c_str(), { -1, -1 } );
		}
		ImGui::PopStyleColor();

		// Pips
		ImGui::SetCursorPos( cursor );
		ImGui::PushStyleColor( ImGuiCol_Button, Invisible() );
		ImGui::PushStyleVar(
			ImGuiStyleVar_ButtonTextAlign, style.pips_alignment );
		{
			ImGui::Button( label.pips.c_str(), { -1, -1 } );
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		ImGui::PopStyleColor( 2 );
		ImGui::PopFont();
	}

	auto get_label( float const percent, unsigned const max ) -> std::string
	{
		auto const current = static_cast<int>( percent * max );

		auto const curr_str = std::to_string( current );
		auto const max_str = std::to_string( max );

		return curr_str + "/" + max_str;
	}

	auto pips_to_string( int const pips ) -> std::string
	{
		if ( pips > 0 )
		{
			char buffer[8];
			snprintf( buffer, sizeof( buffer ), "(+%d)", pips );
			return { buffer + std::string( pips, '>' ) };
		}

		if ( pips < 0 )
		{
			char buffer[8];
			snprintf( buffer, sizeof( buffer ), "(%d)", pips );
			return std::string( std::abs( pips ), '<' ).append( buffer );
		}

		return "";
	}

	constexpr auto color_by_effect(
		uint32_t const effects,
		Healthbar::Config::Colors const& colors ) noexcept
	{
		if ( (effects & 0x0400) == 0x0400 ) return colors.degen_hexed;
		if ( (effects & 0x0020) == 0x0020 ) return colors.deep_wounded;
		if ( (effects & 0x0001) == 0x0001 ) return colors.bleeding;
		if ( (effects & 0x0040) == 0x0040 ) return colors.poisoned;

		return colors.standard;
	}

}


April::Gui::Healthbar::Healthbar( Config const& config )
	: config{ config }, font{ LoadFont( config.font ) }
{
}

void April::Gui::Healthbar::Display() const
{
	auto const player = GW::Agents::GetCharacter();
	if ( player == nullptr ) return;

	auto const loss_per_sec = player->hp_pips * player->max_hp;
	auto const pips_f = std::round( loss_per_sec * 0.5f );
	auto const pips = static_cast<int>( pips_f );

	auto const label = Label{
		get_label( player->hp, player->max_hp ),
		pips_to_string( pips )
	};
	auto const draw_style = ::Style{
		font,

		color_by_effect( player->effects, config.colors ),
		config.background_color,
		config.border_color,

		player->hp,
		pips < 0 ? config.alignment.left : config.alignment.right
	};

	if ( ImGui::Begin( config.window ) )
	{
		Draw( label, draw_style );
	}
	ImGui::End();
}

auto April::Gui::Healthbar::Config::LoadDefault() -> Config
{
	constexpr auto window_flags =
		ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoScrollWithMouse
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoBackground
		| ImGuiWindowFlags_NoMouseInputs
		| ImGuiWindowFlags_NoFocusOnAppearing
		| ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoNavInputs
		| ImGuiWindowFlags_NoNavFocus;

	auto const config = Config{
		White(),
		{ 0.1f, 0.1f, 0.1f, 1 },
		{
			{ 0.70f, 0.00f, 0.00f, 1 },
			{ 0.55f, 0.00f, 0.55f, 1 },
			{ 0.50f, 0.50f, 0.50f, 1 },
			{ 0.77f, 0.57f, 0.38f, 1 },
			{ 0.50f, 0.50f, 0.00f, 1 }
		},
		{ { 0.1f, 0.5f }, { 0.9f, 0.5f } },
		{ "C:\\Windows\\Fonts\\Consola.ttf", 14 },
		{ "Healthbar", true, window_flags }
	};

	return config;
}

April::Gui::Energybar::Energybar( Config const& config )
	: config{ config }, font{ LoadFont( config.font ) }
{
}

void April::Gui::Energybar::Display() const
{
	auto const player = GW::Agents::GetCharacter();
	if ( player == nullptr ) return;

	auto const loss_per_sec = player->energy_regen * player->max_energy;
	auto const pips_f = std::round( loss_per_sec * 3 );
	auto const pips = static_cast<int>( pips_f );

	auto label = Label{
		get_label( player->energy, player->max_energy ),
		pips_to_string( pips )
	};
	auto const draw_style = ::Style{
		font,

		config.colors.standard,
		config.background_color,
		config.border_color,

		player->energy,
		pips < 0 ? config.alignment.left : config.alignment.right
	};

	if ( ImGui::Begin( config.window ) )
	{
		Draw( label, draw_style );
	}
	ImGui::End();
}

auto April::Gui::Energybar::Config::LoadDefault() -> Config
{
	constexpr auto window_flags =
		ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoScrollWithMouse
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoBackground
		| ImGuiWindowFlags_NoMouseInputs
		| ImGuiWindowFlags_NoFocusOnAppearing
		| ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoNavInputs
		| ImGuiWindowFlags_NoNavFocus;

	auto const config = Config{
		White(),
		{ 0.1f, 0.1f, 0.1f, 1 },
		{ 0, 0, 0.7f, 1 },
		{ { 0.1f, 0.5f }, { 0.9f, 0.5f } },
		{ "C:\\Windows\\Fonts\\Consola.ttf", 14 },
		{ "Energybar", true, window_flags }
	};

	return config;
}
