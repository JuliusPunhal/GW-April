#pragma once

#pragma warning( push, 0 )
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_dx9.h"
#include "backends/imgui_impl_win32.h"
#include "misc/cpp/imgui_stdlib.h"
#pragma warning( pop )

#include "April/Utility/Mouse.h"

#include <string>


namespace ImGui {

	struct Window {
		std::string      name;
		bool             visible;
		ImGuiWindowFlags flags;
	};


	bool Begin( Window& );
	bool Begin( Window&, April::Mouse& );
	bool Button( std::string const&, ImVec2 const& = { 0, 0 } );
	bool CollapsingHeader( std::string const&, ImGuiTreeNodeFlags = 0 );
	void SetNextWindowPosCentered( ImGuiCond );
	bool SmallButton( std::string const& );
	void Text( std::string const& );

}

namespace April {

	using ImGui::Window;


	auto Invisible() -> ImVec4;

	auto Black( float alpha = 1 ) -> ImVec4;
	auto Grey( float alpha = 1 ) -> ImVec4;
	auto White( float alpha = 1 ) -> ImVec4;

	auto Blue( float alpha = 1 ) -> ImVec4;
	auto Green( float alpha = 1 ) -> ImVec4;
	auto Red( float alpha = 1 ) -> ImVec4;
	auto Yellow( float alpha = 1 ) -> ImVec4;


	auto operator+( ImVec2 const&, ImVec2 const& ) -> ImVec2;

}
