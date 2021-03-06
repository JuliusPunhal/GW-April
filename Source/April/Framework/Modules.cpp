
#include "April/Framework/Modules.h"

#include "Dependencies/GWCA.hpp"

#include <memory>
#include <tuple>

using namespace April;
using namespace std::chrono;


namespace {

	auto entry = GW::HookEntry{};

}


April::Modules::Modules(
	Active&& active_,
	Passive&& passive_,
	Guis&& gui_,
	std::unique_ptr<ModuleConfigurations> config_ )
	:
	active{ std::move( active_ ) },
	passive{ std::move( passive_ ) },
	gui{ std::move( gui_ ) },
	config{ std::move( config_ ) }
{
	using namespace GW::Packet::StoC;

	// Callbacks will only be cleaned up during GWCA shutdown.
	GW::StoC::RegisterPacketCallback<AgentAdd>(
		&entry,
		[this]( auto* status, auto* packet )
		{
			std::get<AgentFilter>( passive ).OnSpawn( status, *packet );
		} );

	GW::StoC::RegisterPacketCallback<AgentName>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<UwTimer>( active ).Update( *packet );
		} );

	GW::StoC::RegisterPacketCallback<AgentRemove>(
		&entry,
		[this]( auto* status, auto* packet )
		{
			std::get<AgentFilter>( passive ).OnDespawn( status, *packet );
		} );

	GW::StoC::RegisterPacketCallback<AgentUpdateAllegiance>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<UwTimer>( active ).Update( *packet );
			std::get<WindowMgr>( active ).Update( *packet, *config );
		} );

	GW::StoC::RegisterPacketCallback<ItemGeneral>(
		&entry,
		[this] ( auto*, auto* packet )
		{
			std::get<ShowKitUses>( passive ).UpdateKitUses( *packet );
		} );

	GW::StoC::RegisterPacketCallback<ItemGeneral_ReuseID>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<AgentFilter>( passive ).DeleteOwner( *packet );
			std::get<ShowKitUses>( passive ).UpdateKitUses( *packet );
		} );

	GW::StoC::RegisterPacketCallback<MapLoaded>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<Gui::Skillbar>( gui ).UpdateHSR( *packet );
			std::get<AgentFilter>( passive ).Reset();
			std::get<ConsumablesMgr>( active ).Update( *packet );
			std::get<DhuumsJudgement>( active ).Update( *packet );
			std::get<UwTimer>( active ).Reset();
			std::get<WindowMgr>( active ).Update( *packet, *config );
		} );

	GW::StoC::RegisterPacketCallback<MessageGlobal>(
		&entry,
		[this] ( auto* status, auto* )
		{
			std::get<ChatFilter>( passive ).OnMessage( status );
		} );

	GW::StoC::RegisterPacketCallback<MessageLocal>(
		&entry,
		[this] ( auto* status, auto* )
		{
			std::get<ChatFilter>( passive ).OnMessage( status );
		} );

	GW::StoC::RegisterPacketCallback<MessageServer>(
		&entry,
		[this] ( auto* status, auto* )
		{
			std::get<ChatFilter>( passive ).OnMessage( status );
		} );

	GW::StoC::RegisterPacketCallback<ObjectiveAdd>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<UwTimer>( active ).Update( *packet );
		} );

	GW::StoC::RegisterPacketCallback<ObjectiveDone>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<ConsumablesMgr>( active ).Update( *packet );
			std::get<DhuumBot>( active ).Update( *packet );
			std::get<UwTimer>( active ).Update( *packet );
			std::get<WindowMgr>( active ).Update( *packet, *config );
		} );

	GW::StoC::RegisterPacketCallback<ObjectiveUpdateName>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<UwTimer>( active ).Update( *packet );
		} );

	GW::StoC::RegisterPacketCallback<PartyDefeated>(
		&entry,
		[this]( auto*, auto* )
		{
			std::get<ReturnToOutpost>( passive ).OnDefeated();
		} );

	GW::StoC::RegisterPacketCallback<RemoveEffect>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<NotifyEffectLoss>( passive ).OnEffectLoss( *packet );
		} );

	GW::StoC::RegisterPacketCallback<SkillRecharge>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<Gui::Skillbar>( gui ).UpdateHSR( *packet );
		} );

	GW::StoC::RegisterPacketCallback<SkillRecharged>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<Gui::Skillbar>( gui ).UpdateHSR( *packet );
		} );

	GW::StoC::RegisterPacketCallback<SpeechBubble>(
		&entry,
		[this]( auto* status, auto* )
		{
			std::get<SuppressSpeechBubbles>( passive ).Suppress( status );
		} );

	GW::StoC::RegisterPacketCallback<UpdateItemOwner>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<AgentFilter>( passive ).UpdateOwner( *packet );
		} );

	GW::Chat::RegisterLocalMessageCallback(
		&entry,
		[this]( GW::HookStatus* status, int, wchar_t const* msg )
		{
			std::get<ChatFilter>( passive ).OnMessage( status, msg );
		} );

	GW::Chat::RegisterSendChatCallback(
		&entry,
		[this](
			GW::HookStatus* status, GW::Chat::Channel channel, wchar_t* msg )
		{
			auto& chat_commands = std::get<ChatCommands>( passive );
			auto& agent_filter = std::get<AgentFilter>( passive );
			auto& mgr = std::get<ConsumablesMgr>( active );

			chat_commands.OnMessage(
				status, channel, msg, agent_filter, mgr, *config );
		} );
}

void April::Modules::Update()
{
	std::get<ConsumablesMgr>( active ).Update();
	std::get<ChainedSoul>( active ).Update();
	std::get<DhuumBot>( active ).Update();
	std::get<DhuumsJudgement>( active ).Update();
	std::get<UwTimer>( active ).Update();
	std::get<WindowMgr>( active ).Update( *config );
}

void April::Modules::Display()
{
	auto& chained_soul = std::get<ChainedSoul>( active );
	auto& dhuum_bot = std::get<DhuumBot>( active );
	auto& dhuums_judgement = std::get<DhuumsJudgement>( active );
	auto& cons = std::get<ConsumablesMgr>( active );
	auto& uw_timer = std::get<UwTimer>( active );

	std::get<Gui::ChainedSoulGui>( gui ).Display( chained_soul );
	std::get<Gui::Energybar>( gui ).Display();
	std::get<Gui::DhuumBotGui>( gui ).Display( dhuum_bot );
	std::get<Gui::DhuumInfo>( gui ).Display( dhuums_judgement );
	std::get<Gui::Dialogs>( gui ).Display();
	std::get<Gui::Healthbar>( gui ).Display();
	std::get<Gui::InstanceTimer>( gui ).Display();
	std::get<Gui::Inventory>( gui ).Display( cons );
	std::get<Gui::Settings>( gui ).Display( *config );
	std::get<Gui::Skillbar>( gui ).Display();
	std::get<Gui::TargetInfo>( gui ).Display();
	std::get<Gui::UwTimesGui>( gui ).Display( uw_timer );
}

void April::Modules::Shutdown()
{
	std::get<AgentFilter>( passive ).DisplaySuppressedItems();
}
