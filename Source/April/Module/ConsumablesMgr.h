#pragma once

#include "April/Utility/Consumable.h"
#include "April/Utility/unique_vector.h"

#include "Dependencies/GWCA.hpp"

#include <chrono>
#include <cstdint>
#include <utility>
#include <vector>


namespace April {

	class ConsumablesMgr {
	public:
		struct Config {
			std::chrono::milliseconds timeout;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\ConsumablesManager.ini";
		};


		ConsumablesMgr( Config const& );

		void Update();

		struct Active {
			bool temporary;
			bool persistent;
		};
		auto is_active( GW::ItemID ) const -> Active;

		void activate_temporary( GW::ItemID );
		void deactivate_temporary( GW::ItemID );
		void deactivate_all_temporary();

		void activate_persistent( GW::ItemID );
		void deactivate_persistent( GW::ItemID );
		void deactivate_all_persistent();


		uint32_t deactivating_quest = 0;


	private:
		std::chrono::steady_clock::time_point last_use{};
		unique_vector<Consumable> temporary{};
		unique_vector<Consumable> persistent{};

		Config const& config;
	};

}
