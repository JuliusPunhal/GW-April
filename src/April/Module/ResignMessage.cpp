
#include "April/Module/ResignMessage.h"

#include <algorithm>

using namespace GW::Packet::StoC;


namespace {

	auto count_connected_and_resigned(
		GW::PlayerPartyMemberArray const& players )
	{
		auto connected_and_resigned =
			[&]( GW::PlayerPartyMember const& player )
			{
				auto const& resigners = GW::GetResignedPlayers();
				auto const has_resigned =
					std::find(
						resigners.begin(),
						resigners.end(),
						player.login_number ) != resigners.end();

				return player.connected() && has_resigned;
			};

		return
			std::count_if(
				players.begin(), players.end(), connected_and_resigned );
	}

	auto count_connected( GW::PlayerPartyMemberArray const& players )
	{
		auto connected = []( GW::PlayerPartyMember const& player )
		{
			return player.connected();
		};
		return std::count_if( players.begin(), players.end(), connected );
	}

}


April::Module::ResignMessage::ResignMessage(
	std::shared_ptr<Config const> config )
	: config{ config }
{
}

void April::Module::ResignMessage::Update( MessageCore& packet ) const
{
	if ( not config->active )
		return;

	auto const* party = GW::GetPartyInfo();
	if ( party == nullptr || not party->players.valid() )
		return;

	auto const resigner_name = GW::GetResignerName( packet );
	if ( resigner_name.empty() )
		return;

	auto const new_name =
		std::wstring{ resigner_name }
		+ L" ("
		+ std::to_wstring( count_connected_and_resigned( party->players ) )
		+ L'/'
		+ std::to_wstring( count_connected( party->players ) )
		+ L')';

	packet = GW::CreateResignMessage( new_name );
}
