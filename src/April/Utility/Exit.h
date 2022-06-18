#pragma once

#include <memory>


namespace April {

	class Exit {
	public:
		struct Config {
			bool allow_vk_end = false;
		};


		Exit( std::shared_ptr<Config> );

		void Update();

		auto initiate_exit()   { exit = true; }
		auto want_exit() const { return exit; }


	private:
		std::shared_ptr<Config> config;

		bool exit = false;
	};

}
