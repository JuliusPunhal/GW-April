
#include "April/Module/SuppressSpeechBubbles.h"

using namespace GW::Packet::StoC;


April::Module::SuppressSpeechBubbles::SuppressSpeechBubbles(
	std::shared_ptr<Config const> config )
	: config{ config }
{
}

void April::Module::SuppressSpeechBubbles::Update(
	GW::HookStatus& status, SpeechBubble const& ) const
{
	if ( config->active )
		status.blocked = true;
}
