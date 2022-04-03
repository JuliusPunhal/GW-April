#pragma once

#include "April/Utility/Consumables.h"

#include "GWCA/GWCA.hpp"

#include <chrono>
#include <optional>


namespace April::Module {

	class ConsumablesMgr {
	public:
		ConsumablesMgr() = default;

		void Update() const;
		void Update( GW::Packet::StoC::MapLoaded const& );
		void Update( GW::Packet::StoC::ObjectiveDone const& );


		void activate_perm( GW::ItemModelID );
		void activate_temp( GW::ItemModelID );

		void deactivate_perm( GW::ItemModelID );
		void deactivate_temp( GW::ItemModelID );

		void deactivate_all_perm();
		void deactivate_all_temp();


		bool is_active_perm( GW::ItemModelID ) const;
		bool is_active_temp( GW::ItemModelID ) const;


		std::optional<GW::ObjectiveID> objective;


	private:
		std::vector<Consumable> perm;
		std::vector<Consumable> temp;

		std::chrono::steady_clock::time_point mutable last_use;
	};

}
