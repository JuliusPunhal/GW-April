
#include "April/Framework/Instance.h"


void April::Update( Instance& instance )
{
	std::get<ConsumablesMgr>( instance.modules ).Update(
		std::get<ConsumablesMgr::Config>( instance.config.active ) );
	std::get<ChainedSoul>( instance.modules ).Update();
	std::get<DhuumBot>( instance.modules ).Update();
	std::get<DhuumsJudgement>( instance.modules ).Update();
	std::get<UwTimer>( instance.modules ).Update();
	std::get<WindowMgr>( instance.modules ).Update( instance.config );
}

void April::Display( Instance& instance )
{
	auto& chained_soul = std::get<ChainedSoul>( instance.modules );
	auto& dhuum_bot = std::get<DhuumBot>( instance.modules );
	auto& dhuums_judgement = std::get<DhuumsJudgement>( instance.modules );
	auto& cons = std::get<ConsumablesMgr>( instance.modules );
	auto& uw_timer = std::get<UwTimer>( instance.modules );

	std::get<Gui::ChainedSoulGui>( instance.modules ).Display(
		chained_soul,
		std::get<Gui::ChainedSoulGui::Config>( instance.config.gui ) );

	std::get<Gui::Energybar>( instance.modules ).Display(
		std::get<Gui::Energybar::Config>( instance.config.gui ) );

	std::get<Gui::DhuumBotGui>( instance.modules ).Display(
		dhuum_bot,
		std::get<Gui::DhuumBotGui::Config>( instance.config.gui ) );

	std::get<Gui::DhuumInfo>( instance.modules ).Display(
		dhuums_judgement,
		std::get<Gui::DhuumInfo::Config>( instance.config.gui ) );

	std::get<Gui::Dialogs>( instance.modules ).Display(
		std::get<Gui::Dialogs::Config>( instance.config.gui ) );

	std::get<Gui::Healthbar>( instance.modules ).Display(
		std::get<Gui::Healthbar::Config>( instance.config.gui ) );

	std::get<Gui::InstanceTimer>( instance.modules ).Display(
		std::get<Gui::InstanceTimer::Config>( instance.config.gui ) );

	std::get<Gui::Inventory>( instance.modules ).Display(
		cons,
		std::get<Gui::Inventory::Config>( instance.config.gui ) );

	std::get<Gui::Settings>( instance.modules ).Display( instance.config );

	std::get<Gui::Skillbar>( instance.modules ).Display(
		std::get<Gui::Skillbar::Config>( instance.config.gui ) );

	std::get<Gui::TargetInfo>( instance.modules ).Display(
		std::get<Gui::TargetInfo::Config>( instance.config.gui ) );

	std::get<Gui::UwTimesGui>( instance.modules ).Display(
		uw_timer,
		std::get<Gui::UwTimesGui::Config>( instance.config.gui ) );
}

void April::Shutdown( Instance& instance )
{
	std::get<AgentFilter>( instance.modules ).DisplaySuppressedItems();
}
