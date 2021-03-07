
#include "April/Module/SuppressSpeechBubbles.h"


void April::SuppressSpeechBubbles::Suppress(
	GW::HookStatus* status, Config const& config ) const
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
