#pragma once

#include "April/Module/DhuumBot.h"

#include <memory>


namespace April::Gui {

	class DhuumBotGui {
	public:
		DhuumBotGui( std::shared_ptr<DhuumBot> );

		void Display() const;


	private:
		std::shared_ptr<DhuumBot> bot;
	};

}
