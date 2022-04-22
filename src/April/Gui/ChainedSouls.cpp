
#include "April/Gui/ChainedSouls.h"

#include "April/Utility/stl.h"
#include "April/Utility/TimeFormatting.h"

using namespace April;
using namespace GW::literals;
using namespace GW::Packet::StoC;
using namespace std::string_literals;


namespace {

	constexpr const char* names[] = {
		"Entrace Soul",
		"Bridge Soul",
		"Safespot Soul",
		"Bow Soul",
		"Hill Soul",
		"Bottom Soul",
		"Double Soul",
		"Reaper Soul"
	};

	static_assert(
		std::size( names ) == std::tuple_size_v<ChainedSouls::type> );


	float max_name_lenth()
	{
		auto longest = 0.f;
		for ( auto const* name : names )
			longest = std::max( longest, ImGui::CalcTextSize( name ).x );

		return longest;
	}

	auto time_until_death( ChainedSoul::Alive const& alive_soul )
	{
		auto const sec_f = alive_soul.hp / alive_soul.pips * -1.f;
		return GW::sec32{ static_cast<GW::sec32::rep>( sec_f ) };
	}

	template<typename T>
	auto time_until_respawn( T const& state ) -> std::optional<GW::ms32>
	{
		if ( state.time_of_death == std::nullopt )
			return std::nullopt;

		auto const alive_for = GW::GetInstanceTime() - *state.time_of_death;
		return 2_min - alive_for;
	}

	auto color( ChainedSoul const& soul ) -> ImVec4
	{
		constexpr auto get_color = stl::overloaded{
			[]( ChainedSoul::Alive ) { return DarkRed(); },
			[]( ChainedSoul::Dead ) { return DarkGreen(); },
			[]( ChainedSoul::OutOfRange ) { return DarkGreen(); },
		};
		return std::visit( get_color, soul.state() );
	}

	auto label( ChainedSoul const& soul ) -> std::string
	{
		constexpr auto get_label = stl::overloaded{
			[]( ChainedSoul::Alive const& alive_soul )
			{
				auto const death_in = time_until_death( alive_soul );
				return "Death in: " + to_string_mmss( death_in );
			},
			[]( ChainedSoul::Dead const& dead_soul )
			{
				auto const respawn_in = time_until_respawn( dead_soul );
				if ( respawn_in == std::nullopt )
					return "Respawn in: unknown"s;

				return "Respawn in: " + to_string_mmss( *respawn_in );
			},
			[]( ChainedSoul::OutOfRange const& distant_soul )
			{
				auto const respawn_in = time_until_respawn( distant_soul );
				if ( respawn_in == std::nullopt )
					return "Soul not found!"s;

				if ( *respawn_in < 0_ms ) // will have died by now
					return "Death in: unknown"s;

				return "Respawn in: " + to_string_mmss( *respawn_in );
			}
		};
		return std::visit( get_label, soul.state() );
	}

	float float_divide( GW::ms32 const& lhs, GW::ms32 const& rhs )
	{
		using ms_f = std::chrono::duration<float, std::milli>;
		return ms_f{ lhs } / ms_f{ rhs };
	}

	float filled( ChainedSoul const& soul )
	{
		constexpr auto get_fraction = stl::overloaded{
			[]( ChainedSoul::Alive const& alive_soul )
			{
				return alive_soul.hp;
			},
			[]( ChainedSoul::Dead const& dead_soul )
			{
				auto const respawn_in = time_until_respawn( dead_soul );
				if ( respawn_in == std::nullopt )
					return 0.f;

				return 1.f - float_divide( *respawn_in, 2_min );
			},
			[]( ChainedSoul::OutOfRange const& distant_soul )
			{
				auto const respawn_in = time_until_respawn( distant_soul );
				if ( respawn_in == std::nullopt )
					return 0.f;

				if ( *respawn_in < 0_ms ) // will have died by now
					return 0.f;

				return 1.f - float_divide( *respawn_in, 2_min );
			}
		};
		return std::visit( get_fraction, soul.state() );
	}

}


April::Gui::ChainedSouls::ChainedSouls(
	std::shared_ptr<Config>                    config,
	std::shared_ptr<Mouse>                     mouse,
	std::shared_ptr<April::ChainedSouls const> souls )
	: config{ config }, mouse{ mouse }, souls{ souls }
{
}

void April::Gui::ChainedSouls::Display()
{
	ImGui::SetNextWindowPosCentered( ImGuiCond_FirstUseEver );
	ImGui::SetNextWindowSize( { 300, -1 }, ImGuiCond_FirstUseEver );

	if ( ImGui::Begin( config->window, *mouse ) )
	{
		auto const bar_offset =
			max_name_lenth()
			+ ImGui::GetStyle().WindowPadding.x
			+ ImGui::GetStyle().ItemInnerSpacing.x * 2
			+ ImGui::GetStyle().ItemSpacing.x;

		for ( auto it = 0u; it < std::size( names ); ++it )
		{
			auto const& soul = souls->Get()[it];

			ImGui::PushID( it );

			ImGui::PushStyleColor( ImGuiCol_PlotHistogram, color( soul ) );
			ImGui::PushStyleColor( ImGuiCol_Button, Invisible() );

			auto const time_str = label( soul );
			if ( ImGui::Button( names[it] ) )
			{
				GW::SendChat( '#', "["s + names[it] + "] " + time_str );
			}
			ImGui::SameLine( bar_offset );
			ImGui::ProgressBar( filled( soul ), { -1, 0 }, time_str.c_str() );

			ImGui::PopStyleColor( 2 );

			ImGui::PopID();
		}
	}
	ImGui::End();
}

void April::Gui::ChainedSouls::Update()
{
	auto const* player = GW::GetCharacter();
	if ( player == nullptr || GW::GetMapID() != GW::MapID::The_Underworld )
		return;

	auto const distance = GW::GetDistance( player->pos, { 11427, 5079 } );
	if ( not in_pits && distance < GW::Constants::Range::Compass )
	{
		in_pits = true;

		if ( config->auto_set_visibility )
			config->window.visible = true;
	}
	else if ( in_pits && distance > GW::Constants::Range::Compass )
	{
		in_pits = false;

		if ( config->auto_set_visibility )
			config->window.visible = false;
	}
}

void April::Gui::ChainedSouls::Update( MapLoaded const& )
{
	in_pits = false;

	if ( config->auto_set_visibility )
		config->window.visible = false;
}
