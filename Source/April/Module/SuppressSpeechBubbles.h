#pragma once


namespace April {

	class SuppressSpeechBubbles {
	public:
		struct Config {
			bool active;

			static auto LoadDefault() -> Config;
		};


		SuppressSpeechBubbles( Config const& );


	private:
		Config const& config;
	};

}
