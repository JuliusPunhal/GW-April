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

#include <memory>
#include <tuple>


namespace April {

	class Modules {
	public:
		Modules( std::shared_ptr<UwTimes> );

		void Update();
		void Display();

		void Shutdown();


	private:
		using Active = 
			std::tuple<
				std::shared_ptr<ConsumablesMgr>,
				std::shared_ptr<ChainedSoul>,
				std::shared_ptr<DhuumBot>,
				std::shared_ptr<DhuumsJudgement>,
				std::shared_ptr<UwTimer>>;

		using Passive = 
			std::tuple<
				std::unique_ptr<AgentFilter>,
				std::unique_ptr<ChatCommands>,
				std::unique_ptr<ChatFilter>,
				std::unique_ptr<CursorFix>,
				std::unique_ptr<NotifyEffectLoss>,
				std::unique_ptr<ReturnToOutpost>,
				std::unique_ptr<ShowKitUses>,
				std::unique_ptr<SuppressSpeechBubbles>>;

		using Guis = 
			std::tuple<
				std::unique_ptr<Gui::ChainedSoulGui>,
				std::unique_ptr<Gui::Energybar>,
				std::unique_ptr<Gui::DhuumBotGui>,
				std::unique_ptr<Gui::DhuumInfo>,
				std::unique_ptr<Gui::Dialogs>,
				std::unique_ptr<Gui::Healthbar>,
				std::unique_ptr<Gui::InstanceTimer>,
				std::unique_ptr<Gui::Inventory>,
				std::unique_ptr<Gui::Settings>,
				std::unique_ptr<Gui::Skillbar>,
				std::unique_ptr<Gui::TargetInfo>,
				std::unique_ptr<Gui::UwTimesGui>>;


		Active active;
		Passive passive;
		Guis gui;
	};

}
