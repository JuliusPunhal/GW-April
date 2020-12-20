
#include "April/Module/ShowKitUses.h"

#include <cmath>
#include <optional>


namespace {

	auto entry = GW::HookEntry{};

	
	template<class Packet_t>
	auto adjust_qty( Packet_t* packet )
	{
		switch ( packet->model_id )
		{
			case 5899: // Superior ID
			{
				auto const qty = std::round( packet->price / 2.5f );
				packet->quantity = static_cast<uint32_t>(qty);
				break;
			}
			case 5900: // Superior Salvage
			{
				auto const qty = std::round( packet->price / 10.f );
				packet->quantity = static_cast<uint32_t>(qty);
				break;
			}
			case 2989: // Regular ID
			case 2992: // Reg Salvage Kit
			{
				auto const qty = std::round( packet->price / 2.f );
				packet->quantity = static_cast<uint32_t>(qty);
				break;
			}
		}
	}

}



April::ShowKitUses::ShowKitUses( Config const& config )
	: config{ config }
{
	using namespace GW::Packet::StoC;

	// Callbacks will only be cleaned up during GWCA shutdown.
	GW::StoC::RegisterPacketCallback<ItemGeneral>( 
		&entry, 
		[this] ( auto*, auto* packet )
		{
			if ( this->config.active ) adjust_qty( packet );
		} );

	GW::StoC::RegisterPacketCallback<ItemGeneral_ReuseID>(
		&entry,
		[this] ( auto*, auto* packet )
		{
			if ( this->config.active ) adjust_qty( packet );
		} );
}

auto April::ShowKitUses::Config::LoadDefault() -> Config
{
	auto const config = Config{
		true
	};

	return config;
}
