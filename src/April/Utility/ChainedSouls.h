#pragma once

#include "GWCA/GWCA.hpp"

#include <array>
#include <optional>
#include <variant>


namespace April {

	class ChainedSoul {
	public:
		struct Alive {
			float hp;   //   0 ...  1
			float pips; // -10 ... 10
		};
		struct Dead {
			std::optional<GW::InstanceTime> time_of_death{};
		};
		struct OutOfRange {
			std::optional<GW::InstanceTime> time_of_death{};
		};
		using State = std::variant<OutOfRange, Alive, Dead>;


		ChainedSoul( float, float );

		auto time_of_death() const -> std::optional<GW::InstanceTime>;
		auto state() const -> auto const& { return state_; }

		void Update();


	private:
		GW::GamePos const pos;
		State             state_ = OutOfRange{};
	};


	class ChainedSouls {
	public:
		using type = std::array<ChainedSoul, 8>;


		ChainedSouls() = default;

		auto Get() const -> auto const& { return souls; }

		void Update();


	private:
		type souls{
			ChainedSoul{ 12074, -1496 }, // Entrance, where T3 DCs up after Q
			ChainedSoul{ 14462,  -218 }, // Bridge, dont confuse with Safespot
			ChainedSoul{ 14206,  1318 }, // Safespot, in trench below bridge
			ChainedSoul{ 11459,  2805 }, // Bow
			ChainedSoul{ 13897,  4469 }, // Hill, where souls can aggro
			ChainedSoul{ 11427,  5079 }, // Bottom, where souls get stuck
			ChainedSoul{  9245,  4898 }, // Double
			ChainedSoul{ 10130,  6616 }  // Reaper
		};
	};


}
