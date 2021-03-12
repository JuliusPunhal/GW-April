
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


auto April::NotifyEffectLoss::OnEffectLoss(
	RemoveEffect const& packet, Config const& config ) const
	-> Command
{
	auto const* player = GW::Agents::GetCharacter();
	if ( player == nullptr || player->agent_id != packet.agent_id )
		return NoCommand;

	auto effect = get_effect_by_id( packet.effect_id );
	if ( effect == nullptr )
		return NoCommand;

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
		return WriteChat{ GW::Chat::CHANNEL_GWCA2, found->message };
	}

	return NoCommand;
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
