
#include "April/Gui/InstanceTimer.h"

#include "April/Utility/TimeFormatting.h"

#include "GWCA/GWCA.hpp"


April::Gui::InstanceTimer::InstanceTimer(
	std::shared_ptr<Config>          config,
	std::shared_ptr<FontAtlas const> fonts,
	std::shared_ptr<Mouse>           mouse )
	: config{ config }, fonts{ fonts }, mouse{ mouse }
{
}

void April::Gui::InstanceTimer::Display()
{
	ImGui::SetNextWindowPosCentered( ImGuiCond_FirstUseEver );

	if ( ImGui::Begin( config->window, *mouse ) )
	{
		ImGui::PushFont( fonts->Get( config->font ) );

		auto const label = to_string_hmmss( GW::GetInstanceTime() );
		auto const cursor = ImGui::GetCursorPos();

		// Shadow
		ImGui::SetCursorPos( cursor + config->shadow_offset );
		ImGui::PushStyleColor( ImGuiCol_Text, config->shadow_color );
		{
			ImGui::Text( label );
		}
		ImGui::PopStyleColor();

		// Button
		ImGui::SetCursorPos( cursor );
		ImGui::PushStyleColor( ImGuiCol_Text, config->label_color );
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

		ImGui::PopFont();
	}
	ImGui::End();
}
