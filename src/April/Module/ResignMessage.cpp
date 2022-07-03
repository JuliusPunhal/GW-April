
#include "April/Module/ResignMessage.h"

#include <algorithm>

using namespace GW::Packet::StoC;


April::Module::ResignMessage::ResignMessage(
	std::shared_ptr<Config const> config )
	: config{ config }
{
}

void April::Module::ResignMessage::Update( MessageCore& packet ) const
{
	if ( not config->active )
		return;

	auto const resigner_name = GW::GetResignerName( packet );
	if ( resigner_name.empty() )
		return;

	auto const new_name =
		std::wstring{ resigner_name }
		+ L" ("
		+ std::to_wstring( GW::CountConnectedAndResigned() )
		+ L'/'
		+ std::to_wstring( GW::CountConnected() )
		+ L')';

	packet = GW::CreateResignMessage( new_name );
}
