#pragma once

#include "Dependencies/GWCA.hpp"

#include <string>
#include <vector>


namespace April {

	class NotifyEffectLoss {
	public:
		struct Config {
			struct Notification {
				GW::SkillID skill_id;
				std::string message;
			};

			std::vector<Notification> notifications;

			static auto LoadDefault() -> Config;
		};


		NotifyEffectLoss( Config const& );


	private:
		Config const& config;
	};

}
