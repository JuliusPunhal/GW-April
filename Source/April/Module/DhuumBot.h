#pragma once

#include "Dependencies/GWCA.hpp"

#include <chrono>
#include <optional>


namespace April {

	class DhuumBot {
	public:
		struct Skill {
			unsigned         slot;   // 0 ... 7
			GW::Agent const* target;
		};


		DhuumBot() = default;

		auto should_use_skill() -> std::optional<Skill>;
		void Update( GW::Packet::StoC::ObjectiveDone const& );

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
