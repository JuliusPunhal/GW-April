
#include "April/Module/AgentFilter.h"

#include "April/Utility/Consumable.h"
#include "April/Utility/stl.h"

#include "Dependencies/GWCA.hpp"

using Config = April::AgentFilter::Config;

using namespace GW::Packet::StoC;


namespace {

	auto entry = GW::HookEntry{};


	auto is_item( AgentAdd const& packet ) -> GW::Item const*
	{
		// filter non-item-agents
		if ( packet.type != 4 || packet.unk3 != 0 ) // TODO: Verify
			return nullptr;

		auto const& items = GW::GameContext::instance()->items->item_array;
		auto const item_id = packet.agent_type;
		if ( item_id >= items.size() )
			return nullptr;

		return items[item_id];
	}

	bool want_to_display( 
		GW::Item const& item, 
		std::vector<GW::ItemID> const& ids, 
		std::vector<GW::Rarity> const& rarities )
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


April::AgentFilter::AgentFilter( Config const& config )
	: config{ config }
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
		auto const* item = is_item( packet );
		if ( not item ) return true;

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
				config.visible_user_items, 
				config.visible_user_rarities );
		else
			return ::want_to_display( 
				*item, 
				config.visible_party_items, 
				config.visible_party_rarities );
	};

	if ( config.active && not want_to_display( packet ) )
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

auto April::AgentFilter::Config::LoadDefault() -> Config
{
	auto player_items = std::vector<GW::ItemID>{
		// Rare and valuable items
		GW::Constants::ItemID::EternalBlade,
		GW::Constants::ItemID::VoltaicSpear,
		GW::Constants::ItemID::Lockpick,
		GW::Constants::ItemID::ResScroll,
		GW::Constants::ItemID::GakiSummon,
		GW::Constants::ItemID::TurtleSummon,
		GW::Constants::ItemID::MiniDhuum,
		GW::Constants::ItemID::DSR,
		GW::Constants::ItemID::ELMiku,
		GW::Constants::ItemID::Diamond,
		GW::Constants::ItemID::Ruby,
		GW::Constants::ItemID::Sapphire,
		GW::Constants::ItemID::PhantomKey,
		GW::Constants::ItemID::GhastlyStone,
		GW::Constants::ItemID::GlobofEctoplasm,
		GW::Constants::ItemID::ObsidianShard,

		// consumables
		GW::Constants::ItemID::Cupcakes,
		GW::Constants::ItemID::Apples,
		GW::Constants::ItemID::Corns,
		GW::Constants::ItemID::Pies,
		GW::Constants::ItemID::Eggs,
		GW::Constants::ItemID::Warsupplies,
		GW::Constants::ItemID::SkalefinSoup,
		GW::Constants::ItemID::PahnaiSalad,
		GW::Constants::ItemID::Kabobs,

		GW::Constants::ItemID::GRC,
		GW::Constants::ItemID::BRC,
		GW::Constants::ItemID::RRC,

		GW::Constants::ItemID::ConsEssence,
		GW::Constants::ItemID::ConsArmor,
		GW::Constants::ItemID::ConsGrail,

		GW::Constants::ItemID::LunarDragon,
		GW::Constants::ItemID::LunarHorse,
		GW::Constants::ItemID::LunarMonkey,
		GW::Constants::ItemID::LunarOx,
		GW::Constants::ItemID::LunarRabbit,
		GW::Constants::ItemID::LunarRat,
		GW::Constants::ItemID::LunarRooster,
		GW::Constants::ItemID::LunarSheep,
		GW::Constants::ItemID::LunarSnake,
		GW::Constants::ItemID::LunarTiger,
		GW::Constants::ItemID::LunarDog,
		GW::Constants::ItemID::LunarPig,

		GW::Constants::ItemID::Grog,
		GW::Constants::ItemID::SpikedEggnog,
		GW::Constants::ItemID::FlaskOfFirewater,
		GW::Constants::ItemID::Absinthe,
		GW::Constants::ItemID::HuntersAle,

		GW::Constants::ItemID::GhastlyStone,
		GW::Constants::ItemID::LegionnaireStone,
		GW::Constants::ItemID::TurtleSummon,
		GW::Constants::ItemID::GakiSummon,
		GW::Constants::ItemID::ImperialGuardSummon,

		GW::Constants::ItemID::IdentKit,
		GW::Constants::ItemID::IdentKit_Superior,

		GW::Constants::ItemID::PumpkinCookie,

		GW::Constants::ItemID::ELMiku,
	};
	auto player_rarities = std::vector<GW::Rarity>{ 
		GW::Rarity::Gold 
	};
	auto party_items = std::vector<GW::ItemID>{
		GW::Constants::ItemID::EternalBlade,
		GW::Constants::ItemID::VoltaicSpear,
		GW::Constants::ItemID::MiniDhuum,
		GW::Constants::ItemID::DSR
	};
	auto party_rarities = std::vector<GW::Rarity>{};

	auto const config = Config{
		true,
		std::move( player_items ),
		std::move( player_rarities ),
		std::move( party_items ),
		std::move( party_rarities )
	};

	return config;
}
