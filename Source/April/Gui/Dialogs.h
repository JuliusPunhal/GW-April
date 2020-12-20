#pragma once

#include "April/Utility/DrawStructures.h"

#include "Dependencies/ImGui.hpp"

#include <chrono>
#include <optional>
#include <string>
#include <vector>


namespace April::Gui {

	class Dialogs {
	public:
		struct Config {
			struct Dialog {
				std::string description;
				int			dialog;
			};

			std::vector<Dialog>			dialogs;
			std::chrono::milliseconds	confirm_time;
			RGBA						confirm_color;

			std::string			window_name;
			ImGuiWindowFlags	window_flags;

			static auto LoadDefault() -> Config;
		};


		Dialogs( Config const& );

		void Display();


	private:
		struct LastClick {
			unsigned 								button;
			std::chrono::steady_clock::time_point	time;
		};
		std::optional<LastClick> awaiting_confirmation = std::nullopt;

		Config const& config;
	};

}
