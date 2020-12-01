
#include "April/Gui/UwTimes.h"

#include "April/Config/Gui/UwTimesGui.config.hpp"
#include "April/Framework/WndProc.h"
#include "April/Utility/DrawStructures.h"
#include "April/Utility/TimeFormatting.h"

#include "Dependencies/ImGui.hpp"

#include <array>
#include <chrono>
#include <exception>
#include <optional>
#include <string>

namespace config = April::Gui::UwTimesGuiConfig;

using namespace std::chrono;
using namespace std::string_literals;


namespace {

	inline constexpr auto chamber = 0, restore = 1, escort = 2, uwg = 3, 
		vale = 4, wastes = 5, pits = 6, plains = 7, mnts = 8, pools = 9, 
		dhuum = 10;
	inline constexpr auto rows = 11;

	inline constexpr auto pops = 0, takes = 1, dones = 2, durations = 3;
	inline constexpr auto columns = 4;

	inline constexpr auto lab_reaper = 0;

	using TimeStrings = std::array<std::array<std::string, rows>, columns>;


	auto to_string( milliseconds const time )
	{
		return time > 0ms
			? to_string( time, config::quest_time_fmt ) 
			: config::default_quest_time;
	}

	constexpr auto duration(
		milliseconds const start, 
		milliseconds const end, 
		milliseconds const now )
		-> milliseconds
	{
		if ( start == 0ms ) return 0ms;
		if ( end == 0ms ) return now - start;
		
		return end - start;
	}

	auto uwtimes_to_strings( April::UwTimes const& times )
		-> TimeStrings
	{
		auto strings = TimeStrings{};

		// Pops
		auto const lab_pop = to_string( times.pop[lab_reaper] );
		strings[pops][chamber] = config::default_quest_time;
		strings[pops][restore] = lab_pop;
		strings[pops][escort] = lab_pop;
		strings[pops][uwg] = lab_pop;
		for ( auto it = vale; it <= pools; ++it )
		{
			strings[pops][it] = to_string( times.pop[it - uwg] );
		}
		strings[pops][10] = to_string( times.nightman_cometh );

		// Takes
		for ( auto it = chamber; it <= pools; ++it )
		{
			strings[takes][it] = to_string( times.take[it] );
		}
		strings[takes][dhuum] = to_string( times.dhuum_hostile );

		// Dones
		for ( auto it = chamber; it <= pools; ++it )
		{
			strings[dones][it] = to_string( times.done[it] );
		}
		strings[dones][dhuum] = to_string( times.dhuum_done );

		// Times
		for ( auto it = chamber; it <= pools; ++it )
		{
			auto const time = duration(
				times.take[it], times.done[it], times.instance_time );

			strings[durations][it] = to_string( time );
		}
		auto const time = duration(
			times.dhuum_hostile, times.dhuum_done, times.instance_time );
		strings[durations][dhuum] = to_string( time );

		return strings;
	}

	auto full_info( int const objective, TimeStrings const& strings )
	{
		using namespace config;
		assert( objective >= chamber && objective <= dhuum );

		return "["s + quest_names[objective] + "] "
			+ column_names[pops] + ' ' + strings[pops][objective] + " | " 
			+ column_names[takes] + ' ' + strings[takes][objective] + " | " 
			+ column_names[dones] + ' ' + strings[dones][objective] + " | " 
			+ column_names[durations] + ' ' + strings[durations][objective];
	}

	auto chat_str( 
		int const objective, int const column, TimeStrings const& strings )
	{
		assert( objective >= chamber && objective <= dhuum );
		assert( column >= pops && column <= durations );

		auto const quest_name = config::quest_names[objective];
		auto const column_name = config::column_names[column];
		auto const time_str = strings[column][objective];

		return "["s + quest_name + '|' + column_name + "] " + time_str; 
	}

}


April::Gui::UwTimesGui::UwTimesGui( std::shared_ptr<UwTimes const> times )
	: times{ std::move( times ) }
{
}

void April::Gui::UwTimesGui::Display() const
{
	auto const strings = uwtimes_to_strings( *times );
	auto const instance_time_str = 
		to_string( times->instance_time, config::instance_time_fmt );

	ImGui::Begin( config::window_name, config::window_flags );
	if ( ImGui::IsWindowHovered() )
	{
		WndProc::BlockMouseInput();
	}
	ImGui::PushStyleColor( ImGuiCol_Button, Invisible() );
	ImGui::PushStyleColor( ImGuiCol_ButtonActive, White( 0.5f ) );
	ImGui::PushStyleColor( ImGuiCol_ButtonHovered, White( 0.2f ) );
	ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, { 0, 0 } );
	ImGui::Columns( 5 );
	{
		// Current Time
		if ( ImGui::SmallButton( instance_time_str ) )
		{
			GW::SendChat( '#', "[/age] " + instance_time_str );
		}
		ImGui::NextColumn();

		// Headers
		for ( auto it = pops; it <= durations; ++it )
		{
			ImGui::Text( config::column_names[it] ); 
			ImGui::NextColumn();
		}
		ImGui::Separator();

		// Quests
		for ( auto objective = chamber; objective <= dhuum; ++objective )
		{
			// Quest Name -> full info on click
			ImGui::PushID( objective * -1 );
			if ( ImGui::SmallButton( config::quest_names[objective] ) )
			{
				auto const str = full_info( objective, strings );
				GW::SendChat( '#', str );
			}
			ImGui::PopID();
			ImGui::NextColumn();

			// Indiviadual times
			for ( auto column = pops; column <= durations; ++column )
			{
				ImGui::PushID( objective * columns + column );
				if ( ImGui::SmallButton( strings[column][objective] ) )
				{
					auto const str = chat_str( objective, column, strings );
					GW::SendChat( '#', str );
				}
				ImGui::PopID();
				ImGui::NextColumn();
			}
		}				
	}
	ImGui::Columns( 1 );
	ImGui::PopStyleVar();
	ImGui::PopStyleColor( 3 );
	ImGui::End();
}
