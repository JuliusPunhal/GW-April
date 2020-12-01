#pragma once

#include "April/Utility/stl.h"

#include <chrono>
#include <optional>
#include <variant>


namespace April {

	class ChainedSoul {
	public:
		struct AliveSoul {
			float hp;
			float pips;
		};
		struct DeadSoul {
			std::optional<std::chrono::milliseconds> time_of_death{};
		};
		struct OutOfRange {
			std::optional<std::chrono::milliseconds> time_of_death{};
		};
		using SoulState = std::variant<AliveSoul, DeadSoul, OutOfRange>;


		ChainedSoul() = default;

		void Update();

		constexpr auto get() const noexcept { return state; }

		constexpr auto time_of_death() const noexcept
			-> std::optional<std::chrono::milliseconds>
		{
			using namespace std::chrono;

			auto const tot_visitor =
				std::overloaded{
					[]( AliveSoul ) { return std::optional<milliseconds>{}; },
					[]( DeadSoul const soul ) { return soul.time_of_death; },
					[]( OutOfRange const soul ) { return soul.time_of_death; }
				};
			return std::visit( tot_visitor, state );
		}
		

	private:
		SoulState state = OutOfRange{};
	};

}
