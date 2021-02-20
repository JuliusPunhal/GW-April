
#include "April/Gui/InstanceTimer.h"

#include "April/Framework/WndProc.h"
#include "April/Utility/TimeFormatting.h"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"


April::Gui::InstanceTimer::InstanceTimer( Config const& config )
	: config{ config }, font{ LoadFont( config.font ) }
{
}

void April::Gui::InstanceTimer::Display() const
{
	// Update
	auto const label = to_string( GW::GetInstanceTime(), HMMSS );

	// Draw
	if ( ImGui::Begin( config.window ) )
	{
		ImGui::PushFont( font );
		if ( ImGui::IsWindowHovered() )
		{
			WndProc::BlockMouseInput();
		}

		auto const cursor = ImGui::GetCursorPos();

		// Shadow
		ImGui::SetCursorPos( cursor + config.shadow_offset );
		ImGui::PushStyleColor( ImGuiCol_Text, config.shadow_color );
		{
			ImGui::Text( label );
		}
		ImGui::PopStyleColor();

		// Button
		ImGui::SetCursorPos( cursor );
		ImGui::PushStyleColor( ImGuiCol_Text, config.text_color );
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

auto April::Gui::InstanceTimer::Config::LoadDefault() -> Config
{
	constexpr auto wnd_flags =
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

	return Config{
		{ "C:\\Windows\\Fonts\\Consola.ttf", 30 },
		White(),
		Black(),
		XY{ 1, 1 },
		{ "Instance Time", true, wnd_flags }
	};
}
