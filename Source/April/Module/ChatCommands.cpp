
#include "April/Module/ChatCommands.h"

#include "April/Framework/Initialization.h"
#include "April/Framework/Instance.h"
#include "April/Utility/stl.h"

#include "Dependencies/GWCA.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>

using namespace std::string_literals;
using namespace std::string_view_literals;

using April::AgentFilter;
using April::ChatCommands;
using April::ConsumablesMgr;
using Configs = April::Instance::Configuration;
using Config = April::ChatCommands::Config;


namespace {

	constexpr auto PARTY = GW::Chat::CHANNEL_GROUP;


	template<typename Iter_t>
	constexpr auto make_sv( Iter_t const begin, Iter_t const end )
	{
		auto const length = std::distance( begin, end );
		return std::string_view( &*begin, length );
	}

	constexpr auto is_word( char const ch )
	{
		// counting slash as part of word so the command needs to include it
		return ch != ' ' && ch != '"';
	}

	constexpr auto is_not_word( char const ch )
	{
		return not is_word( ch );
	}

	template<class Iter_t, typename T>
	auto find_unquoted( Iter_t const begin, Iter_t const end, T const val )
	{
		auto const val_start = std::find( begin, end, val );
		auto const quote_start = std::find( begin, end, '"' );

		if ( val_start == end || quote_start == end || val_start < quote_start )
			return val_start;

		auto const quote_end = std::find( quote_start + 1, end, '"' );
		if ( quote_end == end ) // mismatched quotes, treat rest as quote
			return end;

		return find_unquoted( quote_end + 1, end, val ); // recurse
	}

	template<class Iter_t, typename Pred>
	auto find_if_unquoted(
		Iter_t const begin, Iter_t const end, Pred const pred )
	{
		auto const val_start = std::find_if( begin, end, pred );
		auto const quote_start = std::find( begin, end, '"' );

		if ( val_start == end || val_start < quote_start || quote_start == end )
			return val_start;

		auto const quote_end = std::find( quote_start + 1, end, '"' );
		if ( quote_end == end ) // mismatched quotes, treat rest as quote
			return end;

		return find_if_unquoted( quote_end + 1, end, pred ); // recurse
	}

	template<class Container_t, typename Pred>
	auto find_if_unquoted( Container_t& container, Pred const pred )
	{
		return
			find_if_unquoted(
				std::begin( container ), std::end( container ), pred );
	}

	auto find_original( std::string_view const word, Config const& config )
		-> std::optional<std::string>
	{
		auto const iter =
			std::find_if(
				config.abbreviations,
				[word]( auto const& alias )
				{
					return alias.abbreviation == word;
				} );

		if ( iter == std::end( config.abbreviations ) )
			return std::nullopt;
		else
			return iter->original;
	}

	void expand_abbreviations( std::string& str, Config const& config )
	{
		for (
			auto word_begin = find_if_unquoted( str, is_word );
			word_begin != str.end();
			/**/ )
		{
			auto const word_end =
				find_if_unquoted( word_begin, str.end(), is_not_word );

			auto const abbrev = make_sv( word_begin, word_end );
			auto const original = find_original( abbrev, config );
			if ( original )
			{
				auto const word_pos = std::distance( str.begin(), word_begin );
				str.replace( word_begin, word_end, *original );
				word_begin = str.begin() + word_pos;
				continue;
			}

			word_begin = find_if_unquoted( word_end, str.end(), is_word );
		}
	}

	template<class Iter_t>
	auto stoi_can_parse( Iter_t const begin, Iter_t const end )
	{
		auto const first_char = std::find_if_not( begin, end, iswspace );
		return first_char != end && iswdigit( *first_char );
	}

	template<class Container_t>
	auto stoi_can_parse( Container_t const& container )
	{
		return stoi_can_parse( std::begin( container ), std::end( container ) );
	}

	template<class Iter_t>
	auto str_to_ids( Iter_t const begin, Iter_t const end )
		-> std::vector<GW::ItemID>
	{
		auto ids = std::vector<GW::ItemID>{};

		for (
			auto word_begin = std::find_if_not( begin, end, iswspace );
			word_begin != end;
			/**/ )
		{
			auto const word_end =
				std::find_if( word_begin, end, iswspace );

			if ( not stoi_can_parse( word_begin, word_end ) )
				return {};

			auto const id = stoi( std::string{ word_begin, word_end } );
			if ( id <= 0 )
				return {};

			ids.emplace_back( id );

			word_begin = std::find_if_not( word_end, end, iswspace );
		}

		return ids;
	}

	template<class Container_t>
	auto str_to_ids( Container_t const& container )
	{
		return str_to_ids( std::begin( container ), std::end( container ) );
	}

	struct cli {
		std::string_view cmd;
		std::string_view arguments;
		std::string_view message;
	};

	auto parse_cmd( std::string_view const message ) -> cli
	{
		auto const space_pos = message.find( L' ' );
		if ( space_pos == std::string::npos )
			return cli{ message, ""sv, message };

		return cli{
			std::string_view( message.data(), space_pos ),
			std::string_view(
				message.data() + space_pos + 1,
				message.size() - space_pos - 1 ),
			message
		};
	}

