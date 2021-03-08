#pragma once

#include "April/Gui/ChainedSoulInfo.h"
#include "April/Gui/DhuumBotGui.h"
#include "April/Gui/DhuumInfo.h"
#include "April/Gui/UwTimes.h"

#include "Dependencies/GWCA.hpp"


namespace April {

	class WindowMgr {
	public:
		WindowMgr() = default;

		void Update( Gui::ChainedSoulGui::Config& );

		void Update(
			GW::Packet::StoC::ObjectiveDone const&,
			Gui::DhuumBotGui::Config&,
			Gui::DhuumInfo::Config& ) const;

		void Update(
			GW::Packet::StoC::AgentUpdateAllegiance const&,
			Gui::DhuumBotGui::Config&,
			Gui::DhuumInfo::Config& ) const;

		void Update(
			GW::Packet::StoC::MapLoaded const&,
			Gui::ChainedSoulGui::Config&,
			Gui::DhuumBotGui::Config&,
			Gui::DhuumInfo::Config&,
			Gui::UwTimesGui::Config& ) const;


	private:
		bool in_pits = false;
	};

}
