
#include "Dependencies/ImGui.hpp"


bool ImGui::Begin( April::Window const& wnd )
{
	if ( wnd.visible )
	{
		ImGui::Begin( wnd.name.c_str(), &wnd.visible, wnd.flags );
		return true;
	}
	else
	{
		constexpr auto dummy_flags =
			ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoMouseInputs;

		ImGui::SetNextWindowPos( { -1000, -1000 } );
		ImGui::Begin( "##AprilEmptyDummy", nullptr, dummy_flags );
		return false;
	}
}

void ImGui::Text( std::string const& str )
{
	return ImGui::Text( str.c_str() );
}

bool ImGui::Button( std::string const& label, ImVec2 const& size )
{
	return ImGui::Button( label.c_str(), size );
}

bool ImGui::SmallButton( std::string const& label )
{
	return ImGui::SmallButton( label.c_str() );
}
