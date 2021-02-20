
#include "April/Module/NotifyEffectLoss.h"

#include "April/Utility/stl.h"

#include "Dependencies/GWCA.hpp"

using Config = April::NotifyEffectLoss::Config;

using namespace GW::Packet::StoC;


namespace {

	auto get_effect_by_id( unsigned const id ) -> GW::Effect const*
	{
		for ( auto const& effect : GW::Effects::GetPlayerEffectArray() )
		{
			if ( effect.effect_id == id )
				return &effect;
		}
		return nullptr;
	}

}


April::NotifyEffectLoss::NotifyEffectLoss( Config const& config )
	: config{ config }
{
}

void April::NotifyEffectLoss::OnEffectLoss( RemoveEffect const& packet ) const
{
	auto const* player = GW::Agents::GetCharacter();
	if ( player == nullptr || player->agent_id != packet.agent_id )
		return;

	auto effect = get_effect_by_id( packet.effect_id );
	if ( effect == nullptr )
		return;

	auto const skill_id = static_cast<GW::SkillID>( effect->skill_id );
	auto const found =
		std::find_if(
			config.notifications,
			[skill_id]( auto const& notification )
			{
				return notification.skill_id == skill_id;
			} );

	if ( found != std::end( config.notifications ) )
	{
		GW::WriteChat( GW::Chat::CHANNEL_GWCA2, found->message );
	}
}

auto April::NotifyEffectLoss::Config::LoadDefault() -> Config
{
	return Config{
		std::vector<Notification>{
			{ GW::SkillID::Balthazars_Spirit,
				"<c=#FFFF00>Lost Balthasar's Spirit!</c>" },

			{ GW::SkillID::Protective_Bond,
				"<c=#FFFF00>Lost Protective Bond!</c>" },

			{ GW::SkillID::Life_Bond,
				"<c=#FFFF00>Lost Life Bond!</c>" }
		}
	};
}
