
#include "April/Gui/Dialogs.h"

#include "April/Framework/WndProc.h"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"

using namespace std::chrono;


April::Gui::Dialogs::Dialogs::Dialogs( Config const& config )
	: config{ config }
{
}

void April::Gui::Dialogs::Dialogs::Display()
{
	if ( awaiting_confirmation
		&& steady_clock::now() - awaiting_confirmation->time
			> config.confirm_time )
	{
		awaiting_confirmation = std::nullopt;
	}

	if ( ImGui::Begin( config.window ) )
	{
		if ( ImGui::IsWindowHovered() )
		{
			WndProc::BlockMouseInput();
		}

		ImGui::PushStyleVar( ImGuiStyleVar_ButtonTextAlign, { -1, 0 } );
		for ( auto it = 0u; it < config.dialogs.size(); ++it )
		{
			auto const& dialog = config.dialogs[it];

			auto const width = ImGui::GetContentRegionAvailWidth();
			if ( awaiting_confirmation && awaiting_confirmation->button == it )
			{
				ImGui::PushStyleColor( ImGuiCol_Button, config.confirm_color );
				ImGui::PushStyleColor(
					ImGuiCol_ButtonActive, config.confirm_color );
				ImGui::PushStyleColor(
					ImGuiCol_ButtonHovered, config.confirm_color );

				if ( ImGui::Button( "Confirm", { width, 0 } ) )
				{
					GW::Agents::SendDialog( dialog.dialog );
					awaiting_confirmation = std::nullopt;
				}

				ImGui::PopStyleColor( 3 );
			}
			else if ( ImGui::Button( dialog.description, { width, 0 } ) )
			{
				awaiting_confirmation = { it, steady_clock::now() };
			}
		}
		ImGui::PopStyleVar();

		auto const last_dialog = GW::Agents::GetLastDialogId();
		ImGui::Text( "Last Dialog: %d (0x%08X)", last_dialog, last_dialog );
	}
	ImGui::End();
}

auto April::Gui::Dialogs::Config::LoadDefault() -> Config
{
	using namespace GW::Constants;

	auto dialogs = std::vector<Dialog>{
		{ "Take Quest: Slayer",
			( QuestID::UW::Mnt << 8 ) | 0x800001 },

		{ "Take Quest: Four Horsemen",
			( QuestID::UW::Planes << 8 ) | 0x800001 }
	};

	return Config{
		std::move( dialogs ),
		3s,
		{ 0.7f, 0, 0, 1 },
		{ "Dialogs", false, ImGuiWindowFlags_None }
	};
}
