#pragma once

namespace April {
	struct ModuleConfigurations; // avoid cyclic include
}

#include "Dependencies/GWCA.hpp"


namespace April {

	class WindowMgr {
	public:
		WindowMgr() = default;

		void Update( ModuleConfigurations& );

		void Update(
			GW::Packet::StoC::ObjectiveDone const&,
			ModuleConfigurations& ) const;

		void Update(
			GW::Packet::StoC::AgentUpdateAllegiance const&,
			ModuleConfigurations& ) const;

		void Update(
			GW::Packet::StoC::MapLoaded const&,
			ModuleConfigurations& ) const;


	private:
		bool in_pits = false;
	};

}