	void remove_quotes( std::string& str )
	{
		str.erase( std::remove( str.begin(), str.end(), '"' ), str.end() );
	}


	template<typename, typename = void>
	struct has_window : std::false_type {};

	template<typename T>
	struct has_window<T, std::void_t<decltype(std::declval<T>().window)>>
		: std::true_type {};


	template<typename T, auto... I>
	bool toggle_window_impl(
		std::string_view name, T&& tup, std::index_sequence<I...> )
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

		( try_toggle( std::get<I>( tup ) ), ... );

		return window_found;
	}

	template<typename T>
	bool toggle_window_by_name( std::string_view name, T&& tup )
	{
		return
			toggle_window_impl(
				name,
				std::forward<T>( tup ),
				std::make_index_sequence<
					std::tuple_size_v<stl::remove_cvref_t<T>>>{} );
	}


	bool call_command(
		cli const& cmd,
		AgentFilter& agent_filter,
		ConsumablesMgr& consumables,
		Configs& configs )
	{
		using namespace April;

		if ( cmd.cmd == ChatCommands::cmd_sendchat )
		{
			if ( cmd.arguments.size() > 0 )
			{
				auto str = std::string{ cmd.arguments };
				remove_quotes( str );
				GW::Chat::SendChat( static_cast<char>( str[0] ), &str[1] );
				return true;
			}
		}

		else if ( cmd.cmd == ChatCommands::cmd_writechat )
		{
			if ( cmd.arguments.size() > 0 )
			{
				auto str = std::string{ cmd.arguments };
				remove_quotes( str );
				GW::WriteChat( PARTY, str );
				return true;
			}
		}

		else if ( cmd.cmd == ChatCommands::cmd_pcons_on )
		{
			if ( cmd.arguments == "" || cmd.arguments == "off" )
			{
				consumables.deactivate_all_temporary();
				return true;
			}

			auto const ids = str_to_ids( cmd.arguments );
			if ( ids.empty() )
			{
				auto const error = "Could not parse all arguments in: "s;
				GW::WriteChat( PARTY, error + std::string{ cmd.message } );
				return true;
			}

			for ( auto const id : ids )
			{
				consumables.activate_temporary( id );
			}

			return true;
		}

		// TODO: CLI-Argument to specify persistence (--persistent?)
		else if ( cmd.cmd == ChatCommands::cmd_persistent_on )
		{
			if ( cmd.arguments == "" || cmd.arguments == "off" )
			{
				consumables.deactivate_all_persistent();
				return true;
			}

			auto const ids = str_to_ids( cmd.arguments );
			if ( ids.empty() )
			{
				auto const error = "Could not parse all arguments in: "s;
				GW::WriteChat( PARTY, error + std::string{ cmd.message } );
				return true;
			}

			for ( auto const id : ids )
			{
				consumables.activate_persistent( id );
			}

			return true;
		}

		else if ( cmd.cmd == ChatCommands::cmd_pcons_off )
		{
			if ( cmd.arguments == "" || cmd.arguments == "all" )
			{
				consumables.deactivate_all_temporary();
				return true;
			}

			auto const ids = str_to_ids( cmd.arguments );
			if ( ids.empty() )
			{
				auto const error = "Could not parse all arguments in: "s;
				GW::WriteChat( PARTY, error + std::string{ cmd.message } );
				return true;
			}

			for ( auto const& id : ids )
			{
				consumables.deactivate_temporary( id );
			}

			return true;
		}

		// TODO: CLI-Argument to specify persistence (--persistent?)
		else if ( cmd.cmd == ChatCommands::cmd_persistent_off )
		{
			if ( cmd.arguments == "" || cmd.arguments == "all" )
			{
				consumables.deactivate_all_persistent();
				return true;
			}

			auto const ids = str_to_ids( cmd.arguments );
			if ( ids.empty() )
			{
				auto const error = "Could not parse all arguments in: "s;
				GW::WriteChat( PARTY, error + std::string{ cmd.message } );
				return true;
			}

			for ( auto const& id : ids )
			{
				consumables.deactivate_persistent( id );
			}

			return true;
		}

		else if ( cmd.cmd == ChatCommands::cmd_pcons_objective )
		{
			if ( cmd.arguments == "" || cmd.arguments == "off" )
			{
				consumables.deactivating_quest = 0u;
				return true;
			}

			if ( not stoi_can_parse( cmd.arguments ) )
			{
				auto const error = "Could not parse argument in: "s;
				GW::WriteChat( PARTY, error + std::string{ cmd.message } );
				return true;
			}

			auto const obj = std::stoul( std::string{ cmd.arguments } );
			consumables.deactivating_quest = obj;
			return true;
		}

		else if ( cmd.cmd == ChatCommands::cmd_openxunlai )
		{
			if ( GW::GetInstanceType() != GW::InstanceType::Outpost )
			{
				GW::WriteChat(
					PARTY, "Xunlai Chest can only be opened in Outposts!" );
				return true;
			}

			auto const region = GW::Map::GetCurrentMapInfo()->region;
			if ( region == GW::Region::Region_Presearing )
			{
				GW::WriteChat(
					PARTY, "Xunlai Chest cannot be opened in Presearing!" );
				return true;
			}

			GW::GameThread::Enqueue( []() { GW::Items::OpenXunlaiWindow(); } );
			return true;
		}

		else if ( cmd.cmd == ChatCommands::cmd_toggle_gui )
		{
			if ( toggle_window_by_name( cmd.arguments, configs ) == false )
			{
				auto const window_name = std::string{ cmd.arguments };
				GW::WriteChat(
					PARTY, "Window \"" + window_name + "\" not found!" );
			}
			return true;
		}

		else if ( cmd.cmd == ChatCommands::cmd_show_suppressed )
		{
			auto const size = agent_filter.size();
			agent_filter.DisplaySuppressedItems();
			GW::WriteChat(
				PARTY, "Revealed " + std::to_string( size ) + " items." );

			return true;
		}

		else if ( cmd.cmd == ChatCommands::cmd_exit )
		{
			April::Die();
			return true;
		}

		return false;
	}

	auto msg_to_string( wchar_t const* message ) -> std::string
	{
		auto str = std::string{ '/' };
		str.reserve( 128 );

		for ( auto it = 0; it < 128; ++it )
		{
			if ( message[it] == '\0' )
				break;

			str += static_cast<char>( message[it] );
		}

		return str;
	}

}


