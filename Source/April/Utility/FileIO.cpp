
#include "April/Utility/FileIO.h"

#include <fstream>
#include <string>
#include <tuple>
#include <vector>

using namespace April;
using namespace April::Gui;
using namespace std::chrono;
using namespace std::string_literals;

using Abbreviation = ChatCommands::Config::Abbreviation;
using Notification = NotifyEffectLoss::Config::Notification;
using Dialog = Dialogs::Config::Dialog;


namespace {

	template<typename... Args>
	auto tup_to_string( Args&&... args ) -> std::string;


	auto val_to_string( bool const val )
	{
		return std::to_string( val );
	}

	auto val_to_string( int const val )
	{
		return std::to_string( val );
	}

	auto val_to_string( float const val )
	{
		return std::to_string( val );
	}

	auto val_to_string( uint32_t const id )
	{
		return std::to_string( id );
	}

	auto val_to_string( std::string const& str )
	{
		// TODO: Allow braces, quotes, etc to be stored
		return str;
	}

	auto val_to_string( milliseconds const& ms )
	{
		return std::to_string( ms.count() );
	}

	auto val_to_string( RGBA const& col )
	{
		return tup_to_string( col.r, col.g, col.b, col.a );
	}

	auto val_to_string( XY const& xy )
	{
		return tup_to_string( xy.x, xy.y );
	}

	auto val_to_string( WH const& wh )
	{
		return tup_to_string( wh.w, wh.h );
	}

	auto val_to_string( FontInfo const& font )
	{
		return tup_to_string( font.path, font.size );
	}

	auto val_to_string( Window const& wnd )
	{
		return tup_to_string( wnd.name, wnd.visible, wnd.flags );
	}

	auto val_to_string( GW::Rarity const rarity )
	{
		switch ( rarity )
		{
			case GW::Rarity::White: return "White";
			case GW::Rarity::Blue: return "Blue";
			case GW::Rarity::Purple: return "Purple";
			case GW::Rarity::Gold: return "Gold";
			case GW::Rarity::Green: return "Green";
			default: return "";
		}
	}

	auto val_to_string( Abbreviation const& abbrev )
	{
		return tup_to_string( abbrev.abbreviation, abbrev.original );
	}

	auto val_to_string( Notification const& notification )
		-> std::string
	{
		return tup_to_string(
			static_cast<int>( notification.skill_id ),
			notification.message );
	}

	auto val_to_string( Dialog const& dialog )
	{
		return tup_to_string( dialog.description, dialog.dialog );
	}

	auto val_to_string( Energybar::Config::Colors const& colors )
	{
		return tup_to_string( colors.standard );
	}

	auto val_to_string( Energybar::Config::PipsAlignment const& alignment )
	{
		return tup_to_string( alignment.left, alignment.right );
	}

	auto val_to_string( Healthbar::Config::Colors const& colors )
	{
		return tup_to_string(
			colors.standard,
			colors.degen_hexed,
			colors.deep_wounded,
			colors.bleeding,
			colors.poisoned );
	}

	auto val_to_string( Healthbar::Config::PipsAlignment const& alignment )
	{
		return tup_to_string( alignment.left, alignment.right );
	}

	auto val_to_string( Skillbar::Config::Threshold const& config )
	{
		return tup_to_string( config.time, config.color );
	}

	template<typename T>
	auto vec_to_string( T&& vec ) -> std::string;

	template<typename T>
	auto to_string( T&& val ) -> std::string
	{
		if constexpr ( stl::is_std_vector_v<stl::remove_cvref_t<T>> )
		{
			return vec_to_string( val );
		}
		else if constexpr ( stl::is_std_array_v<stl::remove_cvref_t<T>> )
		{
			return vec_to_string( val );
		}
		else return val_to_string( val );
	}

	template<typename T>
	auto vec_to_string( T&& vec ) -> std::string
	{
		if ( vec.size() == 0 )
			return std::string{ "[]" };

		auto result = std::string{ '[' };
		for ( auto it = vec.begin(); it < vec.end() - 1; ++it )
		{
			result += ::to_string( *it ) += ',';
		}
		result += ::to_string( vec.back() ) += ']';

		return result;
	}

