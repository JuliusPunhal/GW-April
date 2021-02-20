#pragma once

#include "Dependencies/GWCA.hpp"


namespace April {

	class SuppressSpeechBubbles {
	public:
		struct Config {
			bool active;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\SuppressSpeechBubbles.ini";
		};


		SuppressSpeechBubbles( Config const& );

		void Suppress( GW::HookStatus* ) const;


	private:
		Config const& config;
	};

}
