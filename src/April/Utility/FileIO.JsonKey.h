#pragma once

#include "April/Gui/InstanceTimer.h"
#include "April/Gui/Skillbar.h"
#include "April/Gui/UwTimer.h"


namespace April {

	template<typename T>
	struct JsonKey{};


	template<>
	struct JsonKey<Gui::InstanceTimer::Config> {
		static constexpr auto key = "Gui/InstanceTimer";
	};

	template<>
	struct JsonKey<Gui::UwTimer::Config> {
		static constexpr auto key = "Gui/UwTimer";
	};

	template<>
	struct JsonKey<Gui::Skillbar::Config> {
		static constexpr auto key = "Gui/Skillbar";
	};

}
