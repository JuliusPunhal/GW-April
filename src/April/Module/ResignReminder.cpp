
#include "April/Module/ResignReminder.h"

#include <algorithm>
#include <string_view>

using namespace GW::Packet::StoC;


namespace {

	bool player_has_resigned()
	{
		auto const* party_player =
			GW::GetPlayerPartyMember( GW::GetPlayerLoginNumber() );
		if ( party_player == nullptr )
			return false;

		return GW::HasResigned( *party_player );
	}

	auto get_player_name() -> std::wstring_view
	{
		auto const player = GW::GetPlayerLoginNumber();
		if ( player == 0u )
			return L"";

		auto const* name = GW::GetPlayerArray()[player].name;
		return name ? name : L"";
	}

}


April::Module::ResignReminder::ResignReminder(
	std::shared_ptr<Config const> config )
	: config{ config }
{
}

void April::Module::ResignReminder::Update( MessageCore& packet ) const
{
	if ( not config->active )
		return;

	auto const resigner_name = GW::GetResignerName( packet );
	if ( resigner_name.empty() )
		return;

	if (
		resigner_name != get_player_name()
		&& not player_has_resigned()
		&& GW::CountConnectedAndResigned() == GW::CountConnected() - 1 )
	{
		GW::detail::Enqueue(
			[]()
			{
				GW::WriteChat(
					GW::PARTY,
					"<c=#FFFF00>You are the only player who has not resigned!"
					"</c>" );
			} );
	}
}
