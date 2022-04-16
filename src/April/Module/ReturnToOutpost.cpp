
#include "April/Module/ReturnToOutpost.h"

using namespace GW::Packet::StoC;


April::Module::ReturnToOutpost::ReturnToOutpost(
	std::shared_ptr<Config const> config )
	: config{ config }
{
}

void April::Module::ReturnToOutpost::Update( PartyDefeated const& ) const
{
	if ( config->active && GW::GetIsPlayerLeader() )
		GW::SendPacket( GW::Packet::CtoS::ReturnToOutpost{} );
}

void April::Module::ReturnToOutpost::Update(
	PlayerIsPartyLeader const& packet ) const
{
	if (
		config->active
		&& packet.is_leader
		&& GW::GetPartyContext().IsDefeated() )
	{
		GW::SendPacket( GW::Packet::CtoS::ReturnToOutpost{} );
	}
}
