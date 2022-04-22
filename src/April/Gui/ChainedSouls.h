#pragma once

#include "April/Utility/ChainedSouls.h"
#include "April/Utility/Mouse.h"

#include "GWCA/GWCA.hpp"

#include "ImGui/ImGui.hpp"

#include <memory>


namespace April::Gui {

	class ChainedSouls {
	public:
		struct Config {
			Window window = { "Chained Souls", false, 0 };
			bool   auto_set_visibility = true;
		};


		ChainedSouls(
			std::shared_ptr<Config>,
			std::shared_ptr<Mouse>,
			std::shared_ptr<April::ChainedSouls const> );

		void Display();

		void Update();
		void Update( GW::Packet::StoC::MapLoaded const& );


	private:
		std::shared_ptr<Config>                    config;
		std::shared_ptr<Mouse>                     mouse;
		std::shared_ptr<April::ChainedSouls const> souls;

		bool mutable in_pits = false;
	};

}
