#pragma once

namespace April {
	struct ModuleConfigurations; // avoid cyclic include
}

#include "Dependencies/ImGui.hpp"

#include <string>


namespace April::Gui {

	class Settings {
	public:
		struct Config {
			std::string			window_name;
			ImGuiWindowFlags	window_flags;

			static auto LoadDefault() -> Config;
		};


		Settings( ModuleConfigurations& );
	
		void Display() const;


	private:
		ModuleConfigurations& configurations;
	};

}
