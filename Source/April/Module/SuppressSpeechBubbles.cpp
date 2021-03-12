
#include "April/Module/SuppressSpeechBubbles.h"


bool April::SuppressSpeechBubbles::should_suppress( Config const& config ) const
{
	return config.active;
}

auto April::SuppressSpeechBubbles::Config::LoadDefault() -> Config
{
	return Config{
		true
	};
}
