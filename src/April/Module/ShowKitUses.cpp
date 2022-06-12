
#include "April/Module/ShowKitUses.h"

using namespace April::Module;
using namespace GW::Packet::StoC;


namespace {

	template<class Packet_t>
	auto get_uses( Packet_t const& p ) -> std::optional<uint32_t>
	{
		if ( auto const qty = GW::GetIdentKitUses( p.model_id, p.price ) )
			return *qty;

		if ( auto const qty = GW::GetSalvageKitUses( p.model_id, p.price ) )
			return *qty;

		return std::nullopt;
	}

	template<class Packet_t>
	void adjust_qty( Packet_t& packet )
	{
		if ( auto const new_qty = get_uses( packet ) )
			packet.quantity = *new_qty;
	}

	template<class Packet_t>
	void save_unchanged(
		Packet_t const& packet,
		std::vector<ShowKitUses::ItemPacket>& unchanged )
	{
		// using get_uses() as makeshift is_ident_or_salvage()
		if ( get_uses( packet ) == std::nullopt )
			return;

		auto get_id = []( auto const& packet ) { return packet.item_id; };

		auto is_current = [&]( ShowKitUses::ItemPacket const& saved )
		{
			return packet.item_id == std::visit( get_id, saved );
		};

		auto const found =
			std::find_if( unchanged.begin(), unchanged.end(), is_current );

		if ( found != unchanged.end() )
		{
			*found = packet;
		}
		else unchanged.push_back( packet );
	}

}


April::Module::ShowKitUses::ShowKitUses(
	std::shared_ptr<Config const> config )
	: config{ config }
{
}

void April::Module::ShowKitUses::Update( InstanceLoadStart const& )
{
	unchanged.clear();
}

void April::Module::ShowKitUses::Update( ItemGeneral_FirstID& packet )
{
	save_unchanged( packet, unchanged );

	if ( config->active )
		adjust_qty( packet );
}

void April::Module::ShowKitUses::Update( ItemGeneral_ReuseID& packet )
{
	save_unchanged( packet, unchanged );

	if ( config->active )
		adjust_qty( packet );
}
