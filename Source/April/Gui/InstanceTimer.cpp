
#include "April/Gui/InstanceTimer.h"

#include "April/Config/Gui/InstanceTimer.config.hpp"
#include "April/Framework/WndProc.h"
#include "April/Utility/TimeFormatting.h"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"

namespace config = April::Config::Gui::InstanceTimer;


April::Gui::InstanceTimer::InstanceTimer()
	: font{ LoadFont( config::font_path, config::font_size ) }
{
}

void April::Gui::InstanceTimer::Display() const
{
	// Update
	auto const label = to_string( GW::GetInstanceTime(), config::time_format );
	
	// Draw
	ImGui::Begin( config::window_name, config::window_flags );
	ImGui::PushFont( font );
	{
		if ( ImGui::IsWindowHovered() )
		{
			WndProc::BlockMouseInput();
		}

		auto const cursor = ImGui::GetCursorPos();

		// Shadow
		ImGui::SetCursorPos( cursor + config::shadow_offset );
		ImGui::PushStyleColor( ImGuiCol_Text, config::shadow_color );
		{
			ImGui::Text( label );
		}
		ImGui::PopStyleColor();

		// Button 
		ImGui::SetCursorPos( cursor );
		ImGui::PushStyleColor( ImGuiCol_Text, config::text_color );
		ImGui::PushStyleColor( ImGuiCol_Button, Invisible() );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, Invisible() );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, Invisible() );
		ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, { 0, 0 } );
		if ( ImGui::Button( label ) )
		{
			GW::SendChat( '#', "[Now] " + label );
		}
		ImGui::PopStyleColor( 4 );
		ImGui::PopStyleVar();
	}
	ImGui::PopFont();
	ImGui::End();
}
