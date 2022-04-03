
#include "ImGui/ImGui.hpp"


bool ImGui::Begin( Window& wnd )
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

		ImGui::Begin( "###GW-April Empty Window", nullptr, dummy_flags );
		return false;
	}
}

bool ImGui::Begin( Window& wnd, April::Mouse& mouse )
{
	auto const visible = Begin( wnd );

	if ( visible && ImGui::IsWindowHovered( ImGuiHoveredFlags_ChildWindows ) )
		mouse.suppress();

	return visible;
}

bool ImGui::Button( std::string const& label, ImVec2 const& size )
{
	return ImGui::Button( label.c_str(), size );
}

bool ImGui::CollapsingHeader(
	std::string const& label, ImGuiTreeNodeFlags const flags )
{
	return ImGui::CollapsingHeader( label.c_str(), flags );
}

void ImGui::MoveCursorPos( ImVec2 const& new_pos )
{
	auto const cursor = ImGui::GetCursorPos();
	ImGui::SetCursorPos( { cursor.x + new_pos.x, cursor.y + new_pos.y } );
}

void ImGui::MoveCursorPosX( float const new_x )
{
	ImGui::SetCursorPosX( ImGui::GetCursorPosX() + new_x );
}

void ImGui::MoveCursorPosY( float const new_y )
{
	ImGui::SetCursorPosY( ImGui::GetCursorPosY() + new_y);
}

void ImGui::SetNextWindowPosCentered( ImGuiCond const cond )
{
	auto const& display = ImGui::GetIO().DisplaySize;
	auto const size = ImVec2{ display.x * 0.5f, display.y * 0.5f };

	ImGui::SetNextWindowPos( size, cond, { 0.5f, 0.5f } );
}

bool ImGui::SmallButton( std::string const& label )
{
	return ImGui::SmallButton( label.c_str() );
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

auto April::Grey( float alpha ) -> ImVec4
{
	return { .5f, .5f, .5f, alpha };
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
