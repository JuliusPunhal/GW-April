
#include "April/Framework/Modules.h"

#include <tuple>


namespace {

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
	active{ 
		std::make_shared<ConsumablesMgr>(),
		std::make_shared<ChainedSoul>(),
		std::make_shared<DhuumBot>(),
		std::make_shared<DhuumsJudgement>(),
		std::make_shared<UwTimer>( uw_times ) 
	},
	passive{
		std::make_unique<AgentFilter>(),
		std::make_unique<ChatCommands>(
			std::get<std::shared_ptr<ConsumablesMgr>>( active ) ),
		std::make_unique<ChatFilter>(),
		std::make_unique<CursorFix>(),
		std::make_unique<NotifyEffectLoss>(),
		std::make_unique<ReturnToOutpost>(),
		std::make_unique<ShowKitUses>(),
		std::make_unique<SuppressSpeechBubbles>() 
	},
	gui{
		std::make_unique<Gui::ChainedSoulGui>(
			std::get<std::shared_ptr<ChainedSoul>>( active ) ),
		std::make_unique<Gui::Energybar>(),
		std::make_unique<Gui::DhuumBotGui>(
			std::get<std::shared_ptr<DhuumBot>>( active ) ),
		std::make_unique<Gui::DhuumInfo>(
			std::get<std::shared_ptr<DhuumsJudgement>>( active ) ),
		std::make_unique<Gui::Dialogs>(),
		std::make_unique<Gui::Healthbar>(),
		std::make_unique<Gui::InstanceTimer>(),
		std::make_unique<Gui::Inventory>(
			std::get<std::shared_ptr<ConsumablesMgr>>( active ) ),
		std::make_unique<Gui::Settings>(),
		std::make_unique<Gui::Skillbar>(),
		std::make_unique<Gui::TargetInfo>(),
		std::make_unique<Gui::UwTimesGui>( uw_times ) 
	}
{
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
