
#include "April/Framework/Instance.h"


void April::Update( Instance& instance )
{
	std::get<ConsumablesMgr>( instance.modules ).Update(
		std::get<ConsumablesMgr::Config>( instance.config ) );
	std::get<ChainedSoul>( instance.modules ).Update();
	std::get<DhuumBot>( instance.modules ).Update();
	std::get<DhuumsJudgement>( instance.modules ).Update();
	std::get<UwTimer>( instance.modules ).Update();
	std::get<WindowMgr>( instance.modules ).Update(
		std::get<Gui::ChainedSoulGui::Config>( instance.config ) );
}

void April::Display( Instance& instance )
{
	std::get<Gui::ChainedSoulGui>( instance.modules ).Display(
		std::get<ChainedSoul>( instance.modules ),
		std::get<Gui::ChainedSoulGui::Config>( instance.config ) );

	std::get<Gui::Energybar>( instance.modules ).Display(
		std::get<Gui::Energybar::Config>( instance.config ) );

	std::get<Gui::DhuumBotGui>( instance.modules ).Display(
		std::get<DhuumBot>( instance.modules ),
		std::get<Gui::DhuumBotGui::Config>( instance.config ) );

	std::get<Gui::DhuumInfo>( instance.modules ).Display(
		std::get<DhuumsJudgement>( instance.modules ),
		std::get<Gui::DhuumInfo::Config>( instance.config ) );

	std::get<Gui::Dialogs>( instance.modules ).Display(
		std::get<Gui::Dialogs::Config>( instance.config ) );

	std::get<Gui::Healthbar>( instance.modules ).Display(
		std::get<Gui::Healthbar::Config>( instance.config ) );

	std::get<Gui::InstanceTimer>( instance.modules ).Display(
		std::get<Gui::InstanceTimer::Config>( instance.config ) );

	std::get<Gui::Inventory>( instance.modules ).Display(
		std::get<ConsumablesMgr>( instance.modules ),
		std::get<Gui::Inventory::Config>( instance.config ) );

	std::get<Gui::Settings>( instance.modules ).Display(
		std::forward_as_tuple(
			std::get<ConsumablesMgr::Config>( instance.config ),
			std::get<AgentFilter::Config>( instance.config ),
			std::get<ChatCommands::Config>( instance.config ),
			std::get<ChatFilter::Config>( instance.config ),
			std::get<NotifyEffectLoss::Config>( instance.config ),
			std::get<ReturnToOutpost::Config>( instance.config ),
			std::get<ShowKitUses::Config>( instance.config ),
			std::get<SuppressSpeechBubbles::Config>( instance.config ),
			std::get<Gui::ChainedSoulGui::Config>( instance.config ),
			std::get<Gui::DhuumBotGui::Config>( instance.config ),
			std::get<Gui::DhuumInfo::Config>( instance.config ),
			std::get<Gui::Dialogs::Config>( instance.config ),
			std::get<Gui::Energybar::Config>( instance.config ),
			std::get<Gui::Healthbar::Config>( instance.config ),
			std::get<Gui::InstanceTimer::Config>( instance.config ),
			std::get<Gui::Inventory::Config>( instance.config ),
			std::get<Gui::Settings::Config>( instance.config ),
			std::get<Gui::Skillbar::Config>( instance.config ),
			std::get<Gui::TargetInfo::Config>( instance.config ),
			std::get<Gui::UwTimesGui::Config>( instance.config ) ) );

	std::get<Gui::Skillbar>( instance.modules ).Display(
		std::get<Gui::Skillbar::Config>( instance.config ) );

	std::get<Gui::TargetInfo>( instance.modules ).Display(
		std::get<Gui::TargetInfo::Config>( instance.config ) );

	std::get<Gui::UwTimesGui>( instance.modules ).Display(
		std::get<UwTimer>( instance.modules ),
		std::get<Gui::UwTimesGui::Config>( instance.config ) );
}

void April::Shutdown( Instance& instance )
{
	std::get<AgentFilter>( instance.modules ).DisplaySuppressedItems();
}
