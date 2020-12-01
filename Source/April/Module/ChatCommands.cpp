
#include "April/Module/ChatCommands.h"

#include "April/Config/Module/ChatCommands.Config.hpp"
#include "April/Utility/stl.h"

#include "Dependencies/GWCA.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <variant>

namespace config = April::ChatCommandsConfig;

using namespace std::string_literals;
using namespace std::string_view_literals;

using April::ConsumablesMgr;


namespace {

	constexpr auto PARTY = GW::Chat::CHANNEL_GROUP;

	auto entry = GW::HookEntry{};


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

	auto find_original( std::string_view const word ) -> char const*
	{
		auto const iter = 
			std::find_if(
				config::abbreviations,
				[=]( config::Abbreviation const& alias )
				{
					return alias.abbreviation == word;
				} );

		return iter == std::end( config::abbreviations )
			? nullptr : iter->original;
	}

	void expand_abbreviations( std::string& str )
	{
		for ( 
			auto word_begin = find_if_unquoted( str, is_word );
			word_begin != str.end();
			/**/ )
		{
			auto const word_end =
				find_if_unquoted( word_begin, str.end(), is_not_word );
			
			auto const abbrev = make_sv( word_begin, word_end );
			auto const original = find_original( abbrev );
			if ( original )
			{
				auto const word_pos = std::distance( str.begin(), word_begin );
				str.replace( word_begin, word_end, original );
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

	bool call_command( 
		cli const& cmd, ConsumablesMgr& consumables )
	{
		using namespace April;

		if ( cmd.cmd == config::sendchat )
		{
			if ( cmd.arguments.size() > 0 )
			{
				auto str = std::string{ cmd.arguments };
				remove_quotes( str );
				GW::Chat::SendChat( static_cast<char>( str[0] ), &str[1] );
				return true;
			}
		}

		else if ( cmd.cmd == config::writechat )
		{
			if ( cmd.arguments.size() > 0 )
			{
				auto str = std::string{ cmd.arguments };
				remove_quotes( str );
				GW::WriteChat( PARTY, str );
				return true;
			}
		}

		else if ( cmd.cmd == config::activate_pcons )
		{
			if ( cmd.arguments == "" || cmd.arguments == "off" )
			{
				consumables.DeactivateAll( false );
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
				consumables.Activate( id, false );
			}

			return true;
		}
		
		// TODO: CLI-Argument to specify persistence (--persistent?)
		else if ( cmd.cmd == config::activate_persistent )
		{
			if ( cmd.arguments == "" || cmd.arguments == "off" )
			{
				consumables.DeactivateAll( true );
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
				consumables.Activate( id, true );
			}

			return true;
		}

		else if ( cmd.cmd == config::deactivate_pcons )
		{
			if ( cmd.arguments == "" || cmd.arguments == "all" )
			{
				consumables.DeactivateAll( false );
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
				consumables.Deactivate( id, false );
			}

			return true;
		}
		
		// TODO: CLI-Argument to specify persistence (--persistent?)
		else if ( cmd.cmd == config::deactivate_persistent )
		{
			if ( cmd.arguments == "" || cmd.arguments == "all" )
			{
				consumables.DeactivateAll( true );
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
				consumables.Deactivate( id, true );
			}

			return true;
		}

		else if ( cmd.cmd == config::set_deactivating_objective )
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

		else if ( cmd.cmd == config::openxunlai )
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

		return false;
	}

	void on_message(
		GW::HookStatus* status, 
		std::string msg, 
		ConsumablesMgr& consumables )
	{
		expand_abbreviations( msg );

		for ( auto cmd_begin = msg.cbegin(); cmd_begin != msg.cend(); /**/ )
		{
			auto const cmd_end = 
				find_unquoted( cmd_begin + 1, msg.cend(), '/' );

			auto cmd = make_sv( cmd_begin, cmd_end );
			while( cmd.size() > 0 && cmd.back() == ' ' )
				cmd.remove_suffix( 1 );

			auto const cli = parse_cmd( cmd );
			if ( call_command( cli, consumables ) )
			{
				status->blocked = true;
			}

			cmd_begin = cmd_end;
		}	
	}

}


April::ChatCommands::ChatCommands( std::shared_ptr<ConsumablesMgr> cons )
	: consumables{ std::move( cons ) }
{
	// Callbacks will only be cleaned up during GWCA shutdown.
	GW::Chat::RegisterSendChatCallback(
		&entry,
		[this]( GW::HookStatus* s, GW::Chat::Channel channel, wchar_t* msg )
		{ 
			if ( channel != GW::Chat::CHANNEL_COMMAND )
				return;

			char buf[128] = { '/' };
			for ( auto it = 1; it < sizeof( buf ); ++it )
			{
				buf[it] = static_cast<char>( msg[it - 1] );
			}
			buf[127] = '\0';

			on_message( s, buf, *consumables ); 
		} );
}
