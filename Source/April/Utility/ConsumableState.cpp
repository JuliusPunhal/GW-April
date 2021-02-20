
#include "April/Utility/ConsumableState.h"

#include "Dependencies/GWCA.hpp"

#include <chrono>

using namespace std::chrono;


namespace {

	bool is_active( GW::SkillID const id )
	{
		auto const* effect = GW::Effects::GetPlayerEffectById( id );
		if ( effect == nullptr )
			return false;

		auto const uptime = GW::GetTimeRemaining( *effect );

		return uptime > 1s || uptime < -1s; // account for permanent effects
	}

	bool is_disguised()
	{
		return is_active( GW::SkillID::Disguised_verification_requested )
			|| is_active( GW::SkillID::Disguised_when_using_Everlasting_Legionnaire_Tonic )
			|| is_active( GW::SkillID::Disguised_when_using_everlasting_tonics_except_Everlasting_Legionnaire_Tonic )
			|| is_active( GW::SkillID::Disguised_when_using_non_everlasting_tonics );
	}

}


auto April::CState<April::Trivial>::get_state( Trivial const& consumable )
	-> State
{
	if ( GW::GetInstanceType() != GW::InstanceType::Explorable )
		return WrongInstanceType{};

	if ( is_active( consumable.skill_id ) )
		return Active{};
	else
		return NotActive{};
}

auto April::CState<April::RockCandy>::get_state( RockCandy const& consumable )
	-> State
{
	if ( GW::GetInstanceType() != GW::InstanceType::Explorable )
		return WrongInstanceType{};

	if ( is_active( consumable.skill_id ) )
		return Active{};

	auto const slowed =
		is_active( GW::SkillID::Crippled )
		|| is_active( GW::SkillID::Tar );

	if ( slowed )
		return NotActive_Slowed{};
	else
		return NotActive_NotSlowed{};
}

auto April::CState<April::Conset>::get_state( Conset const& consumable )
	-> State
{
	if ( GW::GetInstanceType() != GW::InstanceType::Explorable )
		return WrongInstanceType{};

	if ( is_active( consumable.skill_id ) )
		return Active{};

	if ( GW::PartyMgr::GetIsPartyLoaded() )
		return NotActive_PartyLoaded{};
	else
		return NotActive_PartyNotLoaded{};
}

auto April::CState<April::LunarFortune>::get_state( LunarFortune )
	-> State
{
	if ( GW::GetInstanceType() != GW::InstanceType::Explorable )
		return WrongInstanceType{};

	if ( is_active( GW::SkillID::Lunar_Blessing ) )
		return Active{};
	else
		return NotActive{};
}

auto April::CState<April::Alcohol>::get_state( Alcohol )
	-> State
{
	if ( GW::GetInstanceType() != GW::InstanceType::Explorable )
		return WrongInstanceType{};

	if ( GW::Effects::GetAlcoholLevel() > 0 )
		return Drunk{};
	else
		return NotDrunk{};
}

auto April::CState<April::SummoningStone>::get_state( SummoningStone )
	-> State
{
	if ( GW::GetInstanceType() != GW::InstanceType::Explorable )
		return WrongInstanceType{};

	if ( is_active( GW::SkillID::Summoning_Sickness ) )
		return SummoningSickness{};
	else
		return NoSummoningSickness{};
}

auto April::CState<April::IdentKit>::get_state( IdentKit )
	-> State
{
	if ( GW::GetInstanceType() == GW::InstanceType::Loading )
		return WrongInstanceType{};

	// TODO: Accept different rarities
	if ( GW::FindUnidentGold() != nullptr )
		return NotAllItemsIdentified{};
	else
		return AllItemsIdentified{};
}

auto April::CState<April::MoraleBooster>::get_state(
	MoraleBooster const& consumable )
	-> State
{
	if ( GW::GetInstanceType() != GW::InstanceType::Explorable )
		return WrongInstanceType{};

	if ( GW::GetMorale() + consumable.max_boost >= 10 )
		return CouldCause10{};
	else
		return CouldNotCause10{};
}

auto April::CState<April::Tonic>::get_state( Tonic )
	-> State
{
	if ( GW::GetInstanceType() != GW::InstanceType::Explorable )
			return WrongInstanceType{};

	if ( is_active( GW::SkillID::Tonic_Tipsiness ) )
		return Tipsiness{};

	if ( is_disguised() )
		return Disguised{};
	else
		return NotDisguised{};
}

auto April::consumable_state( Consumable const& consumable )
-> ConsumableState
{
	auto const get_state = []( auto const& cons ) -> ConsumableState
	{
		using T = std::decay_t<decltype(cons)>;
		return CState<T>::get_state( cons );
	};

	return std::visit( get_state, consumable );
}
