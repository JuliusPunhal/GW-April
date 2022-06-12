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
	void HelpMarker( const char* );
	void HelpMarker( std::string const& );
	void MoveCursorPos( ImVec2 const& );
	void MoveCursorPosX( float );
	void MoveCursorPosY( float );
	void SeparatorSpaced( float = 0.5f, float = 0.5f );
	void SetNextWindowPosCentered( ImGuiCond );
	bool SmallButton( std::string const& );
	void Text( std::string const& );
	void TextCentered( const char* );
	void TextCentered( std::string const& );
	void TextWrapped( std::string const& );
	float WidthToFitNItems( int );


	template<typename T>
	bool InputIntegral(
		const char* label,
		T& val,
		int const step = 1,
		int const step_fast = 100,
		ImGuiInputTextFlags const flags = 0 )
	{
		auto temp = static_cast<int>( val );

		auto const value_changed =
			ImGui::InputInt( label, &temp, step, step_fast, flags );

		val = static_cast<T>( temp );

		return value_changed;
	}

	template<typename T>
	bool InputDuration(
		const char* label,
		T& dur,
		int const step = 1,
		int const step_fast = 100,
		ImGuiInputTextFlags const flags = 0 )
	{
		auto temp = dur.count();

		auto const value_changed =
			ImGui::InputIntegral( label, temp, step, step_fast, flags );

		dur = T{ temp };

		return value_changed;
	}

}

namespace April {

	using ImGui::Window;


	auto Invisible() -> ImVec4;

	auto Black( float alpha = 1 ) -> ImVec4;
	auto Grey( float alpha = 1 ) -> ImVec4;
	auto White( float alpha = 1 ) -> ImVec4;

	auto Blue( float alpha = 1 ) -> ImVec4;
	auto DarkGreen( float alpha = 1 ) -> ImVec4;
	auto DarkRed( float alpha = 1 ) -> ImVec4;
	auto Green( float alpha = 1 ) -> ImVec4;
	auto Red( float alpha = 1 ) -> ImVec4;
	auto Yellow( float alpha = 1 ) -> ImVec4;

	auto MutedRed( float alpha = 1 ) -> ImVec4;


	auto operator+( ImVec2 const&, ImVec2 const& ) -> ImVec2;

}
