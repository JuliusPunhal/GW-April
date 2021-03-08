#pragma once

#include "April/Module/AgentFilter.h"
#include "April/Module/ChatCommands.h"
#include "April/Module/ChatFilter.h"
#include "April/Module/ConsumablesMgr.h"
#include "April/Module/NotifyEffectLoss.h"
#include "April/Module/ReturnToOutpost.h"
#include "April/Module/ShowKitUses.h"
#include "April/Module/SuppressSpeechBubbles.h"

#include "April/Gui/ChainedSoulInfo.h"
#include "April/Gui/DhuumBotGui.h"
#include "April/Gui/DhuumInfo.h"
#include "April/Gui/Dialogs.h"
#include "April/Gui/HealthEnergy.h"
#include "April/Gui/InstanceTimer.h"
#include "April/Gui/Inventory.h"
#include "April/Gui/Skillbar.h"
#include "April/Gui/TargetInfo.h"
#include "April/Gui/UwTimes.h"

#include "April/Utility/DrawStructures.h"

#include <tuple>


namespace April::Gui {

	class Settings {
	public:
		struct Config {
			Window window;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\Gui_Settings.ini";
		};


		Settings() = default;

		void Display(
			std::tuple<
				ConsumablesMgr::Config&,
				AgentFilter::Config&,
				ChatCommands::Config&,
				ChatFilter::Config&,
				NotifyEffectLoss::Config&,
				ReturnToOutpost::Config&,
				ShowKitUses::Config&,
				SuppressSpeechBubbles::Config&,
				Gui::ChainedSoulGui::Config&,
				Gui::DhuumBotGui::Config&,
				Gui::DhuumInfo::Config&,
				Gui::Dialogs::Config&,
				Gui::Energybar::Config&,
				Gui::Healthbar::Config&,
				Gui::InstanceTimer::Config&,
				Gui::Inventory::Config&,
				Gui::Settings::Config&,
				Gui::Skillbar::Config&,
				Gui::TargetInfo::Config&,
				Gui::UwTimesGui::Config&> ) const;
	};

}
