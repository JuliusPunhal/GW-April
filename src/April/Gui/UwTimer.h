#pragma once

#include "April/Utility/Mouse.h"
#include "April/Utility/UwTimesHistory.h"

#include "GWCA/GWCA.hpp"

#include "ImGui/ImGui.hpp"

#include <memory>


namespace April::Gui {

	class UwTimer {
	public:
		struct Config {
			Window window = {
				"Underworld Times",
				false,
				ImGuiWindowFlags_NoTitleBar
				| ImGuiWindowFlags_NoResize
				| ImGuiWindowFlags_NoScrollbar
				| ImGuiWindowFlags_NoCollapse
			};
			bool auto_set_visibility = true;
		};


		UwTimer(
			std::shared_ptr<Config>,
			std::shared_ptr<Mouse>,
			std::shared_ptr<UwTimesHistory const> );

		void Display();

		void Update( GW::Packet::StoC::MapLoaded const& );


	private:
		std::shared_ptr<Config>               config;
		std::shared_ptr<Mouse>                mouse;
		std::shared_ptr<UwTimesHistory const> history;
	};

}
