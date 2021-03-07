
#include "April/Module/WindowManager.h"

#include "April/Framework/Instance.h"

#include "Dependencies/GWCA.hpp"

using namespace April::Gui;
using namespace GW::Packet::StoC;
using namespace GW::Constants::ModelID::UW;


void April::WindowMgr::Update( Instance& instance )
{
	auto const* player = GW::Agents::GetCharacter();
	if ( player == nullptr )
		return;

	auto const distance = GW::GetDistance( player->pos, { 11427, 5079 } );

	auto& window = std::get<ChainedSoulGui::Config>( instance.config ).window;
	if ( not in_pits && distance < GW::Constants::Range::Compass )
	{
		in_pits = true;
		window.visible = true;
	}
	else if ( in_pits && distance > GW::Constants::Range::Compass )
	{
		in_pits = false;
		window.visible = false;
	}
}

void April::WindowMgr::Update(
	ObjectiveDone const& packet, Instance& instance ) const
{
	if ( packet.objective_id == GW::Objectives::Dhuum )
	{
		std::get<DhuumBotGui::Config>( instance.config ).window.visible = false;
		std::get<DhuumInfo::Config>( instance.config ).window.visible = false;
	}
}

void April::WindowMgr::Update(
	AgentUpdateAllegiance const& packet, Instance& instance ) const
{
	auto const* agent = GW::Agents::GetAgentByID( packet.agent_id );
	if ( agent == nullptr ) return;

	auto const* living = agent->GetAsAgentLiving();
	if ( living == nullptr ) return;
	if ( living->player_number != Dhuum ) return;
	if ( living->hp < 1 ) return; // TODO: find content for turning hostile

	std::get<DhuumInfo::Config>( instance.config ).window.visible = true;
	std::get<DhuumBotGui::Config>( instance.config ).window.visible = true;
}

void April::WindowMgr::Update(
	MapLoaded const&, Instance& instance ) const
{
	if ( GW::GetMapID() == GW::MapID::The_Underworld
		|| GW::GetMapID() == GW::MapID::Embark_Beach )
	{
		std::get<UwTimesGui::Config>( instance.config ).window.visible = true;
	}
	else std::get<UwTimesGui::Config>( instance.config ).window.visible = false;

	std::get<DhuumBotGui::Config>( instance.config ).window.visible = false;
	std::get<DhuumInfo::Config>( instance.config ).window.visible = false;
	std::get<ChainedSoulGui::Config>( instance.config ).window.visible = false;
}
