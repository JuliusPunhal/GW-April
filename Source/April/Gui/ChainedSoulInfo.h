#pragma once

#include "April/Module/ChainedSoul.h"

#include <memory>


namespace April::Gui {

	class ChainedSoulGui {
	public:
		ChainedSoulGui( std::shared_ptr<ChainedSoul const> );

		void Display() const;


	private:
		std::shared_ptr<ChainedSoul const> state;
	};

}
