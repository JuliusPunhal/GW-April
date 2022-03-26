
cmake_minimum_required( VERSION 3.19.0 )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )


project( GW-April )
add_library( GW-April SHARED )

target_include_directories(
	GW-April
	PRIVATE
		src/
		external/ImGui/ )

target_sources(
	GW-April
	PRIVATE
		src/DllMain.cpp
		src/April/Framework/WndProc.h
		src/April/Framework/WndProc.cpp
		src/April/Gui/InstanceTimer.h
		src/April/Gui/InstanceTimer.cpp
		src/April/Utility/Fonts.h
		src/April/Utility/Fonts.cpp
		src/April/Utility/Mouse.h
		src/April/Utility/Mouse.cpp
		src/April/Utility/TimeFormatting.h
		src/April/Utility/TimeFormatting.cpp
		src/GWCA/GWCA.hpp
		src/GWCA/GWCA.cpp
		src/ImGui/ImGui.hpp
		src/ImGui/ImGui.cpp
		external/ImGui/imgui.h
		external/ImGui/imgui_internal.h
		external/ImGui/imgui.cpp
		external/ImGui/imgui_demo.cpp
		external/ImGui/imgui_draw.cpp
		external/ImGui/imgui_tables.cpp
		external/ImGui/imgui_widgets.cpp
		external/ImGui/backends/imgui_impl_dx9.h
		external/ImGui/backends/imgui_impl_dx9.cpp
		external/ImGui/backends/imgui_impl_win32.h
		external/ImGui/backends/imgui_impl_win32.cpp
		external/ImGui/misc/cpp/imgui_stdlib.h
		external/ImGui/misc/cpp/imgui_stdlib.cpp )

if ( MSVC )
	target_compile_options(
		GW-April
		PUBLIC /permissive-
		PRIVATE /W4 /MP /utf-8 )
endif()

add_subdirectory( "external/GWCA" )
target_link_libraries( GW-April PUBLIC gwca )
