#pragma once

#include "GWCA/GWCA.hpp"

#include <memory>
#include <string>
#include <vector>


namespace April::Module {

	class NotifyOnEffect {
	public:
		struct Notification {
			GW::SkillID skill_id;
			std::string message;
		};
		using Notifications = std::vector<Notification>;

		struct Config {
			bool                      active = true;
			Notifications on_gain{};
			Notifications on_loss{
				{ GW::SkillID::Balthazars_Spirit,
					"<c=#FFFF00>Lost Balthasar's Spirit!</c>" },
				{ GW::SkillID::Protective_Bond,
					"<c=#FFFF00>Lost Protective Bond!</c>" },
				{ GW::SkillID::Life_Bond,
					"<c=#FFFF00>Lost Life Bond!</c>" }
			};
		};


		NotifyOnEffect( std::shared_ptr<Config const> );

		void Update( GW::Packet::StoC::AddEffect const& ) const;
		void Update( GW::Packet::StoC::AddExternalBond const& ) const;
		void Update( GW::Packet::StoC::RemoveEffect const& ) const;


	private:
		std::shared_ptr<Config const> config;
	};

}
