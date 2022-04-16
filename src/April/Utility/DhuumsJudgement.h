#pragma once

#include "GWCA/GWCA.hpp"

#include <optional>


namespace April {

	class DhuumsJudgement {
	public:
		DhuumsJudgement() = default;

		auto Get() const { return last_judgement; }

		void Update();
		void Update( GW::Packet::StoC::MapLoaded const& );


	private:
		std::optional<GW::InstanceTime> last_judgement;
		std::optional<GW::AgentID>      dhuum_id;
	};

}
