
#include "April/Module/ConsumablesMgr.h"
#include "April/Module/ConsumablesMgr.AllowUse.hpp"

#include "April/Config/Module/ConsumablesMgr.config.hpp"
#include "April/Utility/ConsumableState.h"
#include "April/Utility/stl.h"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"

#include <algorithm>
#include <optional>
#include <variant>

namespace config = April::ConsumablesMgrConfig;

using namespace std::chrono;


namespace {

	auto entry = GW::HookEntry{};


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


April::ConsumablesMgr::ConsumablesMgr()
{
	using namespace GW::Packet::StoC;

	// Callbacks will only be cleaned up during GWCA shutdown.
	GW::StoC::RegisterPacketCallback<ObjectiveDone>(
		&entry,
		[this]( auto*, auto* packet )
		{
			if ( packet->objective_id == deactivating_quest )
				DeactivateAll();
		} );
	
	GW::StoC::RegisterPacketCallback<MapLoaded>(
		&entry,
		[this]( auto*, auto* )
		{
			if ( GW::GetInstanceType() == GW::InstanceType::Outpost )
				DeactivateAll();
		} );
}

void April::ConsumablesMgr::Update()
{
	if ( steady_clock::now() - last_use < config::timeout ) 
		return;

	auto const* player = GW::Agents::GetCharacter();
	if ( player == nullptr || player->GetIsDead() )
		return;

	if ( use_consumables( persistent ) || use_consumables( until_load ) )
	{
		last_use = steady_clock::now();
	}
}

void April::ConsumablesMgr::Activate( GW::ItemID const id, bool const perma )
{
	if ( not std::holds_alternative<Inactive>( IsActive( id ) ) )
		return;
	
	auto const found = std::find( consumables, id );
	if ( found == consumables.end() )
		return;

	if ( perma )
		persistent.push_back( *found );
	else
		until_load.push_back( *found );
}

void April::ConsumablesMgr::Deactivate( GW::ItemID const id, bool const perma )
{
	// unconditionally remove from until_load
	{
		auto const iter = std::find( until_load, id );
		if ( iter != until_load.end() )
			until_load.erase( iter );
	}

	// if flag is set, also remove from persistent
	if ( perma )
	{
		auto const iter = std::find( persistent, id );
		if ( iter != persistent.end() )
			persistent.erase( iter );
	}
}

void April::ConsumablesMgr::DeactivateAll( bool const perma )
{
	until_load.clear();
	if ( perma ) persistent.clear();
}

auto April::ConsumablesMgr::IsActive( GW::ItemID const id ) const
	-> ActiveState
{
	if ( std::find( until_load, id ) != until_load.end() )
		return ActiveState{ UntilLoad{} };
	
	if ( std::find( persistent, id ) != persistent.end() )
		return ActiveState{ Persistent{} };

	return ActiveState{ Inactive{} };
}
