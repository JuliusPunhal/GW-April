#pragma once

#include "Dependencies/GWCA.hpp"

#include <array>
#include <chrono>
#include <memory>


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
		UwTimer( std::shared_ptr<UwTimes> );

		void Update();


	private:
		std::shared_ptr<UwTimes> times;

		void UpdatePop( GW::Packet::StoC::AgentName const& );
		void UpdateTake( GW::Packet::StoC::ObjectiveAdd const& );
		void UpdateTake( GW::Packet::StoC::ObjectiveUpdateName const& );
		void UpdateDone( GW::Packet::StoC::ObjectiveDone const& );
		void UpdateHostile( GW::Packet::StoC::AgentUpdateAllegiance const& );
		void Reset();
	};

}
