
#include "April/Framework/Instance.h"

#include "Dependencies/GWCA.hpp"

#include <memory>
#include <tuple>

using namespace April;
using namespace std::chrono;


namespace {

	auto entry = GW::HookEntry{};

}


April::Instance::Instance( Modules&& modules_, ModuleConfigurations&& config_ )
	: modules{ std::move( modules_ ) }, config{ std::move( config_ ) }
{
	using namespace GW::Packet::StoC;

	// Callbacks will only be cleaned up during GWCA shutdown.
	GW::StoC::RegisterPacketCallback<AgentAdd>(
		&entry,
		[this]( auto* status, auto* packet )
		{
			std::get<AgentFilter>( modules ).OnSpawn(
				status,
				*packet,
				std::get<AgentFilter::Config>( config.passive ) );
		} );

	GW::StoC::RegisterPacketCallback<AgentName>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<UwTimer>( modules ).Update( *packet );
		} );

	GW::StoC::RegisterPacketCallback<AgentRemove>(
		&entry,
		[this]( auto* status, auto* packet )
		{
			std::get<AgentFilter>( modules ).OnDespawn( status, *packet );
		} );

	GW::StoC::RegisterPacketCallback<AgentUpdateAllegiance>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<UwTimer>( modules ).Update( *packet );
			std::get<WindowMgr>( modules ).Update( *packet, config );
		} );

	GW::StoC::RegisterPacketCallback<ItemGeneral>(
		&entry,
		[this] ( auto*, auto* packet )
		{
			std::get<ShowKitUses>( modules ).UpdateKitUses(
				*packet, std::get<ShowKitUses::Config>( config.passive ) );
		} );

	GW::StoC::RegisterPacketCallback<ItemGeneral_ReuseID>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<AgentFilter>( modules ).DeleteOwner( *packet );
			std::get<ShowKitUses>( modules ).UpdateKitUses(
				*packet, std::get<ShowKitUses::Config>( config.passive ) );
		} );

	GW::StoC::RegisterPacketCallback<MapLoaded>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<Gui::Skillbar>( modules ).UpdateHSR( *packet );
			std::get<AgentFilter>( modules ).Reset();
			std::get<ConsumablesMgr>( modules ).Update( *packet );
			std::get<DhuumsJudgement>( modules ).Update( *packet );
			std::get<UwTimer>( modules ).Reset();
			std::get<WindowMgr>( modules ).Update( *packet, config );
		} );

	GW::StoC::RegisterPacketCallback<MessageGlobal>(
		&entry,
		[this] ( auto* status, auto* )
		{
			std::get<ChatFilter>( modules ).OnMessage(
				status, std::get<ChatFilter::Config>( config.passive ) );
		} );

	GW::StoC::RegisterPacketCallback<MessageLocal>(
		&entry,
		[this] ( auto* status, auto* )
		{
			std::get<ChatFilter>( modules ).OnMessage(
				status, std::get<ChatFilter::Config>( config.passive ) );
		} );

	GW::StoC::RegisterPacketCallback<MessageServer>(
		&entry,
		[this] ( auto* status, auto* )
		{
			std::get<ChatFilter>( modules ).OnMessage(
				status, std::get<ChatFilter::Config>( config.passive ) );
		} );

	GW::StoC::RegisterPacketCallback<ObjectiveAdd>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<UwTimer>( modules ).Update( *packet );
		} );

	GW::StoC::RegisterPacketCallback<ObjectiveDone>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<ConsumablesMgr>( modules ).Update( *packet );
			std::get<DhuumBot>( modules ).Update( *packet );
			std::get<UwTimer>( modules ).Update( *packet );
			std::get<WindowMgr>( modules ).Update( *packet, config );
		} );

	GW::StoC::RegisterPacketCallback<ObjectiveUpdateName>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<UwTimer>( modules ).Update( *packet );
		} );

	GW::StoC::RegisterPacketCallback<PartyDefeated>(
		&entry,
		[this]( auto*, auto* )
		{
			std::get<ReturnToOutpost>( modules ).OnDefeated(
				std::get<ReturnToOutpost::Config>( config.passive ) );
		} );

	GW::StoC::RegisterPacketCallback<RemoveEffect>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<NotifyEffectLoss>( modules ).OnEffectLoss(
				*packet,
				std::get<NotifyEffectLoss::Config>( config.passive ) );
		} );

	GW::StoC::RegisterPacketCallback<SkillRecharge>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<Gui::Skillbar>( modules ).UpdateHSR( *packet );
		} );

	GW::StoC::RegisterPacketCallback<SkillRecharged>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<Gui::Skillbar>( modules ).UpdateHSR( *packet );
		} );

	GW::StoC::RegisterPacketCallback<SpeechBubble>(
		&entry,
		[this]( auto* status, auto* )
		{
			std::get<SuppressSpeechBubbles>( modules ).Suppress(
				status,
				std::get<SuppressSpeechBubbles::Config>( config.passive ) );
		} );

	GW::StoC::RegisterPacketCallback<UpdateItemOwner>(
		&entry,
		[this]( auto*, auto* packet )
		{
			std::get<AgentFilter>( modules ).UpdateOwner( *packet );
		} );

	GW::Chat::RegisterLocalMessageCallback(
		&entry,
		[this]( GW::HookStatus* status, int, wchar_t const* msg )
		{
			std::get<ChatFilter>( modules ).OnMessage(
				status, msg, std::get<ChatFilter::Config>( config.passive ) );
		} );

	GW::Chat::RegisterSendChatCallback(
		&entry,
		[this](
			GW::HookStatus* status, GW::Chat::Channel channel, wchar_t* msg )
		{
			auto& chat_commands = std::get<ChatCommands>( modules );
			auto& agent_filter = std::get<AgentFilter>( modules );
			auto& mgr = std::get<ConsumablesMgr>( modules );
			auto& cfg = std::get<ChatCommands::Config>( config.passive );

			chat_commands.OnMessage(
				status, channel, msg, agent_filter, mgr, config, cfg );
		} );
}

