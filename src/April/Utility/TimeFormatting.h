#pragma once

#include "GWCA/GWCA.hpp"

#include <string>


namespace April {

	auto to_string_ss( GW::ms32 ) -> std::string;
	auto to_string_sff( GW::ms32 ) -> std::string; // "0.00"
	auto to_string_mmss( GW::ms32 ) -> std::string;
	auto to_string_hmmss( GW::ms32 ) -> std::string;

	auto to_date_str( std::chrono::system_clock::time_point ) -> std::string;

}
