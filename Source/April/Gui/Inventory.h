#pragma once

#include "April/Module/ConsumablesMgr.h"

#include "April/Utility/DrawStructures.h"

#include <memory>


namespace April::Gui {

	class Inventory {
	public:
		Inventory( std::shared_ptr<ConsumablesMgr> );

		void Display() const;


	private:
		std::shared_ptr<ConsumablesMgr> cons_mgr;
		ImFont* font;
	};

}
