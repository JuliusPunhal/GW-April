
#include "April/Module/WindowManager.h"

#include "Dependencies/GWCA.hpp"

using namespace GW::Packet::StoC;
using namespace GW::Constants::ModelID::UW;


auto April::WindowMgr::Update() -> Command
{
	auto const* player = GW::Agents::GetCharacter();
	if ( player == nullptr )
		return NoCommand;

	auto const distance = GW::GetDistance( player->pos, { 11427, 5079 } );

	if ( not in_pits && distance < GW::Constants::Range::Compass )
	{
		in_pits = true;
		return SetUwGuiVisibility{ SetUwGuiVisibility::Type::Pits, true };
	}
	else if ( in_pits && distance > GW::Constants::Range::Compass )
	{
		in_pits = false;
		return SetUwGuiVisibility{ SetUwGuiVisibility::Type::Pits, false };
	}

	return NoCommand;
}

auto April::WindowMgr::Update( ObjectiveDone const& packet ) const
	-> Command
{
	if ( packet.objective_id == GW::Objectives::Dhuum )
	{
		return SetUwGuiVisibility{ SetUwGuiVisibility::Type::Dhuum, false };
	}
	else return NoCommand;
}

auto April::WindowMgr::Update( AgentUpdateAllegiance const& packet ) const
	-> Command
{
	auto const* agent = GW::Agents::GetAgentByID( packet.agent_id );
	if ( agent == nullptr ) return NoCommand;

	auto const* living = agent->GetAsAgentLiving();
	if ( living == nullptr
		|| living->player_number != Dhuum
		|| living->hp < 1 ) // TODO: find content for turning hostile
	{
		return NoCommand;
	}

	return SetUwGuiVisibility{ SetUwGuiVisibility::Type::Dhuum, true };
}

auto April::WindowMgr::Update( MapLoaded const& ) const
	-> std::vector<Command>
{
	auto commands = std::vector<Command>{
		SetUwGuiVisibility{ SetUwGuiVisibility::Type::Dhuum, false },
		SetUwGuiVisibility{ SetUwGuiVisibility::Type::Dhuum, false }
	};

	if ( GW::GetMapID() == GW::MapID::The_Underworld
		|| GW::GetMapID() == GW::MapID::Embark_Beach )
	{
		commands.push_back(
			SetUwGuiVisibility{ SetUwGuiVisibility::Type::General, true } );
	}
	else
	{
		commands.push_back(
			SetUwGuiVisibility{ SetUwGuiVisibility::Type::General, false } );
	}

	return commands;
}
