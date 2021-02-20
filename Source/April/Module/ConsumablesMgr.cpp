
#include "April/Module/ConsumablesMgr.h"
#include "April/Module/ConsumablesMgr.AllowUse.hpp"

#include "April/Utility/ConsumableState.h"
#include "April/Utility/stl.h"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"

#include <algorithm>
#include <optional>
#include <variant>

using namespace std::chrono;


namespace {

	void use_item( April::Consumable const& consumable, GW::Item const& item )
	{
		using namespace April;

		auto const visitor = std::overloaded{
			[=]( Trivial const& )			{ return GW::UseItem( item ); },
			[=]( RockCandy const& )			{ return GW::UseItem( item ); },
			[=]( Conset const& )			{ return GW::UseItem( item ); },
			[=]( LunarFortune const& )		{ return GW::UseItem( item ); },
			[=]( Alcohol const& )			{ return GW::UseItem( item ); },
			[=]( SummoningStone const& )	{ return GW::UseItem( item ); },
			[=]( IdentKit const& )
			{
				auto const* unident_item = GW::FindUnidentGold();
				if ( unident_item )
					GW::IdentifyItem( *unident_item, item );
			},
			[=]( MoraleBooster const& )		{ return GW::UseItem( item ); },
			[=]( Tonic const& )				{ return GW::UseItem( item ); }
		};

		return std::visit( visitor, consumable );
	}

	auto get_model_id( April::Consumable const cons )
	{
		auto const get_id = []( auto const& cons ) { return cons.model_id; };
		return std::visit( get_id, cons );
	}

	bool use_consumables(
		April::unique_vector<April::Consumable> const& container )
	{
		for ( auto const& consumable : container )
		{
			auto const state = consumable_state( consumable );
			if ( not April::AllowUse( state ) )
				continue;

			auto const modelid = get_model_id( consumable );
			auto const item = GW::Items::GetItemByModelId( modelid );
			if ( item == nullptr )
				continue;

			use_item( consumable, *item );
			return true;
		}

		return false;
	}

}


April::ConsumablesMgr::ConsumablesMgr( Config const& config )
	: config{ config }
{
}

void April::ConsumablesMgr::Update()
{
	if ( steady_clock::now() - last_use < config.timeout )
		return;

	auto const* player = GW::Agents::GetCharacter();
	if ( player == nullptr || player->GetIsDead() )
		return;

	if ( use_consumables( persistent ) || use_consumables( temporary ) )
	{
		last_use = steady_clock::now();
	}
}

void April::ConsumablesMgr::Update(
	GW::Packet::StoC::ObjectiveDone const& packet )
{
	if ( packet.objective_id == deactivating_quest )
		deactivate_all_temporary();
}

void April::ConsumablesMgr::Update( GW::Packet::StoC::MapLoaded const& )
{
	if ( GW::GetInstanceType() == GW::InstanceType::Outpost )
		deactivate_all_temporary();
}

auto April::ConsumablesMgr::is_active( GW::ItemID const id ) const -> Active
{
	return {
		std::find( temporary, id ) != temporary.end(),
		std::find( persistent, id ) != persistent.end()
	};
}

void April::ConsumablesMgr::activate_temporary( GW::ItemID const id )
{
	auto const found = is_consumable( id );
	if ( not found )
		return;

	temporary.push_back( *found );
}

void April::ConsumablesMgr::deactivate_temporary( GW::ItemID const id )
{
	if ( auto const it = std::find( temporary, id ); it != temporary.end() )
		temporary.erase( it );
}

void April::ConsumablesMgr::deactivate_all_temporary()
{
	temporary.clear();
}

void April::ConsumablesMgr::activate_persistent( GW::ItemID const id )
{
	auto const found = is_consumable( id );
	if ( not found )
		return;

	persistent.push_back( *found );
}

void April::ConsumablesMgr::deactivate_persistent( GW::ItemID const id )
{
	if ( auto const it = std::find( persistent, id ); it != persistent.end() )
		persistent.erase( it );
}

void April::ConsumablesMgr::deactivate_all_persistent()
{
	persistent.clear();
}

auto April::ConsumablesMgr::Config::LoadDefault() -> Config
{
	return Config{
		250ms
	};
}
