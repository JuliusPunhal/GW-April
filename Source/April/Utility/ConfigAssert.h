#pragma once


namespace April::Config {

	constexpr bool ValidString( char const* str )
	{
		return str && str[0] != '\0';
	}

}
