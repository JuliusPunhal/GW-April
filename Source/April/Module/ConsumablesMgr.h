#pragma once

#include "April/Utility/Consumable.h"
#include "April/Utility/unique_vector.h"

#include "Dependencies/GWCA.hpp"

#include <chrono>
#include <variant>
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

		void Activate( GW::ItemID, bool persistent = false );
		void Deactivate( GW::ItemID, bool persistent = false );
		void DeactivateAll( bool persistent = false );


		struct Inactive {}; struct Persistent {}; struct UntilLoad {};
		using ActiveState = std::variant<Inactive, Persistent, UntilLoad>;
		auto IsActive( GW::ItemID ) const -> ActiveState;

		
		using Quest = decltype(GW::Packet::StoC::ObjectiveDone::objective_id);
		Quest deactivating_quest = 0;


	private:
		std::chrono::steady_clock::time_point last_use{};
		unique_vector<Consumable> until_load{};
		unique_vector<Consumable> persistent{};

		Config const& config;
	};

}
