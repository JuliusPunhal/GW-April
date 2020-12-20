#pragma once

#include "April/Module/ConsumablesMgr.h"

#include "April/Utility/DrawStructures.h"

#include "Dependencies/ImGui.hpp"

#include <memory>
#include <string>


namespace April::Gui {

	class Inventory {
	public:
		struct Config {
			ImFont* font;
			
			WH		slot_size;
			XY		item_spacing;
			bool	show_border;
			float	button_alpha;
			
			RGBA no_item;
			RGBA unknown_item;
			RGBA wrong_instance_type;
			RGBA need_not_use;
			RGBA can_not_use;
			RGBA suggest_use;
			RGBA special_state;
			
			char label_no_item;
			char label_unknown_item;
			char label_persistent;
			char label_until_load;
			char label_until_objective;
			char label_inactive;

			std::string			window_name;
			ImGuiWindowFlags	window_flags;

			static auto LoadDefault() -> Config;
		};


		Inventory( std::shared_ptr<ConsumablesMgr>, Config const& );

		void Display() const;


	private:
		std::shared_ptr<ConsumablesMgr> cons_mgr;
		Config const& config;
	};

}
