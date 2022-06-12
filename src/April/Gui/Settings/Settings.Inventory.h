#pragma once

#include "April/Gui/Inventory.h"
#include "April/Utility/FontAtlas.h"

#include <memory>


namespace April::Gui {

	class Settings_Inventory {
	public:
		Settings_Inventory(
			std::shared_ptr<Inventory::Config>,
			std::shared_ptr<FontAtlas const> );

		void Draw();


	private:
		std::shared_ptr<Inventory::Config> config;
		std::shared_ptr<FontAtlas const>   fonts;
	};

}
