
#include "April/Utility/ReducedSkillRecharge.h"

#include <cfenv>
#include <cmath>
#include <optional>

using namespace GW::Packet::StoC;


namespace {

	constexpr auto FC = GW::AttributeID::FastCasting;
	constexpr auto BU = GW::SkillID::Essence_of_Celerity_item_effect;


	auto fc_reduction( GW::Skill const& data )
	{
		auto const prof = static_cast<GW::Profession>( data.profession );
		if ( prof != GW::Profession::Mesmer )
			return 1.f;

		auto const type = static_cast<GW::SkillType>( data.type );
		if (
			type != GW::SkillType::Spell
			&& type != GW::SkillType::Hex
			&& type != GW::SkillType::Enchantment )
		{
			return 1.f;
		}

		auto const fc_attr = GW::GetPlayerAttribute( FC );
		return fc_attr ? 1 - fc_attr->level * 0.03f : 1.f;
	}

	auto bu_reduction()
	{
		return GW::GetPlayerEffect( BU ) ? 0.8f : 1.f;
	}

	auto expected_recharge_sec( GW::SkillID const skill_id )
	{
		auto const& data = GW::GetSkillConstantData( skill_id );

		auto rounding_mode = std::fegetround();
		std::fesetround( FE_TONEAREST );

		auto const cd_sec =
			std::lrintf(
				data.recharge * fc_reduction( data ) * bu_reduction() );

		std::fesetround( rounding_mode );

		return static_cast<decltype( SkillRecharge::recharge )>( cd_sec );
	}

	template<typename Packet_t>
	auto get_skill_slot( Packet_t const& packet ) -> std::optional<int>
	{
		auto const skillbar = GW::GetPlayerSkillbar();
		if ( skillbar == nullptr )
			return std::nullopt;

		for ( auto it = 0; it < 8; ++it )
		{
			if (
				auto const& skill = skillbar->skills[it];
				skill.skill_id == packet.skill_id
				&& skill.event == packet.skill_instance )
			{
				return it;
			}
		}

		return std::nullopt;
	}

}


void April::ReducedSkillRecharge::Update( MapLoaded const& )
{
	skills = {};
}

void April::ReducedSkillRecharge::Update( SkillRecharge const& packet )
{
	auto const player = GW::GetCharacter();
	if ( player == nullptr || player->agent_id != packet.agent_id )
		return;

	auto const slot = get_skill_slot( packet );
	if ( slot == std::nullopt )
		return;

	auto const skill_id = static_cast<GW::SkillID>( packet.skill_id );
	if ( packet.recharge == 0 ) // pstone
	{
		skills[*slot] = false;
	}
	else if ( packet.recharge < expected_recharge_sec( skill_id ) )
	{
		skills[*slot] = true;
	}
}

void April::ReducedSkillRecharge::Update( SkillRecharged const& packet )
{
	auto const player = GW::GetCharacter();
	if ( player == nullptr || player->agent_id != packet.agent_id )
		return;

	auto const slot = get_skill_slot( packet );
	if ( slot == std::nullopt )
		return;

	skills[*slot] = false;
}
