
#include "Dependencies/ImGui.hpp"


void ImGui::Begin( char const* name, ImGuiWindowFlags const flags )
{
	auto const* wnd = FindWindowByName( name );
	if ( wnd )
		Begin( name, nullptr, wnd->Flags );
	else
		Begin( name, nullptr, flags );
}

void ImGui::Begin( std::string const& name, ImGuiWindowFlags const flags )
{
	Begin( name.c_str(), flags );
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
