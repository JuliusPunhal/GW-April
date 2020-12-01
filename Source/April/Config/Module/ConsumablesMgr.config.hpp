#pragma once

#include <chrono>


namespace April::ConsumablesMgrConfig {

	inline constexpr auto timeout = std::chrono::milliseconds{ 250 };

}