	template<typename Arg>
	auto tup_to_string_impl( Arg&& arg )
	{
		return ::to_string( std::forward<Arg>( arg ) );
	}

	template<typename Arg, typename... Args>
	auto tup_to_string_impl( Arg&& arg, Args&&... args )
	{
		return ::to_string( std::forward<Arg>( arg ) ) + ','
			+ tup_to_string_impl( std::forward<Args>( args )... );
	}

	template<typename... Args>
	auto tup_to_string( Args&&... args ) -> std::string
	{
		static_assert( sizeof...( Args ) > 0 );

		return '{' + tup_to_string_impl( std::forward<Args>( args )... ) + '}';
	}

}


auto April::IO::to_string( AgentFilter::Config const& config ) -> std::string
{
	return tup_to_string(
		config.active,
		config.visible_user_items,
		config.visible_user_rarities,
		config.visible_party_items,
		config.visible_party_rarities );
}

auto April::IO::to_string( ChatCommands::Config const& config ) -> std::string
{
	return tup_to_string( config.abbreviations );
}

auto April::IO::to_string( ChatFilter::Config const& config ) -> std::string
{
	auto const* ptr = reinterpret_cast<bool const*>( &config );
	constexpr auto N = sizeof( ChatFilter::Config );

	auto result = std::string{ '{' };
	for ( auto it = 0; it < N - 1; ++it )
	{
		result += val_to_string( *(ptr + it) ) += ',';
	}

	result += val_to_string( *(ptr + N - 1) ) += '}';
	return result;
}

auto April::IO::to_string( ConsumablesMgr::Config const& config )
	-> std::string
{
	return tup_to_string( config.timeout );
}

auto April::IO::to_string( NotifyEffectLoss::Config const& config )
	-> std::string
{
	return tup_to_string( config.notifications );
}

auto April::IO::to_string( ReturnToOutpost::Config const& config )
	-> std::string
{
	return tup_to_string( config.active );
}

auto April::IO::to_string( ShowKitUses::Config const& config ) -> std::string
{
	return tup_to_string( config.active );
}

auto April::IO::to_string( SuppressSpeechBubbles::Config const& config )
	-> std::string
{
	return tup_to_string( config.active );
}

auto April::IO::to_string( ChainedSoulGui::Config const& config )
	-> std::string
{
	return tup_to_string( config.window );
}

auto April::IO::to_string( DhuumBotGui::Config const& config )
	-> std::string
{
	return tup_to_string( config.window );
}

auto April::IO::to_string( DhuumInfo::Config const& config )
	-> std::string
{
	return tup_to_string( config.window );
}

auto April::IO::to_string( Dialogs::Config const& config )
	-> std::string
{
	return tup_to_string(
		config.dialogs,
		config.confirm_time,
		config.confirm_color,
		config.window );
}

auto April::IO::to_string( Energybar::Config const& config )
	-> std::string
{
	return tup_to_string(
		config.border_color,
		config.background_color,
		config.colors,
		config.alignment,
		config.font,
		config.window );
}

auto April::IO::to_string( Healthbar::Config const& config )
	-> std::string
{
	return tup_to_string(
		config.border_color,
		config.background_color,
		config.colors,
		config.alignment,
		config.font,
		config.window );
}

auto April::IO::to_string( InstanceTimer::Config const& config )
	-> std::string
{
	return tup_to_string(
		config.font,
		config.text_color,
		config.shadow_color,
		config.shadow_offset,
		config.window );
}

auto April::IO::to_string( Inventory::Config const& config )
	-> std::string
{
	return tup_to_string(
		config.font,
		config.slot_size,
		config.item_spacing,
		config.border,
		config.button_alpha,
		config.no_item,
		config.unknown_item,
		config.wrong_instance_type,
		config.need_not_use,
		config.can_not_use,
		config.suggest_use,
		config.special_state,
		config.label_no_item,
		config.label_unknown_item,
		config.label_persistent,
		config.label_until_load,
		config.label_until_objective,
		config.label_inactive,
		config.window );
}

auto April::IO::to_string( Settings::Config const& config )
	-> std::string
{
	return tup_to_string( config.window );
}

