
#include "April/Module/WindowManager.h"

#include "April/Framework/Modules.h"

#include "Dependencies/GWCA.hpp"

using namespace April;
using namespace GW::Packet::StoC;
using namespace GW::Constants::ModelID::UW;

using Guis = ModuleConfigurations::Gui;


namespace {

	auto entry = GW::HookEntry{};


	void on_load( Guis& guis )
	{
		if ( GW::GetMapID() == GW::MapID::The_Underworld
			|| GW::GetMapID() == GW::MapID::Embark_Beach )
		{
			std::get<Gui::UwTimesGui::Config>( guis ).window.visible = true;
		}
		else std::get<Gui::UwTimesGui::Config>( guis ).window.visible = false;

		std::get<Gui::DhuumBotGui::Config>( guis ).window.visible = false;
		std::get<Gui::DhuumInfo::Config>( guis ).window.visible = false;
		std::get<Gui::ChainedSoulGui::Config>( guis ).window.visible = false;
	}

	void on_curse_of_dhuum( Guis& guis, AddExternalBond const& packet )
	{
		auto const skill_id = static_cast<GW::SkillID>( packet.skill_id );
		if ( skill_id == GW::SkillID::Curse_of_Dhuum )
		{
			std::get<Gui::DhuumBotGui::Config>( guis ).window.visible = true;
		}
	}

	void on_dhuum_hostile( Guis& guis, AgentUpdateAllegiance const& packet )
	{
		auto const* agent = GW::Agents::GetAgentByID( packet.agent_id );
		if ( agent == nullptr ) return;

		auto const* living = agent->GetAsAgentLiving();
		if ( living == nullptr ) return;
		if ( living->player_number != Dhuum ) return;
		if ( living->hp < 1 ) return; // TODO: find content for turning hostile

		std::get<Gui::DhuumInfo::Config>( guis ).window.visible = true;
	}

	void on_objective( Guis& guis, ObjectiveDone const& packet )
	{
		if ( packet.objective_id == GW::Objectives::Dhuum )
		{
			std::get<Gui::DhuumBotGui::Config>( guis ).window.visible = false;
			std::get<Gui::DhuumInfo::Config>( guis ).window.visible = false;
		}
	}

}


April::WindowMgr::WindowMgr( ModuleConfigurations& configs )
	: configs{ configs }
{
	// Callbacks will only be cleaned up during GWCA shutdown.
	GW::StoC::RegisterPacketCallback<MapLoaded>(
		&entry, [this]( auto*, auto* ) { on_load( this->configs.gui ); } );
	GW::StoC::RegisterPacketCallback<AddExternalBond>(
		&entry,
		[this]( auto*, auto* packet )
		{
			on_curse_of_dhuum( this->configs.gui, *packet );
		} );
	GW::StoC::RegisterPacketCallback<AgentUpdateAllegiance>(
		&entry,
		[this]( auto*, auto* packet )
		{
			on_dhuum_hostile( this->configs.gui, *packet );
		} );
	GW::StoC::RegisterPacketCallback<ObjectiveDone>(
		&entry,
		[this]( auto*, auto* packet )
		{
			on_objective( this->configs.gui, *packet );
		} );
}

void April::WindowMgr::Update()
{
	auto const* player = GW::Agents::GetCharacter();
	if ( player == nullptr )
		return;

	auto const distance = GW::GetDistance( player->pos, { 11427, 5079 } );

	auto& window = std::get<Gui::ChainedSoulGui::Config>( configs.gui ).window;
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
