
#include "April/Gui/Settings.h"

#include "April/Config/Gui/Settings.config.hpp"
#include "April/Framework/WndProc.h"

#include "Dependencies/ImGui.hpp"

#include <algorithm>
#include <limits>

namespace config = April::Gui::WindowSettingsConfig;


namespace {

	bool window_compare( ImGuiWindow const* lhs, ImGuiWindow const* rhs )
	{
		return strcmp( lhs->Name, rhs->Name ) < 0;
	}

	bool is_visible( ImGuiWindow& window )
	{
		return window.HiddenFramesCanSkipItems == 0;
	}

	void hide( ImGuiWindow& window )
	{
		using T = decltype(ImGuiWindow::HiddenFramesCanSkipItems);
		window.HiddenFramesCanSkipItems = std::numeric_limits<T>::max();
	}

	void show( ImGuiWindow& window )
	{
		window.HiddenFramesCanSkipItems = 0;
	}

	void draw_visibility( ImGuiWindow& window )
	{
		ImGui::PushID( window.Name );
		auto checked = is_visible( window );
		if ( ImGui::Checkbox( "##visibility", &checked ) )
		{
			checked ? show( window ) : hide( window );
		}
		ImGui::PopID();
	}

	void draw_settings( ImGuiWindow& window )
	{
		ImGui::PushID( window.Name );

		// Window Position
		ImGui::AlignTextToFramePadding();
		ImGui::DragFloat2( "Position", &window.Pos.x, 1, 0, 0, "%.0f" );

		// Window Size
		if ( (window.Flags & ImGuiWindowFlags_AlwaysAutoResize) == 0 )
		{
			ImGui::AlignTextToFramePadding();
			ImGui::DragFloat2( "Size", &window.SizeFull.x, 1, 0, 0, "%.0f" );
		}
		else ImGui::Text( "Turn off Auto Resize to define size" );

		// Window Flags
		auto const draw = [&window]( const char* str, unsigned const flag )
		{
			auto const is_set = (window.Flags & flag) == flag;

			auto checked = is_set;
			if ( ImGui::Checkbox( str, &checked ) )
			{
				is_set ? window.Flags &= ~flag : window.Flags |= flag;
			}
		};

		ImGui::Columns( 2, 0, false );
		draw( "Auto Resize",		ImGuiWindowFlags_AlwaysAutoResize );
		draw( "No Movement",		ImGuiWindowFlags_NoMove );
		draw( "No Resizing",		ImGuiWindowFlags_NoResize );
		draw( "No Titlebar",		ImGuiWindowFlags_NoTitleBar );
		draw( "No Collapsing",		ImGuiWindowFlags_NoCollapse );
		draw( "No Scrolling",		ImGuiWindowFlags_NoScrollWithMouse );
		ImGui::NextColumn();
		draw( "No Scrollbar",		ImGuiWindowFlags_NoScrollbar );
		draw( "Force Scrollbar",	ImGuiWindowFlags_AlwaysVerticalScrollbar );
		draw( "Enable H-Scrollbar", ImGuiWindowFlags_HorizontalScrollbar );
		draw( "Disable Mouse",		ImGuiWindowFlags_NoMouseInputs );
		draw( "Disable Keyboard",	ImGuiWindowFlags_NoNavInputs );
		draw( "Opaque Background",	ImGuiWindowFlags_NoBackground );
		ImGui::Columns();

		ImGui::PopID();
	}

	void draw_window_settings( ImGuiWindow* window )
	{
		if ( window == nullptr ) return;
		if ( window->IsFallbackWindow ) return;
		if ( window->Name[0] == '#' ) return;
			
		draw_visibility( *window );
		ImGui::SameLine();
		if ( ImGui::CollapsingHeader( window->Name ) )
		{
			ImGui::Indent();
			draw_settings( *window );
			ImGui::Unindent();
		}
	}

}


void April::Gui::Settings::Display() const
{
	ImGui::Begin( config::window_name, config::window_flags );
	{
		if ( ImGui::IsWindowHovered( ImGuiHoveredFlags_ChildWindows ) )
		{
			WndProc::BlockMouseInput();
		}
		
		if ( ImGui::CollapsingHeader( "Global Style Settings" ) )
		{
			ImGui::ShowStyleEditor();
		}

		auto windows = ImGui::GetCurrentContext()->Windows;
		std::sort( windows.begin(), windows.end(), window_compare );

		for ( auto* window : windows )
		{
			draw_window_settings( window );
		}
	}
	ImGui::End();
}
