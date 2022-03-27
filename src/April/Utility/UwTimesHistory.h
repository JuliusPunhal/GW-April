#pragma once

#include "GWCA/GWCA.hpp"

#include <array>
#include <chrono>
#include <vector>


namespace April {

	struct UwTimes {
		std::chrono::system_clock::time_point date_time;

		GW::InstanceTime instance_time{};

		std::array<GW::InstanceTime, 7> pop{};
		std::array<GW::InstanceTime, 10> take{};
		std::array<GW::InstanceTime, 10> done{};

		GW::InstanceTime nightman_cometh{};
		GW::InstanceTime dhuum_hostile{};
		GW::InstanceTime dhuum_done{};


		UwTimes( std::chrono::system_clock::time_point const& );
	};


	using UwTimesHistory = std::vector<UwTimes>;

}
