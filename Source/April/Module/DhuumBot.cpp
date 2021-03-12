
#include "April/Module/DhuumBot.h"

#include "Dependencies/GWCA.hpp"

using namespace std::chrono;


namespace {

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

	auto rest() -> April::Command
	{
		auto const* skillbar = GW::SkillbarMgr::GetPlayerSkillbar();
		if ( skillbar == nullptr || not skillbar->IsValid() )
			return April::NoCommand;

		auto const rest = skillbar->skills[0];
		auto const id = static_cast<GW::SkillID>( rest.skill_id );

		if ( rest.GetRecharge() != 0 || id != GW::SkillID::Dhuums_Rest )
			return April::NoCommand;

		return April::UseSkill{ 0 };
	}

	auto fury( GW::AgentID& dhuum_id ) -> April::Command
	{
		auto const* dhuum = find_dhuum( dhuum_id );
		if ( dhuum == nullptr || dhuum->allegiance != hostile )
			return April::NoCommand;

		auto const* skillbar = GW::SkillbarMgr::GetPlayerSkillbar();
		if ( skillbar == nullptr || not skillbar->IsValid() )
			return April::NoCommand;

		auto const fury = skillbar->skills[4];
		auto const id = static_cast<GW::SkillID>( fury.skill_id );

		if ( fury.GetRecharge() != 0 || id != GW::SkillID::Ghostly_Fury )
			return April::NoCommand;

		return April::UseSkill{ 4, dhuum };
	}

}


auto April::DhuumBot::Update() -> Command
{
	if ( not active ) return NoCommand;
	if ( steady_clock::now() - last_cast < 1s ) return NoCommand;

	auto const* player = GW::Agents::GetCharacter();
	if ( not player_is_valid( player ) || player->GetIsDead() )
		return NoCommand;

	if ( GW::GetEnergyPoints( *player ) <= 7 )
		return NoCommand;

	if ( not at_dhuum( *player ) )
	{
		active = false;
		return NoCommand;
	}

	if ( player->GetIsCasting() || player->skill != 0 )
		return NoCommand;

	auto const command =
		GW::GetMissionProgress() < 1.f ? rest() : fury( dhuum_id );

	if ( not std::holds_alternative<NoCommand_t>( command ) )
	{
		last_cast = steady_clock::now();
	}

	return command;
}

void April::DhuumBot::Update( GW::Packet::StoC::ObjectiveDone const& packet )
{
	if ( packet.objective_id == GW::Objectives::Dhuum )
	{
		active = false;
		dhuum_id = 0;
	}
}
