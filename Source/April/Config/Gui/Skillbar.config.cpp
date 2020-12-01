
#include "April/Config/Gui/Skillbar.config.hpp"

#include "April/Utility/ConfigAssert.h"

#include <string_view>

using namespace April::Config;
using namespace April::Gui::SkillbarConfig;


namespace {
	
	constexpr bool SortedThresholds() noexcept
	{
		for ( auto it = 1u; it < thresholds.size(); ++it )
		{
			if ( thresholds[it].time >= thresholds[it - 1].time )
				return false;
		}
		return true;
	}

}


static_assert( ValidString( window_name ) );

static_assert( ValidString( font_path ) );
static_assert( IsFontPath( font_path ) );

static_assert( SortedThresholds() );
