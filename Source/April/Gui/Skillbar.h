#pragma once

#include "April/Utility/DrawStructures.h"


namespace April::Gui {

	class Skillbar {
	public:
		Skillbar();

		void Display() const;


	private:
		ImFont* font;
	};

}
