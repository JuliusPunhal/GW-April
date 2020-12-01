#pragma once

#include "April/Utility/DrawStructures.h"


namespace April::Gui {

	class Healthbar {
	public:
		Healthbar();

		void Display() const;


	private:
		ImFont* font;
	};


 	class Energybar {
 	public:
 		Energybar();

		void Display() const;


	private:
		ImFont* font;
	};

}
