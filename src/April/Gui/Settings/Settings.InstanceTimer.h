#pragma once

#include "April/Gui/InstanceTimer.h"
#include "April/Utility/FontAtlas.h"

#include <memory>


namespace April::Gui {

	class Settings_InstanceTimer {
	public:
		Settings_InstanceTimer(
			std::shared_ptr<InstanceTimer::Config>,
			std::shared_ptr<FontAtlas const> );

		void Draw();


	private:
		std::shared_ptr<InstanceTimer::Config> config;
		std::shared_ptr<FontAtlas const>       fonts;
	};

}
