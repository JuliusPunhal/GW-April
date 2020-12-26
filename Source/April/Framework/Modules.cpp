
#include "April/Framework/Modules.h"

#include "April/Utility/FileIO.h"

#include <cstddef>
#include <memory>
#include <tuple>

using namespace April;
using namespace std::chrono;


namespace {

	template<typename T>
	auto load_config()
	{
		auto const config = April::IO::from_file<T>( T::path );
		if ( config == std::nullopt )
			return T::LoadDefault();

		return *config;
	}

	template<auto N = 0, typename... Args>
	#pragma warning( suppress: 4100 )
	void update( std::tuple<Args...>& modules )
	{
		if constexpr ( N < sizeof...( Args ) )
		{
			std::get<N>( modules )->Update();
			update<N + 1>( modules );
		}
	}

	template<auto N = 0, typename... Args>
	#pragma warning( suppress: 4100 )
	void display( std::tuple<Args...>& modules )
	{
		if constexpr ( N < sizeof...( Args ) )
		{
			std::get<N>( modules )->Display();
			display<N + 1>( modules );
		}
	}

}


April::Modules::Modules( std::shared_ptr<UwTimes> uw_times )
	:
	config{
		{
			load_config<ConsumablesMgr::Config>(),
		},
		{
			load_config<AgentFilter::Config>(),
			load_config<ChatCommands::Config>(),
			load_config<ChatFilter::Config>(),
			load_config<NotifyEffectLoss::Config>(),
			load_config<ReturnToOutpost::Config>(),
			load_config<ShowKitUses::Config>(),
			load_config<SuppressSpeechBubbles::Config>(),
		},
		{
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
			load_config<Gui::UwTimesGui::Config>(),
		}
	},
	active{ 
		std::make_shared<ConsumablesMgr>( 
			std::get<ConsumablesMgr::Config>( config.active ) ),

		std::make_shared<ChainedSoul>(),

		std::make_shared<DhuumBot>(),

		std::make_shared<DhuumsJudgement>(),

		std::make_shared<UwTimer>( uw_times ) 
	},
	passive{
		std::make_unique<AgentFilter>( 
			std::get<AgentFilter::Config>( config.passive ) ),

		std::make_unique<ChatCommands>( 
			std::get<std::shared_ptr<ConsumablesMgr>>( active ),
			config,
			std::get<ChatCommands::Config>( config.passive ) ),
		
		std::make_unique<ChatFilter>( 
			std::get<ChatFilter::Config>( config.passive ) ),

		std::make_unique<CursorFix>(),

		std::make_unique<NotifyEffectLoss>( 
			std::get<NotifyEffectLoss::Config>( config.passive ) ),

		std::make_unique<ReturnToOutpost>( 
			std::get<ReturnToOutpost::Config>( config.passive ) ),

		std::make_unique<ShowKitUses>( 
			std::get<ShowKitUses::Config>( config.passive ) ),

		std::make_unique<SuppressSpeechBubbles>( 
			std::get<SuppressSpeechBubbles::Config>( config.passive ) )
	},
	gui{
		std::make_unique<Gui::ChainedSoulGui>( 
			std::get<std::shared_ptr<ChainedSoul>>( active ), 
			std::get<Gui::ChainedSoulGui::Config>( config.gui ) ),

		std::make_unique<Gui::Energybar>( 
			std::get<Gui::Energybar::Config>( config.gui ) ),

		std::make_unique<Gui::DhuumBotGui>( 
			std::get<std::shared_ptr<DhuumBot>>( active ), 
			std::get<Gui::DhuumBotGui::Config>( config.gui ) ),

		std::make_unique<Gui::DhuumInfo>( 
			std::get<std::shared_ptr<DhuumsJudgement>>( active ), 
			std::get<Gui::DhuumInfo::Config>( config.gui ) ),

		std::make_unique<Gui::Dialogs>( 
			std::get<Gui::Dialogs::Config>( config.gui ) ),

		std::make_unique<Gui::Healthbar>( 
			std::get<Gui::Healthbar::Config>( config.gui ) ),

		std::make_unique<Gui::InstanceTimer>( 
			std::get<Gui::InstanceTimer::Config>( config.gui ) ),

		std::make_unique<Gui::Inventory>( 
			std::get<std::shared_ptr<ConsumablesMgr>>( active ), 
			std::get<Gui::Inventory::Config>( config.gui ) ),

		std::make_unique<Gui::Settings>( config ),

		std::make_unique<Gui::Skillbar>( 
			std::get<Gui::Skillbar::Config>( config.gui ) ),

		std::make_unique<Gui::TargetInfo>( 
			std::get<Gui::TargetInfo::Config>( config.gui ) ),

		std::make_unique<Gui::UwTimesGui>( 
			uw_times, 
			std::get<Gui::UwTimesGui::Config>( config.gui ) )
	}
{
	static_assert( offsetof( Modules, config ) < offsetof( Modules, active ) );
	static_assert( offsetof( Modules, config ) < offsetof( Modules, passive ) );
	static_assert( offsetof( Modules, config ) < offsetof( Modules, gui ) );
}

void April::Modules::Update()
{
	update( active );
}

void April::Modules::Display()
{
	display( gui );
}

void April::Modules::Shutdown()
{
	std::get<std::unique_ptr<AgentFilter>>( passive )->DisplaySuppressedItems();
}
