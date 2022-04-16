
#include "April/Module/NotifyOnEffect.h"

#include <algorithm>

using namespace GW::Packet::StoC;
using Notification = April::Module::NotifyOnEffect::Notification;


namespace {

	template<typename Packet_t>
	bool for_player( Packet_t const& packet )
	{
		auto const* player = GW::GetCharacter();
		return player && player->agent_id == packet.agent_id;
	}

	bool for_player( AddExternalBond const& packet )
	{
		auto const* player = GW::GetCharacter();
		return player && player->agent_id == packet.receiver_id;
	}

	template<typename Packet_t>
	auto skill_id( Packet_t const& packet )
	{
		return static_cast<GW::SkillID>( packet.skill_id );
	}

	auto skill_id( RemoveEffect const& packet )
	{
		if ( auto const* effects = GW::GetPlayerEffects() )
		{
			auto equals = [&]( GW::Effect const& effect )
			{
				return effect.effect_id == packet.effect_id;
			};
			auto const found =
				std::find_if( effects->begin(), effects->end(), equals );

			if ( found != effects->end() )
				return static_cast<GW::SkillID>( found->skill_id );
		}

		return GW::SkillID::No_Skill;
	}

	void send_notification(
		GW::SkillID const id, std::vector<Notification> const& notifications )
	{
		auto equals = [id]( Notification const& notification )
		{
			return notification.skill_id == id;
		};
		auto const found =
			std::find_if( notifications.begin(), notifications.end(), equals );

		if ( found == notifications.end() )
			return;

		GW::WriteChat( GW::PARTY, found->message );
	}

}


April::Module::NotifyOnEffect::NotifyOnEffect(
	std::shared_ptr<Config const> config )
	: config{ config }
{
}

void April::Module::NotifyOnEffect::Update(
	AddEffect const& packet ) const
{
	if ( config->active && for_player( packet ) )
		send_notification( skill_id( packet ), config->on_gain );
}

void April::Module::NotifyOnEffect::Update(
	AddExternalBond const& packet ) const
{
	if ( config->active && for_player( packet ) )
		send_notification( skill_id( packet ), config->on_gain );
}

void April::Module::NotifyOnEffect::Update( RemoveEffect const& packet ) const
{
	if ( config->active && for_player( packet ) )
		send_notification( skill_id( packet ), config->on_loss );
}
