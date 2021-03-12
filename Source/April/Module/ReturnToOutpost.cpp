
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


auto April::ReturnToOutpost::OnDefeated( Config const& config ) const
	-> Command
{
	if ( config.active && player_is_leader() )
	{
		return Return_to_Outpost{};
	}
	else return NoCommand;
}

auto April::ReturnToOutpost::Config::LoadDefault() -> Config
{
	return Config{
		true
	};
}
