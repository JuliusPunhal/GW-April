
#include "April/Module/ShowKitUses.h"

#include <cmath>

using namespace GW::Packet::StoC;


namespace {

	template<class Packet_t>
	auto adjust_qty( Packet_t& packet )
	{
		switch ( packet.model_id )
		{
			case 5899: // Superior ID
			{
				auto const qty = std::round( packet.price / 2.5f );
				packet.quantity = static_cast<uint32_t>(qty);
				break;
			}
			case 5900: // Superior Salvage
			{
				auto const qty = std::round( packet.price / 10.f );
				packet.quantity = static_cast<uint32_t>(qty);
				break;
			}
			case 2989: // Regular ID
			case 2992: // Reg Salvage Kit
			{
				auto const qty = std::round( packet.price / 2.f );
				packet.quantity = static_cast<uint32_t>(qty);
				break;
			}
		}
	}

}


void April::ShowKitUses::UpdateKitUses(
	ItemGeneral& packet, Config const& config ) const
{
	if ( config.active )
		adjust_qty( packet );
}

void April::ShowKitUses::UpdateKitUses(
	ItemGeneral_ReuseID& packet, Config const& config ) const
{
	if ( config.active )
		adjust_qty( packet );
}

auto April::ShowKitUses::Config::LoadDefault() -> Config
{
	return Config{
		true
	};
}
