#pragma once

#include "Dependencies/GWCA.hpp"

#include <array>
#include <chrono>


namespace April {

	struct UwTimes {
		std::chrono::milliseconds instance_time{};

		std::array<std::chrono::milliseconds, 7> pop{};
		std::array<std::chrono::milliseconds, 10> take{};
		std::array<std::chrono::milliseconds, 10> done{};

		std::chrono::milliseconds nightman_cometh{};
		std::chrono::milliseconds dhuum_hostile{};
		std::chrono::milliseconds dhuum_done{};
	};

	class UwTimer {
	public:
		UwTimer() = default;

		void Update();
		void Update( GW::Packet::StoC::AgentName const& );
		void Update( GW::Packet::StoC::ObjectiveAdd const& );
		void Update( GW::Packet::StoC::ObjectiveUpdateName const& );
		void Update( GW::Packet::StoC::ObjectiveDone const& );
		void Update( GW::Packet::StoC::AgentUpdateAllegiance const& );
		void Reset();

		auto current() const noexcept { return times; }


	private:
		UwTimes times;
	};

}
