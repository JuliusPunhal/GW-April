
#include "April/Config/Module/ChatCommands.config.hpp"

#include "April/Utility/ConfigAssert.h"

using namespace April::Config;
using namespace April::ChatCommandsConfig;


namespace {

	inline constexpr auto valid_chars = std::string_view{
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 !\"§$%&/(){}[]?-_.:,;<>|+*~#'"
	};

	
	constexpr bool empty_abbreviations()
	{
		for ( auto const& abbrev : abbreviations )
		{
			if ( abbrev.abbreviation == nullptr ) return true;
			if ( abbrev.abbreviation[0] == '\0' ) return true;

			if ( abbrev.original == nullptr ) return true;
			if ( abbrev.original[0] == '\0' ) return true;
		}

		return false;
	}
				
	constexpr bool invalid_chars()
	{
		auto const is_valid = []( wchar_t const ch )
		{
			for ( auto const valid : valid_chars )
			{
				if ( ch == valid ) return true;
			}
			return false;
		};

		for ( auto const& abbrev : abbreviations )
		{
			auto const sv = std::string_view( abbrev.abbreviation );
			for ( auto const ch : sv )
			{
				if ( not is_valid( ch ) ) return true;
			}

			auto const sv2 = std::string_view( abbrev.original );
			for ( auto const ch : sv2 )
			{
				if ( not is_valid( ch ) ) return true;
			}
		}

		return false;
	}

	constexpr bool repeating_abbreviations()
	{
		auto const size = std::size( abbreviations );
		for ( auto it = 0u; it < size; ++it )
		{
			auto const* first = abbreviations[it].abbreviation;
			for ( auto it2 = it + 1; it2 < size; ++it2 )
			{
				auto const* second = abbreviations[it2].abbreviation;
				if ( first == second )
					return true;
			}
		}

		return false;
	}

	constexpr bool preceeding_or_trailings_spaces()
	{
		for ( auto const& abbrev : abbreviations )
		{
			auto const sv = std::string_view( abbrev.abbreviation );
			if ( sv.front() == ' ' ) return true;
			if ( sv.back() == ' ' ) return true;

			auto const sv2 = std::string_view( abbrev.original );
			if ( sv2.front() == ' ' ) return true;
			if ( sv2.back() == ' ' ) return true;
		}

		return false;
	}

}

static_assert( ValidString( sendchat ) );
static_assert( ValidString( writechat ) );
static_assert( ValidString( openxunlai ) );
static_assert( ValidString( activate_pcons ) );
static_assert( ValidString( deactivate_pcons ) );
static_assert( ValidString( activate_persistent ) );
static_assert( ValidString( deactivate_persistent ) );
static_assert( ValidString( set_deactivating_objective ) );

static_assert( empty_abbreviations() == false );
static_assert( invalid_chars() == false );
static_assert( repeating_abbreviations() == false );
static_assert( preceeding_or_trailings_spaces() == false );
