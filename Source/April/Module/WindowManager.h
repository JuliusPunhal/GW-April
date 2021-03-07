#pragma once

namespace April {
	struct Instance; // avoid cyclic include
}

#include "Dependencies/GWCA.hpp"


namespace April {

	class WindowMgr {
	public:
		WindowMgr() = default;

		void Update( Instance& );

		void Update(
			GW::Packet::StoC::ObjectiveDone const&, Instance& ) const;

		void Update(
			GW::Packet::StoC::AgentUpdateAllegiance const&, Instance& ) const;

		void Update(
			GW::Packet::StoC::MapLoaded const&, Instance& ) const;


	private:
		bool in_pits = false;
	};

}
