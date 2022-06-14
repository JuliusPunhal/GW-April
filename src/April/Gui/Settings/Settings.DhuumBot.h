#pragma once

#include "April/Module/DhuumBot.h"

#include <memory>


namespace April::Gui {

	class Settings_DhuumBot {
	public:
		Settings_DhuumBot( std::shared_ptr<Module::DhuumBot> );

		void Draw();


	private:
		std::shared_ptr<Module::DhuumBot> bot;
	};

}