auto April::IO::to_string( Skillbar::Config const& config )
	-> std::string
{
	return tup_to_string(
		config.font_cooldown,
		config.font_uptime,
		config.show_uptime,
		config.uptime_offset,
		config.text_color,
		config.hsr_color,
		config.border_color,
		config.spacing,
		config.skills_per_row,
		config.thresholds,
		config.color_inactive,
		config.window );
}

auto April::IO::to_string( TargetInfo::Config const& config )
	-> std::string
{
	return tup_to_string( config.window );
}

auto April::IO::to_string( UwTimesGui::Config const& config )
	-> std::string
{
	return tup_to_string(
		config.quest_names, config.column_names, config.window );
}


#include <assert.h>


namespace {

	template<typename T, typename... Args>
	auto t_from_string( std::string_view str ) -> std::optional<T>;

	template<typename T, typename... Args>
	auto t_from_string_unchecked( std::string_view str ) -> std::optional<T>;


	template<typename T>
	auto val_from_string( std::string_view ) -> std::optional<T>;

	template<> auto val_from_string( std::string_view str )
		-> std::optional<bool>
	{
		return str == "0" ? false : true;
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<char>
	{
		if ( not stl::stoi_can_parse( str ) )
			return std::nullopt;

		return static_cast<char>( std::stoi( std::string{ str } ) );
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<int>
	{
		if ( not stl::stoi_can_parse( str ) )
			return std::nullopt;

		return std::stoi( std::string{ str } );
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<float>
	{
		if ( not stl::stoi_can_parse( str ) )
			return std::nullopt;

		return std::stof( std::string{ str } );
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<uint32_t>
	{
		if ( not stl::stoi_can_parse( str ) )
			return std::nullopt;

		return std::stoul( std::string{ str } );
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<std::string>
	{
		return std::string{ str };
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<milliseconds>
	{
		if ( not stl::stoi_can_parse( str ) )
			return std::nullopt;

		return std::chrono::milliseconds{ std::stol( std::string{ str } ) };
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<RGBA>
	{
		return t_from_string<RGBA, float, float, float, float>( str );
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<XY>
	{
		return t_from_string<XY, float, float>( str );
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<WH>
	{
		return t_from_string<WH, float, float>( str );
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<FontInfo>
	{
		return t_from_string<FontInfo, std::string, int>( str );
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<Window>
	{
		return t_from_string<Window, std::string, bool, int>( str );
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<GW::Rarity>
	{
		if ( str == "White" )	return GW::Rarity::White;
		if ( str == "Blue" )	return GW::Rarity::Blue;
		if ( str == "Purple" )	return GW::Rarity::Purple;
		if ( str == "Gold" )	return GW::Rarity::Gold;
		if ( str == "Green" )	return GW::Rarity::Green;
		return std::nullopt;
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<GW::SkillID>
	{
		auto const id = val_from_string<int>( str );
		if ( id == std::nullopt )
			return std::nullopt;

		return static_cast<GW::SkillID>( *id );
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<Abbreviation>
	{
		return t_from_string<Abbreviation, std::string, std::string>( str );
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<Notification>
	{
		return t_from_string<Notification, GW::SkillID, std::string>( str );
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<Dialog>
	{
		return t_from_string<Dialog, std::string, int>( str );
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<Energybar::Config::Colors>
	{
		return t_from_string<Energybar::Config::Colors, RGBA>( str );
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<Energybar::Config::PipsAlignment>
	{
		return t_from_string<Energybar::Config::PipsAlignment, XY, XY>( str );
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<Healthbar::Config::Colors>
	{
		return t_from_string<
			Healthbar::Config::Colors, RGBA, RGBA, RGBA, RGBA, RGBA>( str );
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<Healthbar::Config::PipsAlignment>
	{
		return t_from_string<Healthbar::Config::PipsAlignment, XY, XY>( str );
	}

	template<> auto val_from_string( std::string_view str )
		-> std::optional<Skillbar::Config::Threshold>
	{
		return t_from_string<
			Skillbar::Config::Threshold, milliseconds, RGBA>( str );
	}

	template<typename Iter>
	auto find_closing_del( Iter bgn, Iter end )
		-> std::optional<std::string_view::iterator>
	{
		auto begin = bgn;
		auto count = 0;
		for ( /**/; begin != end; ++begin )
		{
			if ( *begin == '\\' )
				if ( ++begin == end )
					break;

			// TODO: Skip delimiters in quotes

			if ( *begin == '{' || *begin == '[' )
				++count;
			else if ( *begin == '}' || *begin == ']' )
				--count;
			else if ( count == 0 && *begin == ',' )
				return begin;
		}

		if ( count == 0 ) // no trailing comma but braces not mismatched
			return end;
		else
			return std::nullopt;
	}


	template<typename Iter>
	auto find_segments( Iter begin, Iter const end )
		-> std::optional<std::vector<std::string_view>>
	{
		auto result = std::vector<std::string_view>{};

		for ( /**/; /**/; /**/ )
		{
			auto const del = find_closing_del( begin, end );
			if ( del == std::nullopt )
				return std::nullopt;

			result.emplace_back( stl::make_sv( begin, *del ) );

			if ( *del == end )
				break;
			begin = *del + 1;
		}

		return result;
	}

	template<typename T>
	auto from_string_wrapper( std::string_view str_view ) -> std::optional<T>;

	template<typename T, std::enable_if_t<stl::is_std_vector_v<T>, int> = 0>
	auto vec_from_string( std::string_view str ) -> std::optional<T>
	{
		if ( str.size() < 2 || str.front() != '[' )
			return std::nullopt;

		if ( str == "[]" ) return T{};

		auto const vec_end = find_closing_del( str.begin(), str.end() );
		if ( vec_end == std::nullopt )
			return std::nullopt;

		auto segments = find_segments( str.begin() + 1, *vec_end - 1 );
		if ( segments == std::nullopt )
			return std::nullopt;

		auto result = T{};
		for ( auto const& segment : *segments )
		{
			auto val = from_string_wrapper<T::value_type>( segment );
			if ( val == std::nullopt )
				return std::nullopt;

			result.emplace_back( *std::move( val ) );
		}
		return result;
	}

	template<typename T, auto... I>
	auto tup_from_string_impl(
		std::vector<std::string_view> const& segments,
		std::index_sequence<I...> )
	{
		// TODO T -> stdtuple/array<std::optional>
		if constexpr ( stl::is_std_tuple_v<T> )
		{
			return std::tuple{
				from_string_wrapper<
					std::tuple_element_t<I, T>>(
						segments[I] )...
			};
		}
		else if constexpr ( stl::is_std_array_v<T> )
		{
			return std::array{
				from_string_wrapper<
					std::tuple_element_t<I, T>>(
						segments[I] )...
			};
		}
	}

	template<typename T>
	auto tup_from_string(
		std::string_view str, char const first, char const last )
		-> std::optional<T>
	{
		if ( str.size() < 2 || str.front() != first || str.back() != last )
			return std::nullopt;

		auto const segments = find_segments( str.begin() + 1, str.end() - 1 );
		if ( segments == std::nullopt
			|| segments->size() != std::tuple_size_v<T> )
		{
			return std::nullopt;
		}

		auto result =
			tup_from_string_impl<T>(
				*segments,
				std::make_index_sequence<std::tuple_size_v<T>>{} );

		auto is_empty = []( auto&& opt ) { return not opt.has_value(); };
		if ( stl::any_of_tuple( result, is_empty ) )
			return std::nullopt;

		auto unpack = []( auto&&... args )
		{
			return T{ *std::forward<decltype(args)>( args )... };
		};
		return std::apply( unpack, std::move( result ) );
	}

	template<typename T>
	auto from_string_wrapper( std::string_view str_view ) -> std::optional<T>
	{
		if constexpr ( stl::is_std_vector_v<T> )
		{
			return vec_from_string<T>( str_view );
		}
		else if constexpr ( stl::is_std_array_v<T> )
		{
			return tup_from_string<T>( str_view, '[', ']' );
		}
		else if constexpr ( stl::is_std_tuple_v<T> )
		{
			return tup_from_string<T>( str_view, '{', '}' );
		}
		else return val_from_string<T>( str_view );
	}

	template<typename T, typename... Args>
	auto t_from_string_unchecked( std::string_view str ) -> std::optional<T>
	{
		auto tup = from_string_wrapper<std::tuple<Args...>>( str );
		if ( tup == std::nullopt )
			return std::nullopt;

		return stl::init_from_tuple<T>( *std::move( tup ) );
	}

	template<typename T, typename... Args>
	auto t_from_string( std::string_view str ) -> std::optional<T>
	{
		// stl::init_from_tuple() does not guarantee to fail if T has more
		// fields than sizeof...( Args ).
		// TODO: Use std::make_from_tuple() to catch if T and Args... have
		// different layouts; however, this requires a user-defined ctor for T.
		static_assert( sizeof( T ) == sizeof( std::tuple<Args...> ) );

		return t_from_string_unchecked<T, Args...>( str );
	}

}


template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<AgentFilter::Config>
{
	return
		t_from_string<
			AgentFilter::Config,
			bool, std::vector<GW::ItemID>, std::vector<GW::Rarity>,
			std::vector<GW::ItemID>, std::vector<GW::Rarity>>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<ChatCommands::Config>
{
	return
		t_from_string<
			ChatCommands::Config,
			std::vector<ChatCommands::Config::Abbreviation>>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<ChatFilter::Config>
{
	constexpr auto x = sizeof( ChatFilter::Config );
	return
		t_from_string<
			ChatFilter::Config,
			bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool,
			bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool,
			bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool,
			bool, bool, bool, bool>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<ConsumablesMgr::Config>
{
	return t_from_string<ConsumablesMgr::Config, milliseconds>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<NotifyEffectLoss::Config>
{
	return
		t_from_string<
			NotifyEffectLoss::Config,
			std::vector<NotifyEffectLoss::Config::Notification>>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<ReturnToOutpost::Config>
{
	return t_from_string<ReturnToOutpost::Config, bool>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<ShowKitUses::Config>
{
	return t_from_string<ShowKitUses::Config, bool>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<SuppressSpeechBubbles::Config>
{
	return t_from_string<SuppressSpeechBubbles::Config, bool>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<ChainedSoulGui::Config>
{
	return t_from_string<ChainedSoulGui::Config, Window>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<DhuumBotGui::Config>
{
	return t_from_string<DhuumBotGui::Config, Window>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<DhuumInfo::Config>
{
	return t_from_string<DhuumInfo::Config, Window>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<Dialogs::Config>
{
	return
		t_from_string<
			Dialogs::Config,
			std::vector<Dialogs::Config::Dialog>, milliseconds, RGBA,
			Window>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<Energybar::Config>
{
	using Config = Energybar::Config;
	return
		t_from_string<
			Config,
			RGBA, RGBA, Config::Colors, Config::PipsAlignment, FontInfo,
			Window>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<Healthbar::Config>
{
	using Config = Healthbar::Config;
	return
		t_from_string<
			Config,
			RGBA, RGBA, Config::Colors, Config::PipsAlignment, FontInfo,
			Window>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<InstanceTimer::Config>
{
	return
		t_from_string<
			InstanceTimer::Config,
			FontInfo, RGBA, RGBA, XY, Window>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<Inventory::Config>
{
	return
		t_from_string_unchecked<
			Inventory::Config,
			FontInfo, WH, XY, bool, float, RGBA, RGBA, RGBA, RGBA, RGBA, RGBA,
			RGBA, char, char, char, char, char, char, Window>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<Settings::Config>
{
	return t_from_string<Settings::Config, Window>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<Skillbar::Config>
{
	using Config = Skillbar::Config;
	return
		t_from_string<
			Skillbar::Config,
			FontInfo, FontInfo, bool, XY, RGBA, RGBA, RGBA, XY, int,
			std::vector<Config::Threshold>, RGBA, Window>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<TargetInfo::Config>
{
	return t_from_string<TargetInfo::Config, Window>( str );
}

template<> auto April::IO::from_string( std::string_view str )
	-> std::optional<UwTimesGui::Config>
{
	return
		t_from_string<
			UwTimesGui::Config,
			std::array<std::string, 11>, std::array<std::string, 4>,
			Window>( str );
}

auto April::IO::read_file( const char* path ) -> std::string
{
	auto result = std::string{};
	if ( auto is = std::ifstream{ path }; is )
	{
		for ( auto line = std::string{}; std::getline( is, line ); /**/ )
		{
			result.append( std::move( line ) );
		}
	}
	return result;
}
