#pragma once

#include "April/Module/ChatFilter.h"

#include <memory>


namespace April::Gui {

	class Settings_ChatFilter {
	public:
		Settings_ChatFilter( std::shared_ptr<Module::ChatFilter::Config> );

		void Draw();


	private:
		std::shared_ptr<Module::ChatFilter::Config> config;
	};

}
