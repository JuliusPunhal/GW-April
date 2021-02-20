
#include "April/Module/SuppressSpeechBubbles.h"


April::SuppressSpeechBubbles::SuppressSpeechBubbles( Config const& config )
	: config{ config }
{
}

void April::SuppressSpeechBubbles::Suppress( GW::HookStatus* status ) const
{
	if ( config.active )
		status->blocked = true;
}

auto April::SuppressSpeechBubbles::Config::LoadDefault() -> Config
{
	return Config{
		true
	};
}
