
#include "April/Module/ReturnToOutpost.h"

#include "Dependencies/GWCA.hpp"

using namespace GW::Packet::StoC;


namespace {

	bool player_is_leader()
	{
		auto const* party = GW::PartyMgr::GetPartyInfo();
		if ( party == nullptr ) return false;
		if ( party->players.size() == 0) return false;

		auto const* player = GW::Agents::GetCharacter();
		if ( player == nullptr ) return false;

		for ( auto const& member : party->players )
		{
			// Skip players that have already left
			if ( member.connected() == false ) continue;

			return member.login_number == player->login_number;
		}

		return false;
	}

}


April::ReturnToOutpost::ReturnToOutpost( Config const& config )
	: config{ config }
{
}

void April::ReturnToOutpost::OnDefeated() const
{
	if ( config.active && player_is_leader() )
	{
		GW::CtoS::SendPacket( 0x4, GAME_CMSG_PARTY_RETURN_TO_OUTPOST );
	}
}

auto April::ReturnToOutpost::Config::LoadDefault() -> Config
{
	return Config{
		true
	};
}
