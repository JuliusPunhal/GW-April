#pragma once

#include "April/Gui/HealthEnergy.h"
#include "April/Utility/FontAtlas.h"

#include <memory>


namespace April::Gui {

	class Settings_Healthbar {
	public:
		Settings_Healthbar(
			std::shared_ptr<Healthbar::Config>,
			std::shared_ptr<FontAtlas const> );

		void Draw();


	private:
		std::shared_ptr<Healthbar::Config> config;
		std::shared_ptr<FontAtlas const>   fonts;
	};


	class Settings_Energybar {
	public:
		Settings_Energybar(
			std::shared_ptr<Energybar::Config>,
			std::shared_ptr<FontAtlas const> );

		void Draw();


	private:
		std::shared_ptr<Energybar::Config> config;
		std::shared_ptr<FontAtlas const>   fonts;
	};

}
