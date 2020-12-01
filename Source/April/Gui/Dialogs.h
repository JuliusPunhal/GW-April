#pragma once

#include <chrono>
#include <optional>


namespace April::Gui {

	class Dialogs {
	public:
		Dialogs();

		void Display();


	private:
		struct LastClick {
			unsigned 								button;
			std::chrono::steady_clock::time_point	time;
		};
		std::optional<LastClick> awaiting_confirmation = std::nullopt;
	};

}
