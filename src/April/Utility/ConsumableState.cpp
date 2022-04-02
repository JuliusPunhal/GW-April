
#include "April/Utility/ConsumableState.h"

#include "April/Utility/stl.h"

#include "GWCA/GWCA.hpp"

using namespace April;
using namespace GW::literals;


namespace {

	bool is_active( GW::SkillID const id )
	{
		auto const effect = GW::GetPlayerEffect( id );
		if ( effect == nullptr )
			return false;

		auto const remaining = GW::GetTimeRemaining( *effect );
		return remaining > 1000_ms || remaining < -1000_ms; // permanent
	}

	bool is_disguised()
	{
		return
			is_active( GW::SkillID::Disguised_verification_requested )
			|| is_active( GW::SkillID::Disguised_when_using_Everlasting_Legionnaire_Tonic )
			|| is_active( GW::SkillID::Disguised_when_using_everlasting_tonics_except_Everlasting_Legionnaire_Tonic )
			|| is_active( GW::SkillID::Disguised_when_using_non_everlasting_tonics );
	}

	bool can_use_consumables_in_region()
	{
		switch ( GW::GetCurrentMapInfo().type )
		{
			case GW::RegionType_ExplorableZone:     // Explorables
			case GW::RegionType_CooperativeMission: // e.g. Vizunah
			case GW::RegionType_EliteMission:       // e.g. DoA
			case GW::RegionType_Challenge:          // e.g. Remains of Sahlahja
			case GW::RegionType_MissionArea:        // Factions missions
			case GW::RegionType_EotnMission:
			case GW::RegionType_Dungeon:
				return true;
		}

		return false;
	}

	bool can_use()
	{
		return
			GW::GetInstanceType() == GW::InstanceType::Explorable
			&& can_use_consumables_in_region();
	}

	auto find_unident() -> GW::Item const*
	{
		auto const* bags = GW::GetInventoryBags();
		if ( bags == nullptr )
			return nullptr;

		for ( auto const* bag : *bags )
		{
			if ( bag == nullptr )
				continue;

			for ( auto const* item : bag->items )
			{
				if ( item && not GW::IsIdentified( *item ) )
					return item;
			}
		}

		return nullptr;
	}

}


auto April::detail::ConsumableState<Trivial>::get_State(
	Trivial const& consumable )
	-> State
{
	if ( not can_use() )
		return WrongInstanceType{};

	if ( is_active( consumable.skill_id ) )
		return Active{};

	return NotActive{};
}

auto April::detail::ConsumableState<RockCandy>::get_State(
	RockCandy const& consumable )
	-> State
{
	if ( not can_use() )
		return WrongInstanceType{};

	if ( is_active( consumable.skill_id ) )
		return Active{};

	if ( is_active( GW::SkillID::Crippled ) || is_active( GW::SkillID::Tar ) )
		return NotActive{};

	return NotSlowed{};
}

auto April::detail::ConsumableState<Conset>::get_State(
	Conset const& consumable )
	-> State
{
	if ( not can_use() )
		return WrongInstanceType{};

	if ( is_active( consumable.skill_id ) )
		return Active{};

	if ( GW::GetIsPartyLoaded() )
		return NotActive{};

	return PartyNotLoaded{};
}

auto April::detail::ConsumableState<Lunar>::get_State( Lunar ) -> State
{
	if ( not can_use() )
		return WrongInstanceType{};

	if ( is_active( GW::SkillID::Lunar_Blessing ) )
		return Active{};

	return NotActive{};
}

auto April::detail::ConsumableState<Alcohol>::get_State( Alcohol ) -> State
{
	if ( not can_use() )
		return WrongInstanceType{};

	if ( GW::GetDrunkTime() > 0_ms )
		return Active{};

	return NotActive{};
}

auto April::detail::ConsumableState<SummoningStone>::get_State(
	SummoningStone )
	-> State
{
	if ( not can_use() )
		return WrongInstanceType{};

	if ( is_active( GW::SkillID::Summoning_Sickness ) )
		return SummoningSickness{};

	return NotActive{};
}

auto April::detail::ConsumableState<IdentKit>::get_State( IdentKit ) -> State
{
	if ( GW::GetInstanceType() == GW::InstanceType::Loading )
		return WrongInstanceType{};

	if ( find_unident() )
		return UnidentFound{};

	return AllIdent{};
}

auto April::detail::ConsumableState<MoraleBooster>::get_State(
	MoraleBooster const& consumable )
	-> State
{
	if ( not can_use() )
		return WrongInstanceType{};

	if ( GW::GetMorale() + consumable.max_boost >= 10 )
		return CouldCause10{};

	return NotActive{};
}

auto April::detail::ConsumableState<DpRemover>::get_State( DpRemover const& )
	-> State
{
	if ( not can_use() )
		return WrongInstanceType{};

	if ( GW::GetMorale() < 0 )
		return NotActive{};

	return NoDP{};
}

auto April::detail::ConsumableState<Tonic>::get_State( Tonic ) -> State
{
	if ( not can_use() )
		return WrongInstanceType{};

	if ( is_active( GW::SkillID::Tonic_Tipsiness ) )
		return Tipsiness{};

	if ( is_disguised() )
		return Active{};

	return NotActive{};
}

auto April::get_ConsumableState( Consumable const& consumable )
	-> ConsumableState
{
	auto const get_State = []( auto const& cons ) -> ConsumableState
	{
		using T = stl::remove_cvref_t<decltype( cons )>;
		return detail::ConsumableState<T>::get_State( cons );
	};

	return std::visit( get_State, consumable );
}
