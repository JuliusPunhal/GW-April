
#include "April/Module/DhuumBot.h"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"

using namespace std::chrono;


namespace {

	auto entry = GW::HookEntry{};

	inline constexpr auto hostile = 0x3; // GW::Agent::allegiance


	bool at_dhuum( GW::AgentLiving const& player )
	{
		if ( GW::GetInstanceType() != GW::InstanceType::Explorable ) 
			return false;

		if ( GW::GetMapID() != GW::MapID::The_Underworld )
			return false;

		if ( !GW::Effects::GetPlayerEffectById( GW::SkillID::Curse_of_Dhuum ) )
			return false;

		auto const distance = GW::GetDistance( player.pos, { -15750, 17250 } );
		if ( distance > GW::Constants::Range::Compass )
			return false;

		return true;
	}

	bool player_is_valid( GW::Agent const* player )
	{
		auto const* spectated = GW::Agents::GetPlayer();

		return player && spectated && player->agent_id == spectated->agent_id;
	}

	auto is_dhuum( GW::Agent const* agent ) -> GW::AgentLiving const*
	{
		using namespace GW::Constants::ModelID::UW;

		if ( agent == nullptr ) return nullptr;
		
		auto const* living = agent->GetAsAgentLiving();
		return living && living->player_number == Dhuum 
			? living : nullptr;
	}

	auto find_cached_dhuum( GW::AgentID const id ) -> GW::AgentLiving const*
	{
		auto const agent = GW::Agents::GetAgentByID( id );
		return is_dhuum( agent );
	}

	auto find_dhuum( GW::AgentID& cached_id ) -> GW::AgentLiving const*
	{
		auto const* cache = find_cached_dhuum( cached_id );
		if ( cache ) return cache;

		for ( auto const* agent : GW::Agents::GetAgentArray() )
		{
			auto const* dhuum = is_dhuum( agent );
			if ( not dhuum ) continue;
			
			cached_id = dhuum->agent_id;
			return dhuum;
		}
		return nullptr;
	}

	bool used_rest()
	{
		auto const* skillbar = GW::SkillbarMgr::GetPlayerSkillbar();
		if ( skillbar == nullptr || not skillbar->IsValid() )
			return false;

		auto const rest = skillbar->skills[0];
		auto const id = static_cast<GW::SkillID>( rest.skill_id );

		if ( rest.GetRecharge() != 0 || id != GW::SkillID::Dhuums_Rest )
			return false;
		
		GW::UseSkill( 0 );
		return true;
	}

	bool used_fury( GW::AgentID& dhuum_id )
	{
		auto const* dhuum = find_dhuum( dhuum_id );
		if ( dhuum == nullptr || dhuum->allegiance != hostile )
			return false;
	
		auto const* skillbar = GW::SkillbarMgr::GetPlayerSkillbar();
		if ( skillbar == nullptr || not skillbar->IsValid() )
			return false;

		auto const fury = skillbar->skills[4];
		auto const id = static_cast<GW::SkillID>( fury.skill_id );

		if ( fury.GetRecharge() != 0 || id != GW::SkillID::Ghostly_Fury )
			return false;

		GW::UseSkill( 4, *dhuum );
		return true;
	}

	bool used_spirit_skill( GW::AgentID& dhuum_id )
	{
		return GW::GetMissionProgress() < 1.f 
			? used_rest() : used_fury( dhuum_id );
	}

}


April::DhuumBot::DhuumBot()
{
	// Callback will only be cleaned up during GWCA shutdown.
	GW::StoC::RegisterPacketCallback<GW::Packet::StoC::ObjectiveDone>(
		&entry,
		[this]( auto*, auto* packet )
		{
			if ( packet->objective_id == GW::Objectives::Dhuum )
			{
				active = false;
				dhuum_id = 0;
			}
		} );
}

void April::DhuumBot::Update()
{
	if ( not active ) return;
	if ( steady_clock::now() - last_cast < 1s ) return;

	auto const* player = GW::Agents::GetCharacter();
	if ( not player_is_valid( player ) || player->GetIsDead() ) 
		return;

	if ( GW::GetEnergyPoints( *player ) <= 7 )
		return;

	if ( not at_dhuum( *player ) )
	{
		active = false;
		return;
	}

	if ( player->GetIsCasting() || player->skill != 0 )
		return;

	if ( used_spirit_skill( dhuum_id ) )
		last_cast = steady_clock::now();
}
