#pragma once

#include "April/Utility/UwTimesHistory.h"

#include "GWCA/GWCA.hpp"

#include <memory>


namespace April::Module {

	class UwTimer {
	public:
		UwTimer( std::shared_ptr<UwTimesHistory> );

		void Update();
		void Update( GW::Packet::StoC::AgentName const& );
		void Update( GW::Packet::StoC::AgentUpdateAllegiance const& );
		void Update( GW::Packet::StoC::MapLoaded const& );
		void Update( GW::Packet::StoC::ObjectiveAdd const& );
		void Update( GW::Packet::StoC::ObjectiveDone const& );
		void Update( GW::Packet::StoC::ObjectiveUpdateName const& );


	private:
		std::shared_ptr<UwTimesHistory> times;
		GW::MapID prev_map_id;
	};

}
