#pragma once

#include "April/Module/ConsumablesMgr.h"

#include "Dependencies/GWCA.hpp"

#include <string>


namespace April {

	class ChatCommands {
	public:
		ChatCommands( std::shared_ptr<ConsumablesMgr> );


	private:
		std::shared_ptr<ConsumablesMgr> consumables;
	};

}
