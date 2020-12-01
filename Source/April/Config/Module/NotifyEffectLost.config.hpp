#pragma once

#include "Dependencies/GWCA.hpp"


namespace April::NotifyEffectLostConfig {

	struct Notification {
		GW::SkillID skill_id;
		const char* message;
	};
	constexpr bool operator==( 
		Notification const& notification, GW::SkillID const skill_id ) noexcept
	{
		return notification.skill_id == skill_id;
	}


	inline constexpr Notification notifications[]{
		GW::SkillID::Balthazars_Spirit, "<c=#FFFF00>Lost Balthasar's Spirit!</c>",
		GW::SkillID::Protective_Bond, "<c=#FFFF00>Lost Protective Bond!</c>",
		GW::SkillID::Life_Bond, "<c=#FFFF00>Lost Life Bond!</c>"
	};

}
