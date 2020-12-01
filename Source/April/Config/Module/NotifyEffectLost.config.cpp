
#include "April/Config/Module/NotifyEffectLost.config.hpp"

#include "April/Utility/ConfigAssert.h"

using namespace April::Config;
using namespace April::NotifyEffectLostConfig;


namespace {

	constexpr bool ValidMessages()
	{
		for ( auto const& notification : notifications )
		{
			if ( not ValidString( notification.message ) ) return false;
		}
		return true;
	}

}


static_assert( ValidMessages() );
