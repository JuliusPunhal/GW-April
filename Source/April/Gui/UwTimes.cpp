
#include "April/Gui/UwTimes.h"

#include "April/Framework/WndProc.h"
#include "April/Utility/DrawStructures.h"
#include "April/Utility/TimeFormatting.h"

#include "Dependencies/ImGui.hpp"

#include <array>
#include <chrono>
#include <optional>
#include <string>

using Config = April::Gui::UwTimesGui::Config;

using namespace std::chrono;
using namespace std::string_literals;


namespace {

	inline constexpr auto instance_time_fmt = April::HMMSS;
	inline constexpr auto default_instance_time = "--:--:--";
	inline constexpr auto quest_time_fmt = April::MMSS;
	inline constexpr auto default_quest_time = "--:--";

	inline constexpr auto chamber = 0, restore = 1, escort = 2, uwg = 3,
		vale = 4, wastes = 5, pits = 6, plains = 7, mnts = 8, pools = 9,
		dhuum = 10;
	inline constexpr auto rows = 11;

	inline constexpr auto name = -1, pops = 0, takes = 1, dones = 2,
		durations = 3;
	inline constexpr auto columns = 4;

	inline constexpr auto lab_reaper = 0;

	using TimeStrings = std::array<std::array<std::string, rows>, columns>;


	auto to_string( milliseconds const time )
	{
		return time > 0ms
			? to_string( time, quest_time_fmt ) : default_quest_time;
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
		strings[pops][chamber] = default_quest_time;
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

	auto draw_objective(
		int const objective, TimeStrings const& strings, Config const& config )
		-> std::optional<int>
	{
		assert( objective >= chamber && objective <= dhuum );

		auto send_chat = std::optional<int>{};

		// Quest Name -> full column on click
		ImGui::PushID( objective * -1 );
		if ( ImGui::SmallButton( config.quest_names[objective] ) )
		{
			send_chat = name;
		}
		ImGui::PopID();
		ImGui::NextColumn();

		// Indiviadual times
		for ( auto column = pops; column <= durations; ++column )
		{
			ImGui::PushID( objective * columns + column );
			if ( ImGui::SmallButton( strings[column][objective] ) )
			{
				send_chat = column;
			}
			ImGui::PopID();
			ImGui::NextColumn();
		}

		return send_chat;
	}

	auto full_info(
		int const objective, TimeStrings const& strings, Config const& config )
	{
		assert( objective >= chamber && objective <= dhuum );

		auto const time_str = [objective, &strings, &config]( int const col )
		{
			return config.column_names[col] + ' ' + strings[col][objective];
		};

		return "["s + config.quest_names[objective] + "] "
			+ time_str( pops ) + " | "
			+ time_str( takes ) + " | "
			+ time_str( dones ) + " | "
			+ time_str( durations );
	}

	auto chat_str(
		int const objective,
		int const column,
		TimeStrings const& strings,
		Config const& config )
	{
		assert( objective >= chamber && objective <= dhuum );
		assert( column >= pops && column <= durations );

		auto const quest_name =	config.quest_names[objective];
		auto const column_name = config.column_names[column];
		auto const time_str = strings[column][objective];

		return "["s + quest_name + '|' + column_name + "] " + time_str;
	}

	auto chat_message(
		int const objective,
		int const column,
		TimeStrings const& strings,
		Config const& config )
	{
		assert( objective >= chamber && objective <= dhuum );
		assert( column >= name && column <= durations );

		if ( column == name )
			return full_info( objective, strings, config );

		return chat_str( objective, column, strings, config );
	}

	auto dhuum_message(
		int const column, TimeStrings const& strings, Config const& config )
	{
		assert( column >= name && column <= durations );

		if ( column == name )
		{
			return "["s + config.quest_names[dhuum] + "] "
				+ "10/10 " + strings[pops][dhuum] + " | "
				+ "Hostile " + strings[takes][dhuum] + " | "
				+ "Done " + strings[dones][dhuum] + " | "
				+ "Time " + strings[durations][dhuum];
		}
		else if ( column == pops )
		{
			return "[10/10] " + strings[pops][dhuum];
		}
		else if ( column == takes )
		{
			return "["s + config.quest_names[dhuum] + "|Hostile] "
				+ strings[column][dhuum];
		}
		else return chat_str( dhuum, column, strings, config );
	}

}


auto April::Gui::UwTimesGui::Display(
	UwTimer const& timer, Config const& config ) const -> Command
{
	auto command = NoCommand;

	auto const times = timer.current();
	auto const strings = uwtimes_to_strings( times );
	auto instance_time_str =
		to_string( times.instance_time, instance_time_fmt );

	if ( ImGui::Begin( config.window ) )
	{
		if ( ImGui::IsWindowHovered() )
		{
			WndProc::BlockMouseInput();
		}
		ImGui::PushStyleColor( ImGuiCol_Button, Invisible() );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, White( 0.5f ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, White( 0.2f ) );
		ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, { 0, 0 } );
		ImGui::Columns( 5 );

		// Current Time
		if ( ImGui::SmallButton( instance_time_str ) )
		{
			command = SendChat{
				'#', "[/age] " + std::move( instance_time_str )
			};
		}
		ImGui::NextColumn();

		// Headers
		for ( auto it = pops; it <= durations; ++it )
		{
			ImGui::Text( config.column_names[it] );
			ImGui::NextColumn();
		}
		ImGui::Separator();

		// Quests
		for ( auto objective = chamber; objective <= pools; ++objective )
		{
			auto const clicked = draw_objective( objective, strings, config );
			if ( clicked == std::nullopt )
				continue;

			command = SendChat{
				'#', chat_message( objective, *clicked, strings, config )
			};
		}
		ImGui::Separator();
		if ( auto const clicked = draw_objective( dhuum, strings, config );
			clicked )
		{
			command = SendChat{
				'#', dhuum_message( *clicked, strings, config )
			};
		}

		ImGui::Columns( 1 );
		ImGui::PopStyleVar();
		ImGui::PopStyleColor( 3 );
	}
	ImGui::End();

	return command;
}

auto April::Gui::UwTimesGui::Config::LoadDefault() -> Config
{
	return Config{
		std::array<std::string, 11>{
			"Chamber", "Restore", "Escort", "UWG", "Vale",
			"Wastes", "Pits", "Plains", "Mnts", "Pools",
			"Dhuum"
		},
		std::array<std::string, 4>{
			"Pop", "Take", "Done", "Time"
		},

		"Underworld Times",
		ImGuiWindowFlags_None
	};
}
