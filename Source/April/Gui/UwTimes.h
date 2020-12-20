#pragma once

#include "April/Module/UwTimer.h"

#include "Dependencies/ImGui.hpp"

#include <array>
#include <memory>
#include <string>


namespace April::Gui {
	
	class UwTimesGui {
	public:
		struct Config {
			std::array<std::string, 11> quest_names;
			std::array<std::string, 4>	column_names;

			std::string			window_name;
			ImGuiWindowFlags	window_flags;

			static auto LoadDefault() -> Config;
		};


		UwTimesGui( std::shared_ptr<UwTimes const>, Config const& );

		void Display() const;


	private:
		std::shared_ptr<UwTimes const> times;
		Config const& config;
	};

}
