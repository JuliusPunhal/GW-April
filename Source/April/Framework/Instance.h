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

struct IDirect3DDevice9;

#include <memory>
#include <tuple>


namespace April {

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

	using Configuration =
		std::tuple<
			ConsumablesMgr::Config,
			AgentFilter::Config,
			ChatCommands::Config,
			ChatFilter::Config,
			NotifyEffectLoss::Config,
			ReturnToOutpost::Config,
			ShowKitUses::Config,
			SuppressSpeechBubbles::Config,
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


	struct Instance {
		Modules modules;
		Configuration config;

		bool terminate = false;


	private:
		Instance() = default;
		friend auto make_instance( IDirect3DDevice9* )
			-> std::shared_ptr<Instance>;
	};


	auto make_instance( IDirect3DDevice9* ) -> std::shared_ptr<Instance>;

	void Update( Instance& );
	void Display( Instance& );
	void Shutdown( Instance& );

	void OnDeviceReset( IDirect3DDevice9* );

}
