#pragma once

#include "Dependencies/GWCA.hpp"

#include <chrono>


namespace April {

	class DhuumsJudgement {
	public:
		DhuumsJudgement() = default;

		void Update();
		void Update( GW::Packet::StoC::MapLoaded const& );

		// InstanceTime when Dhuum last used Dhuum's Judgement
		constexpr auto last_judgement() const noexcept
		{
			return last;
		}


	private:
		std::chrono::milliseconds last = std::chrono::milliseconds{ -1 };
	};

}
