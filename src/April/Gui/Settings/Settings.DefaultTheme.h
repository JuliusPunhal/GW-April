#pragma once

#include "April/Utility/DefaultTheme.h"
#include "April/Utility/FontAtlas.h"

#include <memory>


namespace April::Gui {

	class Settings_DefaultTheme {
	public:
		Settings_DefaultTheme(
			std::shared_ptr<DefaultTheme::Config>,
			std::shared_ptr<FontAtlas const> );

		void Draw();


	private:
		std::shared_ptr<DefaultTheme::Config> config;
		std::shared_ptr<FontAtlas const>      fonts;
	};

}
