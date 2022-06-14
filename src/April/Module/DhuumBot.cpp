
#include "April/Module/DhuumBot.h"

#include <optional>

using namespace GW::Constants::Allegiance;
using namespace GW::Constants::ModelID::UW;
using namespace GW::literals;
using namespace GW::Packet::StoC;
using namespace std::chrono;


namespace {

	auto const hall_of_judgement = GW::Vec2f{ -15750.f, 17250.f };


	bool at_dhuum( GW::AgentLiving const& player )
	{
		if ( GW::GetInstanceType() != GW::InstanceType::Explorable )
			return false;

		if ( GW::GetMapID() != GW::MapID::The_Underworld )
			return false;

		auto const* found = GW::GetPlayerEffect( GW::SkillID::Curse_of_Dhuum );
		if ( not found )
			return false;

		auto const distance = GW::GetDistance( player.pos, hall_of_judgement );
		if ( distance > GW::Constants::Range::Compass )
			return false;

		return true;
	}

	auto find_skill(
		GW::Skillbar const& skillbar, GW::SkillID const id, int const hint )
		-> GW::SkillbarSkill const*
	{
		if ( GW::GetSkillID( skillbar.skills[hint] ) == id )
			return &skillbar.skills[hint];

		for ( auto const& skill : skillbar.skills )
		{
			if ( GW::GetSkillID( skill ) == id )
				return &skill;
		}

		return nullptr;
	}

	auto used_skill(
		GW::SkillID const id, int const slot_hint, GW::AgentID const target )
		-> std::optional<bool>
	{
		auto const* skillbar = GW::GetPlayerSkillbar();
		if ( skillbar == nullptr || not skillbar->IsValid() )
			return false;

		auto const* skill = find_skill( *skillbar, id, slot_hint );
		if ( skill == nullptr )
			return std::nullopt;

		if ( GW::GetRecharge( *skill ) != 0_ms )
			return false;

		auto const slot = skill - &skillbar->skills[0];
		GW::UseSkill( slot, target );
		return true;
	}

	auto used_rest()
	{
		return used_skill( GW::SkillID::Dhuums_Rest, 0, 0 );
	}

	auto find_dhuum() -> GW::AgentLiving const*
	{
		for ( auto const* agent : GW::GetAgentArray() )
		{
			auto const* dhuum = GW::GetAsAgentLiving( agent );
			if ( dhuum && dhuum->player_number == Dhuum )
				return dhuum;
		}
		return nullptr;
	}

	auto used_fury() -> std::optional<bool>
	{
		auto const* dhuum = find_dhuum();
		if ( dhuum == nullptr || dhuum->allegiance != Hostile )
			return false;

		return used_skill( GW::SkillID::Ghostly_Fury, 4, dhuum->agent_id );
	}

}


void April::Module::DhuumBot::Update()
{
	if ( not active )
		return;

	if ( steady_clock::now() - last_cast < 1s )
		return;

	auto const* player = GW::GetCharacter();
	if (
		player == nullptr || player->GetIsDead()
		|| player->GetIsCasting() || player->skill != 0
		|| GW::GetEnergyPoints( *player ) <= 7 )
	{
		return;
	}

	if ( not at_dhuum( *player ) )
	{
		active = false;
		return;
	}

	auto const used_skill_result =
		GW::GetMissionProgress() < 1 ? used_rest() : used_fury();

	if ( used_skill_result == std::nullopt )
	{
		active = false;
		return;
	}

	if ( *used_skill_result == true )
		last_cast = steady_clock::now();
}

void April::Module::DhuumBot::Update( ObjectiveDone const& packet )
{
	if ( packet.objective_id == GW::Constants::ObjectiveID::Dhuum )
		active = false;
}
