#pragma once

#include "April/Gui/InstanceTimer.h"
#include "April/Utility/FileIO.BasicTypes.h"

#include "nlohmann/json.hpp"


namespace April::Gui {

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
		InstanceTimer::Config,
		font, label_color, shadow_color, shadow_offset, window )

}
