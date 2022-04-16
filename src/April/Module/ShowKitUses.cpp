
#include "April/Module/ShowKitUses.h"

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

}


April::Module::ShowKitUses::ShowKitUses(
	std::shared_ptr<Config const> config )
	: config{ config }
{
}

void April::Module::ShowKitUses::Update( ItemGeneral& packet ) const
{
	if ( config->active )
		adjust_qty( packet );
}

void April::Module::ShowKitUses::Update( ItemGeneral_ReuseID& packet ) const
{
	if ( config->active )
		adjust_qty( packet );
}
