#pragma once


namespace April {

	class ReturnToOutpost {
	public:
		struct Config {
			bool active;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\ReturnToOutpost.ini";
		};


		ReturnToOutpost( Config const& );


	private:
		Config const& config;
	};

}
