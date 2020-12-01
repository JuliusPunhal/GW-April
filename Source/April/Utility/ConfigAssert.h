#pragma once

#include <string_view>


namespace April::Config {

	constexpr bool ValidString( char const* str )
	{
		return str && str[0] != '\0';
	}

	template<typename Container_t>
	constexpr bool ValidStrings( Container_t const& container )
	{
		for ( auto const* str : container )
		{
			if ( not ValidString( str ) )
				return false;
		}
		return true;
	}
	
	constexpr bool IsFontPath( char const* path )
	{
		using namespace std::string_view_literals;

		auto const view = std::string_view( path );
		if ( view.size() <= 4 ) return false;

		return
			view[view.size() - 4] == '.'
			&& view[view.size() - 3] == 't'
			&& view[view.size() - 2] == 't'
			&& view[view.size() - 1] == 'f';
	}

}
