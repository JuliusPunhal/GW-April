
#include "April/Module/WindowManager.h"

#include "Dependencies/GWCA.hpp"

using namespace April::Gui;
using namespace GW::Packet::StoC;
using namespace GW::Constants::ModelID::UW;


void April::WindowMgr::Update( ChainedSoulGui::Config& chained_soul )
{
	auto const* player = GW::Agents::GetCharacter();
	if ( player == nullptr )
		return;

	auto const distance = GW::GetDistance( player->pos, { 11427, 5079 } );

	if ( not in_pits && distance < GW::Constants::Range::Compass )
	{
		in_pits = true;
		chained_soul.window.visible = true;
	}
	else if ( in_pits && distance > GW::Constants::Range::Compass )
	{
		in_pits = false;
		chained_soul.window.visible = false;
	}
}

void April::WindowMgr::Update(
	ObjectiveDone const& packet,
	DhuumBotGui::Config& dhuum_bot,
	DhuumInfo::Config& dhuum_info ) const
{
	if ( packet.objective_id == GW::Objectives::Dhuum )
	{
		dhuum_bot.window.visible = false;
		dhuum_info.window.visible = false;
	}
}

void April::WindowMgr::Update(
	AgentUpdateAllegiance const& packet,
	DhuumBotGui::Config& dhuum_bot,
	DhuumInfo::Config& dhuum_info ) const
{
	auto const* agent = GW::Agents::GetAgentByID( packet.agent_id );
	if ( agent == nullptr ) return;

	auto const* living = agent->GetAsAgentLiving();
	if ( living == nullptr ) return;
	if ( living->player_number != Dhuum ) return;
	if ( living->hp < 1 ) return; // TODO: find content for turning hostile

	dhuum_bot.window.visible = true;
	dhuum_info.window.visible = true;
}

void April::WindowMgr::Update(
	MapLoaded const&,
	ChainedSoulGui::Config& chained_soul,
	DhuumBotGui::Config& dhuum_bot,
	DhuumInfo::Config& dhuum_info,
	UwTimesGui::Config& uw_times ) const
{
	if ( GW::GetMapID() == GW::MapID::The_Underworld
		|| GW::GetMapID() == GW::MapID::Embark_Beach )
	{
		uw_times.window.visible = true;
	}
	else uw_times.window.visible = false;

	dhuum_bot.window.visible = false;
	dhuum_info.window.visible = false;
	chained_soul.window.visible = false;
}
