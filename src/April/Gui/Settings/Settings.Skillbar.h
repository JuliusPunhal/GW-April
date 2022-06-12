#pragma once

#include "April/Gui/Skillbar.h"
#include "April/Utility/FontAtlas.h"

#include <memory>


namespace April::Gui {

	class Settings_Skillbar {
	public:
		using Threshold = Skillbar::Config::Threshold;


		Settings_Skillbar(
			std::shared_ptr<Skillbar::Config>,
			std::shared_ptr<FontAtlas const> );

		void Draw();


	private:
		std::shared_ptr<Skillbar::Config> config;
		std::shared_ptr<FontAtlas const>  fonts;

		Threshold new_threshold = make_threshold();

		static auto make_threshold() -> Threshold;
	};

}
