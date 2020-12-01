
#include "April/Module/AgentFilter.h"

#include "April/Config/Module/AgentFilter.Config.hpp"
#include "April/Utility/Consumable.h"
#include "April/Utility/stl.h"

namespace config = April::AgentFilterConfig;

using namespace GW::Packet::StoC;


namespace {

	auto entry = GW::HookEntry{};


	auto get_item_by_packet( AgentAdd const& packet ) -> GW::Item const*
	{
		// filter non-item-agents
		if ( packet.type != 4 || packet.unk3 != 0 ) // TODO: Verify
			return nullptr;

		auto const items = GW::Items::GetItemArray();
		if ( packet.agent_type >= items.size() )
			return nullptr;

		return items[packet.agent_type];
	}

	template<typename IDs, typename Rarities>
	bool want_to_display(
		GW::Item const& item, IDs const& ids, Rarities const& rarities )
	{
		// Check for modelid
		auto const modelid_it = std::find( ids, item.model_id );
		if ( modelid_it != std::end( ids ) )
			return true;

		// Check for rarity
		auto const item_rarity = GW::GetRarity( item );
		auto const rarity_it = std::find( rarities, item_rarity );
		if ( rarity_it != std::end( rarities ) )
			return true;

		return false;
	}

}


April::AgentFilter::AgentFilter()
{
	// Callbacks will only be cleaned up during GWCA shutdown.
	GW::StoC::RegisterPacketCallback<AgentAdd>(
		&entry, [this]( auto* s, auto* pak ) { OnSpawn( s, *pak ); } );

	GW::StoC::RegisterPacketCallback<AgentRemove>(
		&entry, [this]( auto* s, auto* pak ) { OnDespawn( s, *pak ); } );

	GW::StoC::RegisterPacketCallback<UpdateItemOwner>(
		&entry, [this]( auto*, auto* pak ) { UpdateOwner( *pak ); } );

	GW::StoC::RegisterPacketCallback<ItemGeneral_ReuseID>(
		&entry, [this]( auto*, auto* pak ) { DeleteOwner( *pak ); } );

	GW::StoC::RegisterPacketCallback<MapLoaded>(
		&entry, [this]( auto*, auto* ) { Reset(); } );
}

void April::AgentFilter::DisplaySuppressedItems()
{
	for ( auto const& packet : suppressed_packets )
	{
		GW::GameThread::Enqueue(
			[copy = packet]() mutable
			{
				GW::StoC::EmulatePacket( &copy );
			} );
	}
	suppressed_packets.clear();
}

void April::AgentFilter::OnSpawn( 
	GW::HookStatus* status, AgentAdd const& packet )
{
	auto const want_to_display = [this]( auto const& packet )
	{
		auto const item = get_item_by_packet( packet );
		if ( item == nullptr ) return true;

		auto const player = GW::Agents::GetCharacter();
		if ( player == nullptr )
			return true; // spectating, do not suppress

		auto const it_owner = item_owners.find( item->item_id );
		auto const can_pick_up =
			it_owner == item_owners.end()				// not reserved 
			|| it_owner->second == player->agent_id;	// reserved for user

		if ( can_pick_up )
			return ::want_to_display( 
				*item, 
				config::allowed_ids_player, 
				config::allowed_rarities_player );
		else
			return ::want_to_display( 
				*item, 
				config::allowed_ids_party, 
				config::allowed_rarities_party );
	};

	if ( not want_to_display( packet ) )
	{
		suppressed_packets.emplace_back( packet );
		status->blocked = true;
	}
}

void April::AgentFilter::OnDespawn( 
	GW::HookStatus* status, AgentRemove const& packet )
{
	auto const found_iter =
		std::find_if(
			suppressed_packets,
			[packet]( auto const& suppressed_packet )
			{
				return suppressed_packet.agent_id == packet.agent_id;
			} );

	if ( found_iter != std::end( suppressed_packets ) )
	{
		status->blocked = true;
		suppressed_packets.erase( found_iter );
	}
}

void April::AgentFilter::UpdateOwner( UpdateItemOwner const& packet )
{
	item_owners[packet.item_id] = packet.owner_agent_id;
}

void April::AgentFilter::DeleteOwner( ItemGeneral_ReuseID const& packet )
{
	// TODO: Confirm that ReuseID is sent before UpdateOwner
	auto const it = item_owners.find( packet.item_id );
	if ( it != item_owners.end() )
	{
		item_owners.erase( it );
	}
}

void April::AgentFilter::Reset()
{
	suppressed_packets.clear();
	item_owners.clear();
}