void April::Instance::Update()
{
	std::get<ConsumablesMgr>( modules ).Update(
		std::get<ConsumablesMgr::Config>( config.active ) );
	std::get<ChainedSoul>( modules ).Update();
	std::get<DhuumBot>( modules ).Update();
	std::get<DhuumsJudgement>( modules ).Update();
	std::get<UwTimer>( modules ).Update();
	std::get<WindowMgr>( modules ).Update( config );
}

void April::Instance::Display()
{
	auto& chained_soul = std::get<ChainedSoul>( modules );
	auto& dhuum_bot = std::get<DhuumBot>( modules );
	auto& dhuums_judgement = std::get<DhuumsJudgement>( modules );
	auto& cons = std::get<ConsumablesMgr>( modules );
	auto& uw_timer = std::get<UwTimer>( modules );

	std::get<Gui::ChainedSoulGui>( modules ).Display(
		chained_soul, std::get<Gui::ChainedSoulGui::Config>( config.gui ) );
	std::get<Gui::Energybar>( modules ).Display(
		std::get<Gui::Energybar::Config>( config.gui ) );
	std::get<Gui::DhuumBotGui>( modules ).Display(
		dhuum_bot, std::get<Gui::DhuumBotGui::Config>( config.gui ) );
	std::get<Gui::DhuumInfo>( modules ).Display(
		dhuums_judgement, std::get<Gui::DhuumInfo::Config>( config.gui ) );
	std::get<Gui::Dialogs>( modules ).Display(
		std::get<Gui::Dialogs::Config>( config.gui ) );
	std::get<Gui::Healthbar>( modules ).Display(
		std::get<Gui::Healthbar::Config>( config.gui ) );
	std::get<Gui::InstanceTimer>( modules ).Display(
		std::get<Gui::InstanceTimer::Config>( config.gui ) );
	std::get<Gui::Inventory>( modules ).Display(
		cons, std::get<Gui::Inventory::Config>( config.gui ) );
	std::get<Gui::Settings>( modules ).Display( config );
	std::get<Gui::Skillbar>( modules ).Display(
		std::get<Gui::Skillbar::Config>( config.gui ) );
	std::get<Gui::TargetInfo>( modules ).Display(
		std::get<Gui::TargetInfo::Config>( config.gui ) );
	std::get<Gui::UwTimesGui>( modules ).Display(
		uw_timer, std::get<Gui::UwTimesGui::Config>( config.gui ) );
}

void April::Instance::Shutdown()
{
	std::get<AgentFilter>( modules ).DisplaySuppressedItems();
}
