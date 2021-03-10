
#include "April/Framework/Instance.h"

#include "April/Utility/FileIO.h"


namespace {

	template<typename T>
	auto load_config()
	{
		auto const config = April::IO::from_file<T>( T::path );
		if ( config == std::nullopt )
			return T::LoadDefault();

		return *config;
	}

}


auto April::make_instance() -> std::unique_ptr<Instance>
{
	auto config = Configuration{
		load_config<ConsumablesMgr::Config>(),
		load_config<AgentFilter::Config>(),
		load_config<ChatCommands::Config>(),
		load_config<ChatFilter::Config>(),
		load_config<NotifyEffectLoss::Config>(),
		load_config<ReturnToOutpost::Config>(),
		load_config<ShowKitUses::Config>(),
		load_config<SuppressSpeechBubbles::Config>(),
		load_config<Gui::ChainedSoulGui::Config>(),
		load_config<Gui::DhuumBotGui::Config>(),
		load_config<Gui::DhuumInfo::Config>(),
		load_config<Gui::Dialogs::Config>(),
		load_config<Gui::Energybar::Config>(),
		load_config<Gui::Healthbar::Config>(),
		load_config<Gui::InstanceTimer::Config>(),
		load_config<Gui::Inventory::Config>(),
		load_config<Gui::Settings::Config>(),
		load_config<Gui::Skillbar::Config>(),
		load_config<Gui::TargetInfo::Config>(),
		load_config<Gui::UwTimesGui::Config>()
	};

	auto modules = Modules{
		ConsumablesMgr{},
		ChainedSoul{},
		DhuumBot{},
		DhuumsJudgement{},
		UwTimer{},
		WindowMgr{},
		AgentFilter{},
		ChatCommands{},
		ChatFilter{},
		CursorFix{},
		NotifyEffectLoss{},
		ReturnToOutpost{},
		ShowKitUses{},
		SuppressSpeechBubbles{},
		Gui::ChainedSoulGui{},
		Gui::Energybar{
			std::get<Gui::Energybar::Config>( config )
		},
		Gui::DhuumBotGui{},
		Gui::DhuumInfo{},
		Gui::Dialogs{},
		Gui::Healthbar{
			std::get<Gui::Healthbar::Config>( config )
		},
		Gui::InstanceTimer{
			std::get<Gui::InstanceTimer::Config>( config )
		},
		Gui::Inventory{
			std::get<Gui::Inventory::Config>( config )
		},
		Gui::Settings{},
		Gui::Skillbar{
			std::get<Gui::Skillbar::Config>( config )
		},
		Gui::TargetInfo{},
		Gui::UwTimesGui{}
	};

	return std::make_unique<Instance>(
		Instance{ std::move( modules ), std::move( config ) } );
}

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
