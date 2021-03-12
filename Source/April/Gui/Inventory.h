#pragma once

#include "April/Framework/Command.h"
#include "April/Module/ConsumablesMgr.h"

#include "April/Utility/DrawStructures.h"

#include <vector>


namespace April::Gui {

	class Inventory {
	public:
		struct Config {
			FontInfo font;

			WH		slot_size;
			XY		item_spacing;
			bool	border;
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

			Window window;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\Gui_Inventory.ini";
		};


		Inventory( Config const& );

		auto Display( ConsumablesMgr const&, Config const& ) const
			-> std::vector<Command>;


	private:
		ImFont* font;
	};

}
