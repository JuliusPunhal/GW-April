
#include "April/Gui/Settings/Settings.Dialogs.h"

#include "ImGui/ImGui.hpp"

using namespace April;
using namespace std::string_literals;

using NamedDialog = April::Gui::Settings_Dialogs::NamedDialog;


namespace {

	constexpr auto popup_window_id = "Confirmation Popup";


	void draw_confirmation_popup( std::optional<NamedDialog>& clicked )
	{
		if ( ImGui::BeginPopupModal( popup_window_id ) )
		{
			char buf[32] = {};
			snprintf( buf, sizeof( buf ), "0x%X", clicked->dialog );

			ImGui::Text(
				"Are you sure you want to send this Dialog?\n\""s
				+ clicked->description + "\": " + std::string{ buf } + "?" );

			auto const size = ImGui::WidthToFitNItems( 2 );
			ImGui::PushStyleColor( ImGuiCol_Button, MutedRed() );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive, White() );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, Red() );
			if ( ImGui::Button( "Send", { size, 0 } ) )
			{
				GW::SendDialog( clicked->dialog );
				GW::WriteChat(
					GW::PARTY, "Sent Dialog "s + clicked->description );

				clicked = std::nullopt;
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopStyleColor( 3 );

			ImGui::SameLine();
			if ( ImGui::Button( "Cancel", { size, 0 } ) )
			{
				clicked = std::nullopt;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

}


April::Gui::Settings_Dialogs::Settings_Dialogs(
	std::shared_ptr<Config> config )
	: config{ config }
{
}

void April::Gui::Settings_Dialogs::Draw()
{
	ImGui::TextWrapped(
		"Dialogs are the way your game sends requests to the server when "
		"buttons are clicked in Dialog-windows; e.g. when taking a quest.\n"
		"This module can bypass the need for the in-game button and thus "
		"allows sending the request when the game would not give the "
		"option.\n\n"
		"Be aware this is one of the clearest forms of hacking and should be "
		"used with caution or not at all." );

	ImGui::SeparatorSpaced();

	auto const& inner = ImGui::GetStyle().ItemInnerSpacing;
	auto const& outer = ImGui::GetStyle().ItemSpacing;
	ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { inner.x, outer.y } );

	auto const frame = ImGui::GetFrameHeight();
	auto const width = ImGui::CalcItemWidth() - frame - inner.x;
	auto const& grey = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];

	for (
		auto it = config->dialogs.begin(); it != config->dialogs.end(); /**/ )
	{
		auto const& dialog = *it;

		ImGui::PushStyleVar( ImGuiStyleVar_ButtonTextAlign, { -1, 0 } );
		if ( ImGui::Button( dialog.description, { width, 0 } ) )
		{
			clicked = dialog;
			ImGui::OpenPopup( popup_window_id );
		}
		ImGui::PopStyleVar();

		ImGui::SameLine();
		ImGui::PushStyleColor( ImGuiCol_Button, grey );
		if ( ImGui::Button( "X", { frame, frame } ) )
		{
			it = config->dialogs.erase( it );
		}
		else ++it;
		ImGui::PopStyleColor();
	}

	ImGui::PopStyleVar();

	if ( clicked.has_value() )
		draw_confirmation_popup( clicked );

	ImGui::SeparatorSpaced();

	ImGui::Text( "Add new Dialog" );
	ImGui::InputText( "Description", &new_dialog.description );
	ImGui::InputIntegral(
		"Dialog (Hexadecimal)", new_dialog.dialog, 0, 0,
		ImGuiInputTextFlags_CharsHexadecimal );

	if ( ImGui::Button( "Add Dialog", { ImGui::CalcItemWidth(), 0 } ) )
	{
		config->dialogs.push_back( new_dialog );
		new_dialog = {};
	}

	ImGui::SeparatorSpaced();

	auto last_dialog = GW::GetLastSentDialog();
	ImGui::InputIntegral(
		"Last sent Dialog", last_dialog, 0, 0,
		ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_CharsHexadecimal );
}
