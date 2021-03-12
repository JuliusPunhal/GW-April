#pragma once


namespace April {

	class SuppressSpeechBubbles {
	public:
		struct Config {
			bool active;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\SuppressSpeechBubbles.ini";
		};


		SuppressSpeechBubbles() = default;

		bool should_suppress( Config const& ) const;
	};

}
