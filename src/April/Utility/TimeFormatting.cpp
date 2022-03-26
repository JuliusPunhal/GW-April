
#include "April/Utility/TimeFormatting.h"

#include <array>

using namespace GW::literals;
using namespace std::chrono;
using sec32 = duration<long>;


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
