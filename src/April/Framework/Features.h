#pragma once

#include "April/Gui/InstanceTimer.h"
#include "April/Utility/FontAtlas.h"
#include "April/Utility/Mouse.h"

#include <memory>
#include <tuple>


namespace April {

	using Features =
		std::tuple<
			std::unique_ptr<Gui::InstanceTimer>,
			std::shared_ptr<FontAtlas>,
			std::shared_ptr<Mouse>>;


	auto make_Features() -> Features;

	void Display( Features& );

}
