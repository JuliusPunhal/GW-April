
#include "April/Module/SuppressSpeechBubbles.h"

#include "Dependencies/GWCA.hpp"


namespace {

	auto entry = GW::HookEntry{};

}


April::SuppressSpeechBubbles::SuppressSpeechBubbles( Config const& config )
	: config{ config }
{
	// Callback will onyl be cleaned up during GWCA shutdown.
	GW::StoC::RegisterPacketCallback<GW::Packet::StoC::SpeechBubble>(
		&entry, 
		[this]( auto* status, auto* ) 
		{ 
			if ( this->config.active ) 
				status->blocked = true; 
		} );
}

auto April::SuppressSpeechBubbles::Config::LoadDefault() -> Config
{
	auto const config = Config{
		true
	};

	return config;
}
