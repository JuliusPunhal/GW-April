
#include "April/Utility/TimeFormatting.h"

#include <array>
#include <iomanip>
#include <sstream>
#include <time.h>

using namespace GW::literals;
using namespace std::chrono;
using sec32 = duration<long>;
using sec32f = duration<float>;


auto April::to_string_ss( GW::ms32 const ms ) -> std::string
{
	auto const time = abs( ms );
	auto const s = duration_cast<sec32>( time );

	auto buf = std::array<char, 16>{};
	snprintf(
		buf.data(), buf.size(),
		ms < 0_ms ? "-%02d" : "%02d",
		s.count() );

	return std::string{ buf.data() };
}

auto April::to_string_sff( GW::ms32 const ms ) -> std::string
{
	auto const time = abs( ms );
	auto const s = duration_cast<sec32f>( time );

	auto buf = std::array<char, 16>{};
	snprintf(
		buf.data(), buf.size(),
		ms < 0_ms ? "-%.2f" : "%.2f",
		s.count() );

	return std::string{ buf.data() };
}

auto April::to_string_mmss( GW::ms32 const ms ) -> std::string
{
	auto const time = abs( ms );

	auto const m = duration_cast<minutes>( time );
	auto const s = duration_cast<sec32>( time - m );

	auto buf = std::array<char, 16>{};
	snprintf(
		buf.data(), buf.size(),
		ms < 0_ms ? "-%02d:%02d" : "%02d:%02d",
		m.count(), s.count() );

	return std::string{ buf.data() };
}

auto April::to_string_hmmss( GW::ms32 const ms ) -> std::string
{
	auto const time = abs( ms );

	auto const h = duration_cast<hours>( time );
	auto const m = duration_cast<minutes>( time - h );
	auto const s = duration_cast<sec32>( time - h - m );

	auto buf = std::array<char, 16>{};
	snprintf(
		buf.data(), buf.size(),
		ms < 0_ms ? "-%01d:%02d:%02d" : "%01d:%02d:%02d",
		h.count(), m.count(), s.count() );

	return std::string{ buf.data() };
}

auto April::to_date_str( system_clock::time_point const date_time )
	-> std::string
{
	auto time_buf = std::tm{};
	auto const to_time_t = system_clock::to_time_t( date_time );
	localtime_s( &time_buf, &to_time_t );

	auto ss = std::stringstream{};
	ss << std::put_time( &time_buf, "%Y-%m-%d %X" );

	return ss.str();
}
