
#include "April/Gui/Settings/Settings.ResignLog.h"

#include "April/Utility/stl.h"

#include "GWCA/GWCA.hpp"

#include "ImGui/ImGui.hpp"

#include <array>


namespace {

	constexpr auto label_disconnected = "Not connected";
	constexpr auto label_not_resigned = "Not Resigned";
	constexpr auto label_has_resigned = "Resigned";

	constexpr auto labels = std::array{
		label_disconnected, label_not_resigned, label_has_resigned
	};


	auto get_label( GW::PlayerPartyMember const& player )
	{
		if ( not player.connected() )
			return label_disconnected;

		auto const& resigned_players = GW::GetResignedPlayers();
		auto const has_resigned =
			std::find_if(
				resigned_players.begin(),
				resigned_players.end(),
				[&player]( GW::LoginNumber const login_number )
				{
					return player.login_number == login_number;
				} ) != resigned_players.end();

		return has_resigned ? label_has_resigned : label_not_resigned;
	}

	auto button_size()
	{
		auto longest = 0.f;
		for ( auto const* label : labels )
			longest = std::max( longest, ImGui::CalcTextSize( label ).x );

		return longest + ImGui::GetStyle().ItemInnerSpacing.x * 2;
	}

}


void April::Gui::Settings_ResignLog::Draw()
{
	ImGui::TextWrapped(
		"The Resign Log shows which players in your party have resigned.\n"
		"Note that April has to be active and you have to be connected at the "
		"time they resign." );

	ImGui::SeparatorSpaced();

	auto const* party = GW::GetPartyInfo();
	if ( party == nullptr || not party->players.valid() )
		return;

	auto const size = button_size();
	for ( auto const& player : party->players )
	{
		ImGui::PushID( &player );
		auto const label = get_label( player );
		auto const name = GW::GetPlayerArray()[player.login_number].name;

		if ( ImGui::Button( label, { size, 0 } ) )
			GW::SendChat( '#', stl::wstr_to_ascii( name ) + ": " + label );

		ImGui::SameLine();
		ImGui::Text( stl::wstr_to_utf8( name ) );
		ImGui::PopID();
	}
}
