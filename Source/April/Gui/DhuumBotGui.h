#pragma once

#include "April/Module/DhuumBot.h"

#include "Dependencies/ImGui.hpp"

#include <memory>
#include <string>


namespace April::Gui {

	class DhuumBotGui {
	public:
		struct Config {
			std::string			window_name;
			ImGuiWindowFlags	window_flags;

			static auto LoadDefault() -> Config;
		};


		DhuumBotGui( std::shared_ptr<DhuumBot>, Config const& );

		void Display() const;


	private:
		std::shared_ptr<DhuumBot> bot;
		Config const& config;
	};

}
