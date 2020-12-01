
#include "April/Gui/HealthEnergy.h"

#include "April/Config/Gui/HealthEnergy.Config.hpp"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"

#include <string>

namespace config = April::Gui::HealthEnergyConfig;

using April::RGBA;
using April::XY;
using April::WH;


namespace {
	
	struct Label {
		std::string curr_max;
		std::string pips;

		ImFont* font;
		XY		pips_alignment;
		RGBA	color;
		float	percent_filled; // 0 < percent_filled < 1
	};

	struct Style {
		RGBA	font_color;
		RGBA	background_color;
		RGBA    border_color;
	};
	

	void Draw( Label const& label, Style const& style )
	{
		using namespace April;

		ImGui::PushFont( label.font );
		ImGui::PushStyleColor( ImGuiCol_Text, style.font_color );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, Invisible() );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, Invisible() );

		// Bar
		auto const cursor = ImGui::GetCursorPos();
		ImGui::PushStyleColor( ImGuiCol_Border, style.border_color );
		ImGui::PushStyleColor( ImGuiCol_FrameBg, style.background_color );
		ImGui::PushStyleColor( ImGuiCol_PlotHistogram, label.color );
		ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1 );
		{
			ImGui::ProgressBar( label.percent_filled, { -1, -1 }, "" );
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
			ImGuiStyleVar_ButtonTextAlign, label.pips_alignment );
		{
			ImGui::Button( label.pips.c_str(), { -1, -1 } );
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		ImGui::PopStyleColor( 3 );
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
	
	constexpr auto color_by_effect( unsigned const effects ) noexcept
	{
		using namespace config::health;

		if ( (effects & 0x0400) == 0x0400 ) return colors.degen_hexed;
		if ( (effects & 0x0020) == 0x0020 ) return colors.deep_wounded;
		if ( (effects & 0x0001) == 0x0001 ) return colors.bleeding;
		if ( (effects & 0x0040) == 0x0040 ) return colors.poisoned;

		return colors.standard;
	}

}


April::Gui::Healthbar::Healthbar()
	: font{ LoadFont( config::health::font_path, config::health::font_size ) }
{
}

void April::Gui::Healthbar::Display() const
{
	auto const player = GW::Agents::GetCharacter();
	if ( player == nullptr ) return;

	auto const loss_per_sec = player->hp_pips * player->max_hp;
	auto const pips_f = std::round( loss_per_sec * 0.5f );
	auto const pips = static_cast<int>( pips_f );

	auto label = Label{};
	label.curr_max = get_label( player->hp, player->max_hp );
	label.pips = pips_to_string( pips );
	label.font = font;
	label.pips_alignment = { pips < 0 ? 0.1f : 0.9f, 0.5f };
	label.color = color_by_effect( player->effects );
	label.percent_filled = player->hp;

	ImGui::Begin( config::health::window_name, config::health::window_flags );
	{
		auto const style = Style{
			config::health::text_color,
			config::health::background,
			config::health::border_color
		};
		Draw( label, style );
	}
	ImGui::End();
}

April::Gui::Energybar::Energybar()
	: font{ LoadFont( config::energy::font_path, config::energy::font_size ) }
{
}

void April::Gui::Energybar::Display() const
{
	auto const player = GW::Agents::GetCharacter();
	if ( player == nullptr ) return;

	auto const loss_per_sec = player->energy_regen * player->max_energy;
	auto const pips_f = std::round( loss_per_sec * 3 );
	auto const pips = static_cast<int>( pips_f );

	auto label = Label{};
	label.curr_max = get_label( player->energy, player->max_energy );
	label.pips = pips_to_string( pips );
	label.font = font;
	label.pips_alignment = { pips < 0 ? 0.1f : 0.9f, 0.5f };
	label.color = config::energy::color;
	label.percent_filled = player->energy;

	ImGui::Begin( config::energy::window_name, config::energy::window_flags );
	{
		auto const style = Style{
			config::energy::text_color,
			config::energy::background,
			config::energy::border_color
		};
		Draw( label, style  );
	}
	ImGui::End();
}
