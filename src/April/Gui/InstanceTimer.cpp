
#include "April/Gui/InstanceTimer.h"

#include "April/Utility/TimeFormatting.h"

#include "GWCA/GWCA.hpp"


namespace {

	constexpr auto WINDOW_FLAGS =
		ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoScrollWithMouse
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_AlwaysAutoResize
		| ImGuiWindowFlags_NoBackground
		| ImGuiWindowFlags_NoFocusOnAppearing
		| ImGuiWindowFlags_NoBringToFrontOnFocus;

}


April::Gui::InstanceTimer::InstanceTimer(
	ImFont* font, std::shared_ptr<Mouse> mouse )
	: font{ font }, mouse{ mouse }
{
}

void April::Gui::InstanceTimer::Display() const
{
	auto const label = to_string_hmmss( GW::GetInstanceTime() );

	ImGui::Begin( "Instance Timer", nullptr, WINDOW_FLAGS );
	ImGui::PushFont( font );
	{
		if ( ImGui::IsWindowHovered() )
			mouse->suppress();

		auto const label = to_string_hmmss( GW::GetInstanceTime() );
		auto const cursor = ImGui::GetCursorPos();

		// Shadow
		ImGui::SetCursorPos( cursor + ImVec2{ 2, 2 } );
		ImGui::PushStyleColor( ImGuiCol_Text, Black() );
		{
			ImGui::Text( label );
		}
		ImGui::PopStyleColor();

		// Button
		ImGui::SetCursorPos( cursor );
		ImGui::PushStyleColor( ImGuiCol_Text, White() );
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
