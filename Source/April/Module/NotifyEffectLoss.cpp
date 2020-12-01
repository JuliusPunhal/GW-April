
#include "April/Module/NotifyEffectLoss.h"

#include "April/Config/Module/NotifyEffectLost.config.hpp"
#include "April/Utility/stl.h"

#include "Dependencies/GWCA.hpp"

namespace config = April::NotifyEffectLostConfig;

using namespace GW::Packet::StoC;


namespace {

	auto entry = GW::HookEntry{};


	auto get_effect_by_id( unsigned const id ) -> GW::Effect const*
	{
		for ( auto const& effect : GW::Effects::GetPlayerEffectArray() )
		{
			if ( effect.effect_id == id )
				return &effect;
		}
		return nullptr;
	}
	
	void on_remove_effect( GW::HookStatus*, RemoveEffect const* packet )
	{
		auto const* player = GW::Agents::GetCharacter();
		if ( player == nullptr || player->agent_id != packet->agent_id )
			return;
				
		auto effect = get_effect_by_id( packet->effect_id );
		if ( effect == nullptr )
			return;

		auto const skill_id = static_cast<GW::SkillID>( effect->skill_id );
		auto const found = std::find( config::notifications, skill_id );
		if ( found != std::end( config::notifications ) )
		{
			GW::Chat::WriteChat( GW::Chat::CHANNEL_GWCA2, found->message );
		}
	}

}


April::NotifyEffectLoss::NotifyEffectLoss()
{
	// Callback will only be cleaned up during GWCA shutdown.
	GW::StoC::RegisterPacketCallback<RemoveEffect>( &entry, on_remove_effect );
}
