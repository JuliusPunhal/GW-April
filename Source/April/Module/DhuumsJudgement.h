#pragma once

#include <chrono>


namespace April {

	class DhuumsJudgement {
	public:
		DhuumsJudgement();

		void Update();

		// InstanceTime when Dhuum last used Dhuum's Judgement
		constexpr auto last_judgement() const noexcept 
		{ 
			return last;
		}


	private:
		std::chrono::milliseconds last = std::chrono::milliseconds{ -1 };
	};

}
