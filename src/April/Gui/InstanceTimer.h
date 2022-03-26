#pragma once

#include "April/Utility/Mouse.h"

#include "ImGui/ImGui.hpp"

#include <memory>


namespace April::Gui {

	class InstanceTimer {
	public:
		InstanceTimer( ImFont*, std::shared_ptr<Mouse> );

		void Display() const;


	private:
		ImFont* font;
		std::shared_ptr<Mouse> mouse;
	};

}
