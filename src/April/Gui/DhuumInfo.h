#pragma once

#include "April/Utility/DhuumsJudgement.h"
#include "April/Utility/Mouse.h"

#include "GWCA/GWCA.hpp"

#include "ImGui/ImGui.hpp"

#include <memory>


namespace April::Gui {

	class DhuumInfo {
	public:
		struct Config {
			Window window = { "Dhuum's Rest", false, 0 };
			bool auto_set_visibility = true;
		};


		DhuumInfo(
			std::shared_ptr<Config>,
			std::shared_ptr<DhuumsJudgement const>,
			std::shared_ptr<Mouse> );

		void Display();

		void Update( GW::Packet::StoC::AgentUpdateAllegiance const& );
		void Update( GW::Packet::StoC::ObjectiveDone const& );
		void Update( GW::Packet::StoC::MapLoaded const& );


	private:
		std::shared_ptr<Config>                config;
		std::shared_ptr<DhuumsJudgement const> judgement;
		std::shared_ptr<Mouse>                 mouse;
	};

}
