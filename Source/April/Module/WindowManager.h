#pragma once

namespace April {
	struct ModuleConfigurations; // avoid cyclic include
}

#include "Dependencies/GWCA.hpp"


namespace April {

	class WindowMgr {
	public:
		WindowMgr( ModuleConfigurations& );

		void Update();
		void Update( GW::Packet::StoC::ObjectiveDone const& ) const;
		void Update( GW::Packet::StoC::AgentUpdateAllegiance const& ) const;
		void Update( GW::Packet::StoC::MapLoaded const& ) const;


	private:
		ModuleConfigurations& configs;
		bool in_pits = false;
	};

}
