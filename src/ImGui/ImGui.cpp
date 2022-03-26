
#include "ImGui/ImGui.hpp"


bool ImGui::Button( std::string const& label, ImVec2 const& size )
{
	return ImGui::Button( label.c_str(), size );
}

void ImGui::Text( std::string const& str )
{
	ImGui::Text( str.c_str() );
}

auto April::Invisible() -> ImVec4
{
	return { 0, 0, 0, 0 };
}

auto April::Black( float alpha ) -> ImVec4
{
	return { 0, 0, 0, alpha };
}

auto April::White( float alpha ) -> ImVec4
{
	return { 1, 1, 1, alpha };
}

auto April::Blue( float alpha ) -> ImVec4
{
	return { 0, 0, 1, alpha };
}

auto April::Green( float alpha ) -> ImVec4
{
	return { 0, 1, 0, alpha };
}

auto April::Red( float alpha ) -> ImVec4
{
	return { 1, 0, 0, alpha };
}

auto April::Yellow( float alpha ) -> ImVec4
{
	return { 1, 1, 0, alpha };
}

auto April::operator+( ImVec2 const& lhs, ImVec2 const& rhs ) -> ImVec2
{
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}
