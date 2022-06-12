#pragma once

#include "April/Gui/Settings.Config.h"
#include "April/Utility/FontAtlas.h"

#include <memory>


namespace April::Gui {

	class Settings_Settings {
	public:
		Settings_Settings(
			std::shared_ptr<Settings_Config>,
			std::shared_ptr<FontAtlas const> );

		void Draw();


	private:
		std::shared_ptr<Settings_Config> config;
		std::shared_ptr<FontAtlas const> fonts;
	};

}
