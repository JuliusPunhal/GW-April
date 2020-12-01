
#include "April/Gui/Dialogs.h"

#include "April/Config/Gui/Dialogs.config.hpp"
#include "April/Framework/WndProc.h"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"

namespace config = April::Gui::DialogsConfig;

using namespace std::chrono;


April::Gui::Dialogs::Dialogs::Dialogs()
{
}

void April::Gui::Dialogs::Dialogs::Display()
{
	if ( awaiting_confirmation 
		&& steady_clock::now() - awaiting_confirmation->time 
			> config::confirm_time )
	{
		awaiting_confirmation = std::nullopt;
	}

	ImGui::Begin( config::window_name, config::window_flags );
	{
		if ( ImGui::IsWindowHovered() )
		{
			WndProc::BlockMouseInput();
		}

		ImGui::PushStyleVar( ImGuiStyleVar_ButtonTextAlign, { -1, 0 } );
		for ( auto it = 0u; it < std::size( config::dialogs ); ++it )
		{
			auto const& dialog = config::dialogs[it];

			auto const width = ImGui::GetContentRegionAvailWidth();
			if ( awaiting_confirmation && awaiting_confirmation->button == it )
			{
				using config::confirm_color;
				ImGui::PushStyleColor( ImGuiCol_Button, confirm_color );
				ImGui::PushStyleColor( ImGuiCol_ButtonActive, confirm_color );
				ImGui::PushStyleColor( ImGuiCol_ButtonHovered, confirm_color );
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
