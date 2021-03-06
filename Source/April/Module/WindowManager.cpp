
#include "April/Module/WindowManager.h"

#include "April/Framework/Modules.h"

#include "Dependencies/GWCA.hpp"

using namespace April::Gui;
using namespace GW::Packet::StoC;
using namespace GW::Constants::ModelID::UW;


void April::WindowMgr::Update( ModuleConfigurations& configs )
{
	auto const* player = GW::Agents::GetCharacter();
	if ( player == nullptr )
		return;

	auto const distance = GW::GetDistance( player->pos, { 11427, 5079 } );

	auto& window = std::get<ChainedSoulGui::Config>( configs.gui ).window;
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
	ObjectiveDone const& packet, ModuleConfigurations& configs ) const
{
	if ( packet.objective_id == GW::Objectives::Dhuum )
	{
		std::get<DhuumBotGui::Config>( configs.gui ).window.visible = false;
		std::get<DhuumInfo::Config>( configs.gui ).window.visible = false;
	}
}

void April::WindowMgr::Update(
	AgentUpdateAllegiance const& packet, ModuleConfigurations& configs ) const
{
	auto const* agent = GW::Agents::GetAgentByID( packet.agent_id );
	if ( agent == nullptr ) return;

	auto const* living = agent->GetAsAgentLiving();
	if ( living == nullptr ) return;
	if ( living->player_number != Dhuum ) return;
	if ( living->hp < 1 ) return; // TODO: find content for turning hostile

	std::get<DhuumInfo::Config>( configs.gui ).window.visible = true;
	std::get<DhuumBotGui::Config>( configs.gui ).window.visible = true;
}

void April::WindowMgr::Update(
	MapLoaded const&, ModuleConfigurations& configs ) const
{
	if ( GW::GetMapID() == GW::MapID::The_Underworld
		|| GW::GetMapID() == GW::MapID::Embark_Beach )
	{
		std::get<UwTimesGui::Config>( configs.gui ).window.visible = true;
	}
	else std::get<UwTimesGui::Config>( configs.gui ).window.visible = false;

	std::get<DhuumBotGui::Config>( configs.gui ).window.visible = false;
	std::get<DhuumInfo::Config>( configs.gui ).window.visible = false;
	std::get<ChainedSoulGui::Config>( configs.gui ).window.visible = false;
}
