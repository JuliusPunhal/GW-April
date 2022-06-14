#pragma once

#include "GWCA/GWCA.hpp"

#include <chrono>


namespace April::Module {

	class DhuumBot {
	public:
		DhuumBot() = default;

		void Update();
		void Update( GW::Packet::StoC::ObjectiveDone const& );

		void activate()                   { active = not active; }
		void activate( bool const state ) { active = state; }

		bool is_active() const { return active; }


	private:
		bool                                  active = false;
		std::chrono::steady_clock::time_point last_cast{};
	};

}
