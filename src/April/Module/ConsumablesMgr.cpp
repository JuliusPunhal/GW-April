
#include "April/Module/ConsumablesMgr.h"

#include "April/Utility/ConsumableState.h"
#include "April/Utility/stl.h"

#include <type_traits>

using namespace April;
using namespace GW::Packet::StoC;
using namespace std::chrono;

template<typename T>
using CS = April::detail::ConsumableState<T>;


namespace {

	template<typename Cons_t, typename State_t>
	bool use_regular( Cons_t const& cons, State_t const& )
	{
		using NotActive = typename CS<Cons_t>::NotActive;
		if constexpr ( std::is_same_v<State_t, NotActive> )
		{
			if ( auto const* item = GW::SearchInventory( cons.model_id ) )
				return GW::UseItem( *item ), true;
		}

		return false;
	}

	template<typename State_t>
	bool use_identkit( IdentKit const& cons, State_t const& state )
	{
		using UnidentFound = typename CS<IdentKit>::UnidentFound;
		if constexpr ( std::is_same_v<State_t, UnidentFound> )
		{
			if (
				auto const* kit = GW::SearchInventory( cons.model_id );
				kit && state.unident )
			{
				GW::IdentifyItem( *state.unident, *kit );
				return true;
			}
		}

		return false;
	}

	template<typename Cons_t, typename State_t>
	bool use_cons( Cons_t const& cons, State_t const& state )
	{
		if constexpr ( std::is_same_v<Cons_t, IdentKit> )
		{
			return use_identkit( cons, state );
		}
		else return use_regular( cons, state );
	}

	bool use_consumable( Consumable const& consumable )
	{
		// Somewhat convoluted dispatching since Consumable is an std::variant
		// and ConsumableState is a std::variant of std::variants.

		auto dispatch_state = [&]( auto const& state )
		{
			auto dispatch_both = []( auto const& cons, auto const& state )
			{
				return use_cons( cons, state );
			};
			return std::visit( dispatch_both, consumable, state );
		};
		return std::visit( dispatch_state, get_ConsumableState( consumable ) );
	}

	bool use_consumables( std::vector<Consumable> const& container )
	{
		for ( auto const& cons : container )
		{
			if ( use_consumable( cons ) )
				return true;
		}
		return false;
	}

}


void April::Module::ConsumablesMgr::Update() const
{
	if ( steady_clock::now() - last_use < 250ms )
		return;

	auto const* player = GW::GetCharacter();
	if ( player == nullptr || player->GetIsDead() )
		return;

	if ( not GW::GetIsPlayerLoaded() )
		return;

	if ( GW::InCinematic() )
		return;

	if ( GW::GetInventoryBags() == nullptr )
		return;

	if ( use_consumables( perm ) || use_consumables( temp ) )
	{
		last_use = steady_clock::now();
	}
}

void April::Module::ConsumablesMgr::Update( MapLoaded const& )
{
	if ( GW::GetInstanceType() == GW::InstanceType::Outpost )
		deactivate_all_temp();
}

void April::Module::ConsumablesMgr::Update( ObjectiveDone const& packet )
{
	if ( objective && packet.objective_id == *objective )
		deactivate_all_temp();
}

void April::Module::ConsumablesMgr::activate_perm( GW::ItemModelID const id )
{
	if (
		auto const* cons = is_Consumable( id );
		cons && std::find( perm.begin(), perm.end(), id ) == perm.end() )
	{
		perm.push_back( *cons );
	}
}

void April::Module::ConsumablesMgr::activate_temp( GW::ItemModelID const id )
{
	if (
		auto const* cons = is_Consumable( id );
		cons && std::find( temp.begin(), temp.end(), id ) == temp.end() )
	{
		temp.push_back( *cons );
	}
}

void April::Module::ConsumablesMgr::deactivate_perm( GW::ItemModelID const id )
{
	auto const it = std::find( perm.begin(), perm.end(), id );
	if ( it != perm.end() )
		perm.erase( it );
}

void April::Module::ConsumablesMgr::deactivate_temp( GW::ItemModelID const id )
{
	auto const it = std::find( temp.begin(), temp.end(), id );
	if ( it != temp.end() )
		temp.erase( it );
}

void April::Module::ConsumablesMgr::deactivate_all_perm()
{
	perm.clear();
}

void April::Module::ConsumablesMgr::deactivate_all_temp()
{
	temp.clear();
}

bool April::Module::ConsumablesMgr::is_active_perm(
	GW::ItemModelID const id ) const
{
	return std::find( perm.begin(), perm.end(), id ) != perm.end();
}

bool April::Module::ConsumablesMgr::is_active_temp(
	GW::ItemModelID const id ) const
{
	return std::find( temp.begin(), temp.end(), id ) != temp.end();
}
