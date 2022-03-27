#pragma once

#include "April/Gui/InstanceTimer.h"


namespace April {

	template<typename T>
	struct JsonKey{};


	template<>
	struct JsonKey<Gui::InstanceTimer::Config> {
		static constexpr auto key = "Gui/InstanceTimer";
	};

}
