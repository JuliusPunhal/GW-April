#pragma once

#include "April/Framework/Command.h"

#include "Dependencies/GWCA.hpp"

#include <vector>


namespace April {

	class WindowMgr {
	public:
		WindowMgr() = default;

		auto Update() -> Command;

		auto Update( GW::Packet::StoC::ObjectiveDone const& ) const
			 -> Command;

		auto Update( GW::Packet::StoC::AgentUpdateAllegiance const& ) const
			-> Command;

		auto Update( GW::Packet::StoC::MapLoaded const& ) const
			-> std::vector<Command>;


	private:
		bool in_pits = false;
	};

}
