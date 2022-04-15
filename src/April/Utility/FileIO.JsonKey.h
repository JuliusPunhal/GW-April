#pragma once

#include "April/Gui/HealthEnergy.h"
#include "April/Gui/InstanceTimer.h"
#include "April/Gui/Inventory.h"
#include "April/Gui/Skillbar.h"
#include "April/Gui/UwTimer.h"
#include "April/Module/ChatCommands.h"
#include "April/Module/ItemFilter.h"


namespace April {

	template<typename T>
	struct JsonKey{};


	template<>
	struct JsonKey<Gui::Energybar::Config> {
		static constexpr auto key = "Gui/Energybar";
	};

	template<>
	struct JsonKey<Gui::Healthbar::Config> {
		static constexpr auto key = "Gui/Healthbar";
	};

	template<>
	struct JsonKey<Gui::InstanceTimer::Config> {
		static constexpr auto key = "Gui/InstanceTimer";
	};

	template<>
	struct JsonKey<Gui::Inventory::Config> {
		static constexpr auto key = "Gui/Inventory";
	};

	template<>
	struct JsonKey<Gui::UwTimer::Config> {
		static constexpr auto key = "Gui/UwTimer";
	};

	template<>
	struct JsonKey<Gui::Skillbar::Config> {
		static constexpr auto key = "Gui/Skillbar";
	};

	template<>
	struct JsonKey<Module::ChatCommands::Config> {
		static constexpr auto key = "Module/ChatCommands";
	};

	template<>
	struct JsonKey<Module::ItemFilter::Config> {
		static constexpr auto key = "Module/ItemFilter";
	};

}
