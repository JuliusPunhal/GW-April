
#include "April/Module/ReturnToOutpost.h"

#include "Dependencies/GWCA.hpp"

using namespace GW::Packet::StoC;


namespace {

	// TODO: Detect if someone left, possibly making the user leader.

	auto entry = GW::HookEntry{};


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

	void try_return()
	{
		if ( player_is_leader() )
		{
			GW::CtoS::SendPacket( 0x4, GAME_CMSG_PARTY_RETURN_TO_OUTPOST );
		}
	}

}


April::ReturnToOutpost::ReturnToOutpost( Config const& config )
	: config{ config }
{
	// Callback will only be cleaned up during GWCA shutdown.
	GW::StoC::RegisterPacketCallback<PartyDefeated>( 
		&entry, 
		[this]( auto*, auto* ) { if ( this->config.active ) try_return(); } );
}

auto April::ReturnToOutpost::Config::LoadDefault() -> Config
{
	auto const config = Config{
		true 
	};
	
	return config;
}
