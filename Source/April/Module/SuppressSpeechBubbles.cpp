
#include "April/Module/SuppressSpeechBubbles.h"

#include "Dependencies/GWCA.hpp"


namespace {

	auto entry = GW::HookEntry{};

}


April::SuppressSpeechBubbles::SuppressSpeechBubbles()
{
	// Callback will onyl be cleaned up during GWCA shutdown.
	GW::StoC::RegisterPacketCallback<GW::Packet::StoC::SpeechBubble>(
		&entry, []( auto* status, auto* ) { status->blocked = true; } );
}
