
#include "Dependencies/GWCA.hpp"

using namespace std::chrono;


auto GW::GetInstanceTime() -> milliseconds
{
	return milliseconds{ GW::Map::GetInstanceTime() };
}

auto GW::GetRecharge( GW::SkillbarSkill const& skill )  -> milliseconds
{
	// cast to signed to allow for negative times
	auto const recharge = static_cast<long>( skill.GetRecharge() );
	return milliseconds{ recharge };
}

auto GW::GetTimeRemaining( GW::Effect const& effect ) -> milliseconds
{
	// cast to signed to allow for negative times
	auto const uptime = static_cast<long>( effect.GetTimeRemaining() );
	return milliseconds{ uptime };
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
