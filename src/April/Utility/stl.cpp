
#include "April/Utility/stl.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


auto stl::wstr_to_utf8( std::wstring const& wstr ) -> std::string
{
	auto const count =
		WideCharToMultiByte(
			CP_UTF8, 0, wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL );

	auto result = std::string( count, '\0' );

	WideCharToMultiByte(
		CP_UTF8, 0, wstr.c_str(), -1, result.data(), count, NULL, NULL );

	return result;
}

auto stl::utf8_to_wstr( std::string const& str ) -> std::wstring
{
	auto const count =
		MultiByteToWideChar( CP_UTF8, 0, str.c_str(), str.length(), NULL, 0 );

	auto result = std::wstring( count, L'\0' );

	MultiByteToWideChar(
		CP_UTF8, 0, str.c_str(), str.length(), result.data(), count );

	return result;
}

auto stl::wstr_to_ascii( std::wstring const& str ) -> std::string
{
	auto result = std::string{};
	result.reserve( str.size() );

	for ( auto const ch : str )
		result.push_back( ch > 0x7F ? '?' : static_cast<char>( ch ) );

	return result;
}
