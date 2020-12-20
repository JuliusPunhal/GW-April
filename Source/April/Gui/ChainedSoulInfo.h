#pragma once

#include "April/Module/ChainedSoul.h"

#include "Dependencies/ImGui.hpp"

#include <memory>
#include <string>


namespace April::Gui {

	class ChainedSoulGui {
	public:
		struct Config {
			std::string			window_name;
			ImGuiWindowFlags	window_flags;

			static auto LoadDefault() -> Config;
		};


		ChainedSoulGui( std::shared_ptr<ChainedSoul const>, Config const& );

		void Display() const;


	private:
		std::shared_ptr<ChainedSoul const> state;
		Config const& config;
	};

}
