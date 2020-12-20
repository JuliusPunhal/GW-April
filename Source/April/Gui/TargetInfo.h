#pragma once

#include "Dependencies/ImGui.hpp"

#include <string>


namespace April::Gui {

	class TargetInfo {
	public:
		struct Config {
			std::string			window_name;
			ImGuiWindowFlags	window_flags;

			static auto LoadDefault() -> Config;
		};


		TargetInfo( Config const& );

		void Display() const;
	

	private:
		Config const& config;
	};

}
