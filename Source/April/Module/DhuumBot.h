#pragma once

#include "Dependencies/GWCA.hpp"

#include <chrono>


namespace April {

	class DhuumBot {
	public:
		DhuumBot();

		void Update();

		constexpr bool is_active() const noexcept { return active; }
		constexpr void activate( bool const state = true ) noexcept 
		{ 
			active = state; 
		}


	private:
		std::chrono::time_point<std::chrono::steady_clock> last_cast{};

		bool active = false;
		GW::AgentID dhuum_id = 0;
	};

}
