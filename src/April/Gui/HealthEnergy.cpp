
#include "April/Gui/HealthEnergy.h"

#include <cmath>
#include <string>

using namespace April;
using namespace April::Gui;


namespace {

	auto overcast_color( Healthbar::Config const& config )
	{
		return config.background_color;
	}
	auto overcast_color( Energybar::Config const& config )
	{
		return config.color_overcast;
	}

	auto max_avail( GW::AgentLiving const&, Healthbar::Config const& )
	{
		return 1.f;
	}
	auto max_avail( GW::AgentLiving const& player, Energybar::Config const& )
	{
		return GW::GetMaxAvailableEnergy( player );
	}

	auto bar_color( GW::AgentLiving const& player, Healthbar::Config const& config )
	{
		auto const effects = player.effects;
		if ( (effects & 0x0400) == 0x0400 ) return config.colors.degen_hexed;
		if ( (effects & 0x0020) == 0x0020 ) return config.colors.deep_wounded;
		if ( (effects & 0x0001) == 0x0001 ) return config.colors.bleeding;
		if ( (effects & 0x0040) == 0x0040 ) return config.colors.poisoned;

		return config.colors.standard;
	}
	auto bar_color( GW::AgentLiving const&, Energybar::Config const& config )
	{
		return config.color;
	}

	auto filled( GW::AgentLiving const& player, Healthbar::Config const& )
	{
		return player.hp;
	}
	auto filled( GW::AgentLiving const& player, Energybar::Config const& )
	{
		return player.energy;
	}

	auto label( GW::AgentLiving const& player, Healthbar::Config const& )
	{
		return
			std::to_string( GW::GetHealthPoints( player ) )
			+ "/" + std::to_string( player.max_hp );
	}
	auto label( GW::AgentLiving const& player, Energybar::Config const& )
	{
		return
			std::to_string( GW::GetEnergyPoints( player ) )
			+ "/" + std::to_string( player.max_energy );
	}

	auto get_pips( GW::AgentLiving const& player, Healthbar::Config const& )
	{
		auto const loss_per_sec = player.hp_pips * player.max_hp;
		auto const pips_f = std::round( loss_per_sec * 0.5f );
		return static_cast<int>( pips_f );
	}
	auto get_pips( GW::AgentLiving const& player, Energybar::Config const& )
	{
		auto const loss_per_sec = player.energy_regen * player.max_energy;
		auto const pips_f = std::round( loss_per_sec * 3 );
		return static_cast<int>( pips_f );
	}

	template<typename Config_t>
	auto pips_alignment( int const pips, Config_t const& config )
		-> auto const&
	{
		return pips < 0 ? config.alignment.left : config.alignment.right;
	}

	auto pips_label( int const pips ) -> std::string
	{
		if ( pips > 0 )
		{
			char buffer[8];
			snprintf( buffer, sizeof( buffer ), "(+%d)", pips );
			return buffer + std::string( pips, '>' );
		}

		if ( pips < 0 )
		{
			char buffer[8];
			snprintf( buffer, sizeof( buffer ), "(%d)", pips );
			return std::string( std::abs( pips ), '<' ) + buffer;
		}

		return "";
	}

	template<typename Config_t>
	void Draw( Config_t const& config, April::FontAtlas const& fonts )
	{
		auto const* player = GW::GetCharacter();
		if ( player == nullptr )
			return;

		ImGui::PushFont( fonts.Get( config.font ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, Invisible() );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, Invisible() );

		// Bar
		auto const cursor = ImGui::GetCursorPos();
		ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1 );

		// percentage to available (e.g. Overcast)
		ImGui::PushStyleColor( ImGuiCol_Border, config.border_color );
		ImGui::PushStyleColor( ImGuiCol_FrameBg, overcast_color( config ) );
		ImGui::PushStyleColor(
			ImGuiCol_PlotHistogram, config.background_color );
		{
			auto const avail = max_avail( *player, config );
			ImGui::ProgressBar( avail, config.size, "" );
		}
		ImGui::PopStyleColor( 3 );

		// percentage from max
		ImGui::SetCursorPos( cursor );
		ImGui::PushStyleColor( ImGuiCol_Border, config.border_color );
		ImGui::PushStyleColor( ImGuiCol_FrameBg, Invisible() );
		ImGui::PushStyleColor(
			ImGuiCol_PlotHistogram, bar_color( *player, config ) );
		{
			ImGui::ProgressBar( filled( *player, config ), config.size, "" );
		}
		ImGui::PopStyleColor( 3 );
		ImGui::PopStyleVar();

		// Label
		ImGui::SetCursorPos( cursor );
		ImGui::PushStyleColor( ImGuiCol_Button, Invisible() );
		{
			ImGui::Button( label( *player, config ), { -1, -1 } );
		}
		ImGui::PopStyleColor();

		// Pips
		auto const pips = get_pips( *player, config );
		ImGui::SetCursorPos( cursor );
		ImGui::PushStyleColor( ImGuiCol_Button, Invisible() );
		ImGui::PushStyleVar(
			ImGuiStyleVar_ButtonTextAlign, pips_alignment( pips, config ) );
		{
			auto const label = pips_label( pips ) + "##avoid_empty_label";
			ImGui::Button( label, { -1, -1 } );
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		ImGui::PopStyleColor( 2 );
		ImGui::PopFont();
	}

}


April::Gui::Healthbar::Healthbar(
	std::shared_ptr<Config>          config,
	std::shared_ptr<FontAtlas const> fonts )
	: config{ config }, fonts{ fonts }
{
}

void April::Gui::Healthbar::Display()
{
	ImGui::SetNextWindowPosCentered( ImGuiCond_FirstUseEver );

	if ( ImGui::Begin( config->window ) )
	{
		Draw( *config, *fonts );
	}
	ImGui::End();
}

April::Gui::Energybar::Energybar(
	std::shared_ptr<Config>          config,
	std::shared_ptr<FontAtlas const> fonts )
	: config{ config }, fonts{ fonts }
{
}

void April::Gui::Energybar::Display()
{
	ImGui::SetNextWindowPosCentered( ImGuiCond_FirstUseEver );

	if ( ImGui::Begin( config->window ) )
	{
		Draw( *config, *fonts );
	}
	ImGui::End();
}
