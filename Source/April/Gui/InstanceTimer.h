#pragma once

#include "April/Utility/DrawStructures.h"


namespace April::Gui {

	class InstanceTimer {
	public:
		InstanceTimer();

		void Display() const;


	private:
		ImFont*	font;
	};

}
