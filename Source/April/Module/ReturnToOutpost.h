#pragma once

#include "April/Framework/Command.h"


namespace April {

	class ReturnToOutpost {
	public:
		struct Config {
			bool active;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\ReturnToOutpost.ini";
		};


		ReturnToOutpost() = default;

		auto OnDefeated( Config const& ) const -> Command;
	};

}
