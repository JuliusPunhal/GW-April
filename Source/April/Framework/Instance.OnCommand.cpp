
#include "April/Framework/Instance.OnCommand.hpp"

#include <assert.h>

using namespace std::string_literals;


namespace {

	template<typename, typename = void>
	struct has_window : std::false_type {};

	template<typename T>
	struct has_window<T, std::void_t<decltype(std::declval<T>().window)>>
		: std::true_type {};


	template<auto... I>
	bool toggle_window_impl(
		std::string_view name,
		April::Configuration& configs,
		std::index_sequence<I...> )
	{
		auto window_found = false;
		auto try_toggle = [&window_found, name]( auto& gui )
		{
			using T = stl::remove_cvref_t<decltype(gui)>;
			if constexpr ( has_window<T>::value )
			{
				if ( gui.window.name == name )
				{
					gui.window.visible = !gui.window.visible;
					window_found = true;
				}
			}
		};

		( try_toggle( std::get<I>( configs ) ), ... );

		return window_found;
	}

	bool toggle_window_by_name(
		std::string_view name, April::Configuration& configs )
	{
		constexpr auto size = std::tuple_size_v<April::Configuration>;

		return
			toggle_window_impl(
				name, configs, std::make_index_sequence<size>{} );
	}

}


April::OnCommand::OnCommand( Instance& instance )
	: instance{ instance }
{
}

void April::OnCommand::operator()( SendChat const& cmd ) const
{
	GW::SendChat( cmd.channel, cmd.message );
}

void April::OnCommand::operator()( WriteChat const& cmd ) const
{
	GW::WriteChat( cmd.channel, cmd.message );
}

void April::OnCommand::operator()( SendDialog const& cmd ) const
{
	GW::Agents::SendDialog( cmd.dialog );
}

void April::OnCommand::operator()( UseConsumable const& cmd ) const
{
	assert( cmd.item != nullptr );

	auto const visitor = std::overloaded{
		[&]( Trivial const& )			{ return GW::UseItem( *cmd.item ); },
		[&]( RockCandy const& )			{ return GW::UseItem( *cmd.item ); },
		[&]( Conset const& )			{ return GW::UseItem( *cmd.item ); },
		[&]( LunarFortune const& )		{ return GW::UseItem( *cmd.item ); },
		[&]( Alcohol const& )			{ return GW::UseItem( *cmd.item ); },
		[&]( SummoningStone const& )	{ return GW::UseItem( *cmd.item ); },
		[&]( IdentKit const& )
		{
			auto const* unident_item = GW::FindUnidentGold();
			if ( unident_item )
				GW::IdentifyItem( *unident_item, *cmd.item );
		},
		[&]( MoraleBooster const& )		{ return GW::UseItem( *cmd.item ); },
		[&]( Tonic const& )				{ return GW::UseItem( *cmd.item ); }
	};

	return std::visit( visitor, cmd.consumable );
}

void April::OnCommand::operator()( UseSkill const& cmd ) const
{
	assert( cmd.slot > 0 && cmd.slot < 8 );
	GW::UseSkill( cmd.slot, cmd.target ? cmd.target->agent_id : 0 );
}

void April::OnCommand::operator()( Return_to_Outpost const& ) const
{
	GW::CtoS::SendPacket( 0x4, GAME_CMSG_PARTY_RETURN_TO_OUTPOST );
}

void April::OnCommand::operator()( EnableConsumables const& cmd ) const
{
	auto& cons_mgr = std::get<ConsumablesMgr>( instance.modules );

	if ( cmd.persistent )
	{
		for ( auto const id : cmd.consumables )
		{
			cons_mgr.activate_persistent( id );
		}
	}
	else
	{
		for ( auto const id : cmd.consumables )
		{
			cons_mgr.activate_temporary( id );
		}
	}
}

void April::OnCommand::operator()( DisableConsumables const& cmd ) const
{
	auto& cons_mgr = std::get<ConsumablesMgr>( instance.modules );

	if ( cmd.persistent )
	{
		for ( auto const id : cmd.consumables )
		{
			cons_mgr.deactivate_persistent( id );
		}
	}
	else
	{
		for ( auto const id : cmd.consumables )
		{
			cons_mgr.deactivate_temporary( id );
		}
	}
}

void April::OnCommand::operator()( DisableAllConsumables const& cmd ) const
{
	auto& cons_mgr = std::get<ConsumablesMgr>( instance.modules );

	if ( cmd.persistent )
	{
		cons_mgr.deactivate_all_persistent();
	}
	else
	{
		cons_mgr.deactivate_all_temporary();
	}
}

void April::OnCommand::operator()( SetConsumablesObjective const& cmd ) const
{
	auto& cons_mgr = std::get<ConsumablesMgr>( instance.modules );
	cons_mgr.deactivating_quest = cmd.objective;
}

void April::OnCommand::operator()( OpenXunlai const& ) const
{
	GW::GameThread::Enqueue( [](){ GW::Items::OpenXunlaiWindow(); } );
}

void April::OnCommand::operator()( ToggleGui const& cmd ) const
{
	auto found = toggle_window_by_name( cmd.window_name, instance.config );
	if ( not found )
	{
		GW::WriteChat(
			GW::Chat::CHANNEL_GROUP,
			"Window \"" + cmd.window_name + "\" not found!" );
	}
}

void April::OnCommand::operator()( SetUwGuiVisibility const& cmd ) const
{
	auto& dhuum_bot = std::get<Gui::DhuumBotGui::Config>( instance.config );
	auto& dhuum_info = std::get<Gui::DhuumInfo::Config>( instance.config );
	auto& timer = std::get<Gui::InstanceTimer::Config>( instance.config );
	auto& soul = std::get<Gui::ChainedSoulGui::Config>( instance.config );

	switch ( cmd.gui )
	{
		case SetUwGuiVisibility::Type::Dhuum:
		{
			dhuum_bot.window.visible =
				cmd.visiblity ? *cmd.visiblity : !dhuum_bot.window.visible;
			dhuum_info.window.visible =
				cmd.visiblity ? *cmd.visiblity : !dhuum_info.window.visible;
			break;
		}
		case SetUwGuiVisibility::Type::General:
		{
			timer.window.visible =
				cmd.visiblity ? *cmd.visiblity : !dhuum_info.window.visible;
			break;
		}
		case SetUwGuiVisibility::Type::Pits:
		{
			soul.window.visible =
				cmd.visiblity ? *cmd.visiblity : !dhuum_info.window.visible;
			break;
		}
	}
}

void April::OnCommand::operator()( ShowSuppresedAgents const& ) const
{
	auto& agent_filter = std::get<AgentFilter>( instance.modules );

	auto const msg =
		agent_filter.size() == 1
			? "Releaving 1 item."s
			: "Revealing "s + std::to_string( agent_filter.size() ) + " items.";

	GW::WriteChat( GW::Chat::CHANNEL_GROUP, msg );

	for ( auto const& packet : agent_filter.get_suppressed() )
	{
		GW::GameThread::Enqueue(
			[copy = packet]() mutable
			{
				GW::StoC::EmulatePacket( &copy );
			} );
	}
	agent_filter.clear();
}

void April::OnCommand::operator()( NoCommand_t const& ) const
{
	// Nothing to do.
	// Many Modules/Guis only want to send Commands conditionally, this is a
	// dummy to avoid them using std::optional<Command> everywhere.
}

void April::OnCommand::operator()( Exit const& ) const
{
	instance.terminate = true;
}
