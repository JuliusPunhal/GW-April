
#include "April/Framework/Modules.h"

#include "Dependencies/GWCA.hpp"

#include <memory>
#include <tuple>

using namespace April;
using namespace std::chrono;


namespace {

	auto entry = GW::HookEntry{};


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


	template <typename T, typename Tuple>
	struct has_type;

	template <typename T, typename... Us>
	struct has_type<T, std::tuple<Us...>>
		: std::disjunction<std::is_same<T, Us>...> {};

	template<typename Container, typename Lookup>
	constexpr auto has_type_v = has_type<Container, Lookup>::value;


	template<typename Lookup_t, typename Container_t>
	auto Get( Container_t& container ) -> auto&
	{
		if constexpr ( has_type_v<std::shared_ptr<Lookup_t>, Container_t> )
		{
			return *std::get<std::shared_ptr<Lookup_t>>( container );
		}
		else if constexpr ( has_type_v<std::unique_ptr<Lookup_t>, Container_t> )
		{
			return *std::get<std::unique_ptr<Lookup_t>>( container );
		}
		else static_assert( false, "Type not supported" );
	}

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
			Get<AgentFilter>( passive ).OnSpawn( status, *packet );
		} );

	GW::StoC::RegisterPacketCallback<AgentName>(
		&entry,
		[this]( auto*, auto* packet )
		{
			Get<UwTimer>( active ).Update( *packet );
		} );

	GW::StoC::RegisterPacketCallback<AgentRemove>(
		&entry,
		[this]( auto* status, auto* packet )
		{
			Get<AgentFilter>( passive ).OnDespawn( status, *packet );
		} );

	GW::StoC::RegisterPacketCallback<AgentUpdateAllegiance>(
		&entry,
		[this]( auto*, auto* packet )
		{
			Get<UwTimer>( active ).Update( *packet );
			Get<WindowMgr>( active ).Update( *packet );
		} );

	GW::StoC::RegisterPacketCallback<ItemGeneral>(
		&entry,
		[this] ( auto*, auto* packet )
		{
			Get<ShowKitUses>( passive ).UpdateKitUses( *packet );
		} );

	GW::StoC::RegisterPacketCallback<ItemGeneral_ReuseID>(
		&entry,
		[this]( auto*, auto* packet )
		{
			Get<AgentFilter>( passive ).DeleteOwner( *packet );
			Get<ShowKitUses>( passive ).UpdateKitUses( *packet );
		} );

	GW::StoC::RegisterPacketCallback<MapLoaded>(
		&entry,
		[this]( auto*, auto* packet )
		{
			Get<Gui::Skillbar>( gui ).UpdateHSR( *packet );
			Get<AgentFilter>( passive ).Reset();
			Get<ConsumablesMgr>( active ).Update( *packet );
			Get<DhuumsJudgement>( active ).Update( *packet );
			Get<UwTimer>( active ).Reset();
			Get<WindowMgr>( active ).Update( *packet );
		} );

	GW::StoC::RegisterPacketCallback<MessageGlobal>(
		&entry,
		[this] ( auto* status, auto* )
		{
			Get<ChatFilter>( passive ).OnMessage( status );
		} );

	GW::StoC::RegisterPacketCallback<MessageLocal>(
		&entry,
		[this] ( auto* status, auto* )
		{
			Get<ChatFilter>( passive ).OnMessage( status );
		} );

	GW::StoC::RegisterPacketCallback<MessageServer>(
		&entry,
		[this] ( auto* status, auto* )
		{
			Get<ChatFilter>( passive ).OnMessage( status );
		} );

	GW::StoC::RegisterPacketCallback<ObjectiveAdd>(
		&entry,
		[this]( auto*, auto* packet )
		{
			Get<UwTimer>( active ).Update( *packet );
		} );

	GW::StoC::RegisterPacketCallback<ObjectiveDone>(
		&entry,
		[this]( auto*, auto* packet )
		{
			Get<ConsumablesMgr>( active ).Update( *packet );
			Get<DhuumBot>( active ).Update( *packet );
			Get<UwTimer>( active ).Update( *packet );
			Get<WindowMgr>( active ).Update( *packet );
		} );

	GW::StoC::RegisterPacketCallback<ObjectiveUpdateName>(
		&entry,
		[this]( auto*, auto* packet )
		{
			Get<UwTimer>( active ).Update( *packet );
		} );

	GW::StoC::RegisterPacketCallback<PartyDefeated>(
		&entry,
		[this]( auto*, auto* )
		{
			Get<ReturnToOutpost>( passive ).OnDefeated();
		} );

	GW::StoC::RegisterPacketCallback<RemoveEffect>(
		&entry,
		[this]( auto*, auto* packet )
		{
			Get<NotifyEffectLoss>( passive ).OnEffectLoss( *packet );
		} );

	GW::StoC::RegisterPacketCallback<SkillRecharge>(
		&entry,
		[this]( auto*, auto* packet )
		{
			Get<Gui::Skillbar>( gui ).UpdateHSR( *packet );
		} );

	GW::StoC::RegisterPacketCallback<SkillRecharged>(
		&entry,
		[this]( auto*, auto* packet )
		{
			Get<Gui::Skillbar>( gui ).UpdateHSR( *packet );
		} );

	GW::StoC::RegisterPacketCallback<SpeechBubble>(
		&entry,
		[this]( auto* status, auto* )
		{
			Get<SuppressSpeechBubbles>( passive ).Suppress( status );
		} );

	GW::StoC::RegisterPacketCallback<UpdateItemOwner>(
		&entry,
		[this]( auto*, auto* packet )
		{
			Get<AgentFilter>( passive ).UpdateOwner( *packet );
		} );

	GW::Chat::RegisterLocalMessageCallback(
		&entry,
		[this]( GW::HookStatus* status, int, wchar_t const* msg )
		{
			Get<ChatFilter>( passive ).OnMessage( status, msg );
		} );

	GW::Chat::RegisterSendChatCallback(
		&entry,
		[this](
			GW::HookStatus* status, GW::Chat::Channel channel, wchar_t* msg )
		{
			Get<ChatCommands>( passive ).OnMessage( status, channel, msg );
		} );
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
	std::get<std::shared_ptr<AgentFilter>>( passive )->DisplaySuppressedItems();
}
