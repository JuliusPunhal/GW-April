#pragma once


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

}