void April::ChatCommands::OnMessage(
	GW::HookStatus* status,
	GW::Chat::Channel const channel,
	wchar_t const* raw_msg,
	AgentFilter& agent_filter,
	ConsumablesMgr& mgr,
	Instance& instance,
	Config const& config ) const
{
	if ( channel != GW::Chat::CHANNEL_COMMAND )
		return;

	auto msg = msg_to_string( raw_msg );
	expand_abbreviations( msg, config );

	for ( auto cmd_begin = msg.cbegin(); cmd_begin != msg.cend(); /**/ )
	{
		auto const cmd_end =
			find_unquoted( cmd_begin + 1, msg.cend(), '/' );

		auto cmd = make_sv( cmd_begin, cmd_end );
		while( cmd.size() > 0 && cmd.back() == ' ' )
			cmd.remove_suffix( 1 );

		auto const cli = parse_cmd( cmd );
		if ( call_command( cli, agent_filter, mgr, instance.config ) )
		{
			status->blocked = true;
		}

		cmd_begin = cmd_end;
	}
}

auto April::ChatCommands::Config::LoadDefault() -> Config
{
	namespace ModelID = GW::Constants::ItemID;

	return Config{
		std::vector<Abbreviation>{
			// Commands
			{ "/x",				cmd_openxunlai },
			{ "/s",				cmd_pcons_on },
			{ "/s_off",			cmd_pcons_off },
			{ "/sp",			cmd_persistent_on },
			{ "/sp_off",		cmd_persistent_off },
			{ "/q",				cmd_pcons_objective },
			// Consumables
			{ "cupcake",		std::to_string( ModelID::Cupcakes ) },
			{ "apple",			std::to_string( ModelID::Apples ) },
			{ "corn",			std::to_string( ModelID::Corns ) },
			{ "pie",			std::to_string( ModelID::Pies ) },
			{ "egg",			std::to_string( ModelID::Eggs ) },
			{ "warsupply",		std::to_string( ModelID::Warsupplies ) },
			{ "lunars",			std::to_string( ModelID::LunarRooster ) },
			{ "lunar",			"lunars" },
			{ "soup",			std::to_string( ModelID::SkalefinSoup ) },
			{ "kabob",			std::to_string( ModelID::Kabobs ) },
			{ "salad",			std::to_string( ModelID::PahnaiSalad ) },
			{ "bu",				std::to_string( ModelID::ConsEssence ) },
			{ "grail",			std::to_string( ModelID::ConsGrail ) },
			{ "armor",			std::to_string( ModelID::ConsArmor ) },
			{ "rrc",			std::to_string( ModelID::RRC ) },
			{ "brc",			std::to_string( ModelID::BRC ) },
			{ "grc",			std::to_string( ModelID::GRC ) },
			// Objectives
			{ "restore",		"147" },
			{ "escort",			"148" },
			{ "uwg",			"149" },
			{ "vale",			"150" },
			{ "wastes",			"151" },
			{ "pits",			"152" },
			{ "plains",			"153" },
			{ "mnts",			"154" },
			{ "pools",			"155" },
			{ "dhuum",			"157" },
			// Util
			{ "base",			"cupcake apple corn pie egg warsupply" },
			{ "cons",			"bu grail armor" },
			{ "miku",			std::to_string( ModelID::ELMiku ) },
			{ "ident",			std::to_string( ModelID::IdentKit_Superior ) },
			{ "perma",			"/sp miku ident" },
			{ "/t4",			"/s base lunars /q plains" }
		}
	};
}
