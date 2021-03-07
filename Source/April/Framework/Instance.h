#pragma once

#include "April/Module/AgentFilter.h"
#include "April/Module/ChainedSoul.h"
#include "April/Module/ChatCommands.h"
#include "April/Module/ChatFilter.h"
#include "April/Module/ConsumablesMgr.h"
#include "April/Module/CursorFix.h"
#include "April/Module/DhuumBot.h"
#include "April/Module/DhuumsJudgement.h"
#include "April/Module/NotifyEffectLoss.h"
#include "April/Module/ReturnToOutpost.h"
#include "April/Module/ShowKitUses.h"
#include "April/Module/SuppressSpeechBubbles.h"
#include "April/Module/UwTimer.h"
#include "April/Module/WindowManager.h"

#include "April/Gui/ChainedSoulInfo.h"
#include "April/Gui/DhuumBotGui.h"
#include "April/Gui/DhuumInfo.h"
#include "April/Gui/Dialogs.h"
#include "April/Gui/HealthEnergy.h"
#include "April/Gui/InstanceTimer.h"
#include "April/Gui/Inventory.h"
#include "April/Gui/Settings.h"
#include "April/Gui/Skillbar.h"
#include "April/Gui/TargetInfo.h"
#include "April/Gui/UwTimes.h"

#include <tuple>


namespace April {

	struct ModuleConfigurations {
		using Active =
			std::tuple<
				ConsumablesMgr::Config>;
		using Passive =
			std::tuple<
				AgentFilter::Config,
				ChatCommands::Config,
				ChatFilter::Config,
				NotifyEffectLoss::Config,
				ReturnToOutpost::Config,
				ShowKitUses::Config,
				SuppressSpeechBubbles::Config>;
		using Gui =
			std::tuple<
				Gui::ChainedSoulGui::Config,
				Gui::DhuumBotGui::Config,
				Gui::DhuumInfo::Config,
				Gui::Dialogs::Config,
				Gui::Energybar::Config,
				Gui::Healthbar::Config,
				Gui::InstanceTimer::Config,
				Gui::Inventory::Config,
				Gui::Settings::Config,
				Gui::Skillbar::Config,
				Gui::TargetInfo::Config,
				Gui::UwTimesGui::Config>;

		Active active;
		Passive passive;
		Gui gui;
	};


	class Instance {
	public:
		using Modules =
			std::tuple<
				ConsumablesMgr,
				ChainedSoul,
				DhuumBot,
				DhuumsJudgement,
				UwTimer,
				WindowMgr,
				AgentFilter,
				ChatCommands,
				ChatFilter,
				CursorFix,
				NotifyEffectLoss,
				ReturnToOutpost,
				ShowKitUses,
				SuppressSpeechBubbles,
				Gui::ChainedSoulGui,
				Gui::Energybar,
				Gui::DhuumBotGui,
				Gui::DhuumInfo,
				Gui::Dialogs,
				Gui::Healthbar,
				Gui::InstanceTimer,
				Gui::Inventory,
				Gui::Settings,
				Gui::Skillbar,
				Gui::TargetInfo,
				Gui::UwTimesGui>;


		Instance( Modules&&, ModuleConfigurations&& );

		void Update();
		void Display();

		void Shutdown();


	private:
		Modules modules;
		ModuleConfigurations config;
	};

}
