
#include "April/Gui/ChainedSoulInfo.h"

#include "April/Framework/WndProc.h"
#include "April/Utility/TimeFormatting.h"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"

using April::to_string;
using April::MMSS;
using namespace std::chrono;
using namespace std::string_literals;


namespace {

	auto time_until_death( float const hp, float const pips )
	{
		return seconds{ static_cast<int>(hp / pips * -1.f) };
	}

	auto time_until_respawn( milliseconds const death )
	{
		auto const time_since_death = GW::GetInstanceTime() - death;
		return 2min - time_since_death;
	}

	auto string_until_respawn( milliseconds const time_of_death )
	{
		auto const respawn_in = time_until_respawn( time_of_death );

		if ( respawn_in >= 0ms )
			return "Time until respawn: " + to_string( respawn_in, MMSS );
		else
			return "Time until respawn: unknown"s;
	}


	auto parse( April::ChainedSoul::AliveSoul const& alive_soul )
	{
		auto const ms = time_until_death( alive_soul.hp, alive_soul.pips );
		return "Time until death: " + to_string( ms, MMSS );
	}

	auto parse( April::ChainedSoul::DeadSoul const& dead_soul )
	{
		if ( dead_soul.time_of_death.has_value() )
			return string_until_respawn( *dead_soul.time_of_death );
		else
			return "Time until respawn: unknown"s;
	}

	auto parse( April::ChainedSoul::OutOfRange const& invisible_soul )
	{
		if ( invisible_soul.time_of_death.has_value() )
			return string_until_respawn( *invisible_soul.time_of_death );
		else
			return "Soul not found!"s;
	}

	auto parse_chained_soul_state( April::ChainedSoul::SoulState const& soul )
		-> std::string
	{
		auto const parser = []( auto&& soul ) { return parse( soul ); };
		return std::visit( parser, soul );
	}

}


April::Gui::ChainedSoulGui::ChainedSoulGui( Config const& config )
	: config{ config }
{
}

void April::Gui::ChainedSoulGui::Display( ChainedSoul const& state ) const
{
	auto const label = parse_chained_soul_state( state.get() );

	if ( ImGui::Begin( config.window ) )
	{
		if ( ImGui::IsWindowHovered() )
		{
			WndProc::BlockMouseInput();
		}

		if ( ImGui::Button( label ) )
		{
			GW::SendChat( '#', label );
		}
	}
	ImGui::End();
}

auto April::Gui::ChainedSoulGui::Config::LoadDefault() -> Config
{
	return Config{
		{ "Chained Soul Info", false, ImGuiWindowFlags_None }
	};
}
