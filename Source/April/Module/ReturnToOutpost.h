#pragma once


namespace April {

	class ReturnToOutpost {
	public:
		struct Config {
			bool active;

			static auto LoadDefault() -> Config;
		};


		ReturnToOutpost( Config const& );


	private:
		Config const& config;
	};

}
