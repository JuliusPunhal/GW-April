#pragma once

#include <chrono>
#include <string>
#include <tuple>
#include <type_traits>


namespace April {

	template<typename Precision_t>
	struct TimeFormat {
		using Precision = Precision_t;
		const char* fmt;
	};

	inline constexpr auto HMMSS =
		std::tuple{ 
			TimeFormat<std::chrono::hours>{ "%01d:" }, 
			TimeFormat<std::chrono::minutes>{ "%02d:" }, 
			TimeFormat<std::chrono::seconds>{ "%02d" } 
		};

	inline constexpr auto MMSS =
		std::tuple{ 
			TimeFormat<std::chrono::minutes>{ "%02d:" }, 
			TimeFormat<std::chrono::seconds>{ "%02d" } 
		};


	template<class Time_t, class FormatTime_t>
	auto to_string( Time_t const time, TimeFormat<FormatTime_t> const format ) 
		-> std::string
	{
		char buf[16] = {};

		snprintf( 
			buf, sizeof( buf ), 
			format.fmt, 
			std::chrono::duration_cast<FormatTime_t>(time).count() );

		return std::string( buf );
	}

	template<class Time_t, class LowPrec_t, class HighPrec_t, class... Params>
	auto to_string(
		Time_t const time, 
		TimeFormat<LowPrec_t> const low, 
		TimeFormat<HighPrec_t> const high, 
		Params const... params )
		-> std::string
	{
		static_assert(
			!std::is_convertible_v<HighPrec_t, LowPrec_t>,
			"Format must be ordered from least to most precise." );

		auto const remaining_time =
			time - std::chrono::duration_cast<LowPrec_t>(time);

		return
			to_string( time, low ) +
			to_string( remaining_time, high, params... );
	}

	template<class Time_t, class... Params>
	auto to_string( Time_t const time, std::tuple<TimeFormat<Params>...> const fmt )
		-> std::string
	{
		auto const expand = [time]( TimeFormat<Params> const&... params )
		{
			return to_string( time, params... );
		};

		return std::apply( expand, fmt );
	}

}
