#pragma once

#include "Dependencies/ImGui.hpp"


namespace April::Gui {

	class InstanceTimer {
	public:
		struct Config {
			ImFont*				font;
			RGBA				text_color;
			RGBA				shadow_color;
			XY					shadow_offset;
			std::string			window_name;
			ImGuiWindowFlags	window_flags;

			static auto LoadDefault() -> Config;
		};


		InstanceTimer( Config const& );

		void Display() const;


	private:
		Config const& config;
	};

}
