
#include "April/Gui/UwTimer.h"

#include "April/Utility/TimeFormatting.h"

#include "ImGui/ImGui.hpp"

#include <array>
#include <string>

using namespace April;
using namespace GW::literals;
using namespace GW::Packet::StoC;
using namespace std::string_literals;


namespace {

	constexpr auto TABLE_FLAGS =
		ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable;

	constexpr auto OBJECTIVE_NAMES = std::array<const char*, 11>{
		"Chamber", "Restore", "Escort", "UWG", "Vale",
		"Wastes", "Pits", "Planes", "Mnts", "Pools",
		"Dhuum"
	};

	constexpr auto CHAMBER = 0;
	constexpr auto RESTORE = 1;
	constexpr auto UWG = 3;
	constexpr auto POOLS = 9;
	constexpr auto DHUUM = 10;

	constexpr auto LAB = 0;


	auto to_mmss( GW::InstanceTime const time ) -> std::string
	{
		return time == 0_ms ? "--:--" : to_string_mmss( time );
	}

	auto get_duration(
		GW::InstanceTime const now,
		GW::InstanceTime const take,
		GW::InstanceTime const done )
	{
		if ( take == 0_ms ) return 0_ms;
		if ( done == 0_ms ) return now - take;
		return done - take;
	}

	auto chat_msg(
		const char* which,
		const char* name,
		std::string const& time_str,
		std::string const* date_time )
	{
		auto const result = "["s + name + "|" + which + "] " + time_str;
		if ( date_time == nullptr )
			return result;

		return *date_time + result;
	}

	void impl_draw_row(
		const char* name,
		const char* id,
		GW::InstanceTime const time,
		std::string const* date_time = nullptr )
	{
		ImGui::TableNextColumn();
		auto const str = to_mmss( time );
		if ( ImGui::SmallButton( str + "###" + id ) )
			GW::SendChat( '#', chat_msg( id, name, str, date_time ) );
	}

	void draw_row(
		const char* name,
		GW::InstanceTime const now,
		GW::InstanceTime const pop,
		GW::InstanceTime const take,
		GW::InstanceTime const done,
		std::string const* date_time = nullptr )
	{
		ImGui::PushID( name );
		ImGui::PushStyleColor( ImGuiCol_Button, Invisible() );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, Grey( 1 ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, Grey( .5f ) );

		ImGui::TableNextColumn();
		ImGui::Text( name );

		impl_draw_row( name, "Pop", pop, date_time );
		impl_draw_row( name, "Take", take, date_time );
		impl_draw_row( name, "Done", done, date_time );

		auto const duration = get_duration( now, take, done );
		impl_draw_row( name, "Time", duration, date_time );

		ImGui::PopStyleColor( 3 );
		ImGui::PopID();
	}

	void draw_dhuum_row(
		const char* name,
		GW::InstanceTime const now,
		GW::InstanceTime const pop,
		GW::InstanceTime const take,
		GW::InstanceTime const done,
		std::string const* date_time = nullptr )
	{
		ImGui::PushID( name );
		ImGui::PushStyleColor( ImGuiCol_Button, Invisible() );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, Grey( 1 ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, Grey( .5f ) );

		ImGui::TableNextColumn();
		ImGui::Text( name );

		impl_draw_row( name, "10/10", pop, date_time );
		impl_draw_row( name, "Dhuum Start", take, date_time );
		impl_draw_row( name, "Dhuum Done", done, date_time );

		auto const duration = get_duration( now, take, done );
		impl_draw_row( name, "Dhuum Time", duration, date_time );

		ImGui::PopStyleColor( 3 );
		ImGui::PopID();
	}

	void draw_run(
		UwTimes const& times, std::string const* date_time = nullptr )
	{
		using namespace April;

		if ( ImGui::BeginTable( "Objective Times", 5, TABLE_FLAGS ) )
		{
			auto const instance_time =
				to_string_hmmss( times.instance_time ) + "###InstanceTime";

			ImGui::TableSetupColumn( instance_time.c_str() );
			ImGui::TableSetupColumn( "Pop" );
			ImGui::TableSetupColumn( "Take" );
			ImGui::TableSetupColumn( "Done" );
			ImGui::TableSetupColumn( "Time" );
			ImGui::TableHeadersRow();

			// Chamber
			draw_row(
				OBJECTIVE_NAMES[CHAMBER],
				times.instance_time,
				0_ms,
				times.take[CHAMBER],
				times.done[CHAMBER],
				date_time );

			// Restore - Pools
			for ( auto row = RESTORE; row <= POOLS; ++row )
			{
				draw_row(
					OBJECTIVE_NAMES[row],
					times.instance_time,
					times.pop[row <= UWG ? LAB : row - 3],
					times.take[row],
					times.done[row],
					date_time );
			}

			// Dhuum
			draw_dhuum_row(
				OBJECTIVE_NAMES[DHUUM],
				times.instance_time,
				times.nightman_cometh,
				times.dhuum_hostile,
				times.dhuum_done,
				date_time );

			ImGui::EndTable();
		}
	}

	void draw_runs( UwTimesHistory const& history )
	{
		if ( history.size() == 0 )
		{
			ImGui::Text( "No Underworld-Times have been recorded." );
			return;
		}

		draw_run( history.back() );

		if ( history.size() > 1 )
		{
			for ( auto it = history.rbegin() + 1; it != history.rend(); ++it )
			{
				auto const date_time = to_date_str( it->date_time );
				if ( ImGui::CollapsingHeader( date_time ) )
					draw_run( *it, &date_time );
			}
		}
	}

}


April::Gui::UwTimer::UwTimer(
	std::shared_ptr<Config>               config,
	std::shared_ptr<Mouse>                mouse,
	std::shared_ptr<UwTimesHistory const> history )
	: config{ config }, mouse{ mouse }, history{ history }
{
}

void April::Gui::UwTimer::Display()
{
	ImGui::SetNextWindowPosCentered( ImGuiCond_FirstUseEver );
	ImGui::SetNextWindowSize( { 310, 260 }, ImGuiCond_FirstUseEver );

	if ( ImGui::Begin( config->window, *mouse ) )
	{
		draw_runs( *history );
	}
	ImGui::End();
}

void April::Gui::UwTimer::Update( MapLoaded const& )
{
	if ( not config->auto_set_visibility )
		return;

	auto const map_id = GW::GetMapID();
	if ( map_id == GW::MapID::The_Underworld )
	{
		config->window.visible = true;
	}
	else if ( map_id != GW::MapID::Embark_Beach )
	{
		config->window.visible = false;
	}
}
