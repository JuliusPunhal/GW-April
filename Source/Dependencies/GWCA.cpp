
#include "Dependencies/GWCA.hpp"

using namespace std::chrono;


auto GW::GetInstanceTime() -> milliseconds
{
	return milliseconds{ GW::Map::GetInstanceTime() };
}

void GW::SendChat( char const channel, const char* str )
{
	return GW::Chat::SendChat( channel, str );
}

void GW::SendChat( char const channel, const wchar_t* str )
{
	return GW::Chat::SendChat( channel, str );
}

void GW::SendChat( char const channel, std::string const& str )
{
	return GW::Chat::SendChat( channel, str.c_str() );
}

void GW::SendChat( char const channel, std::wstring const& str )
{
	return GW::Chat::SendChat( channel, str.c_str() );
}
