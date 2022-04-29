
cmake_minimum_required( VERSION 3.19.0 )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )


project( GW-April )
add_library( GW-April SHARED )

target_include_directories(
	GW-April
	PRIVATE
		src/
		external/ImGui/
		external/json/single_include/ )

target_sources(
	GW-April
	PRIVATE
		src/DllMain.cpp
		src/April/Framework/Features.h
		src/April/Framework/Features.cpp
		src/April/Framework/WndProc.h
		src/April/Framework/WndProc.cpp
		src/April/Gui/ChainedSouls.h
		src/April/Gui/ChainedSouls.cpp
		src/April/Gui/DhuumInfo.h
		src/April/Gui/DhuumInfo.cpp
		src/April/Gui/HealthEnergy.h
		src/April/Gui/HealthEnergy.cpp
		src/April/Gui/InstanceTimer.h
		src/April/Gui/InstanceTimer.cpp
		src/April/Gui/Inventory.h
		src/April/Gui/Inventory.cpp
		src/April/Gui/Skillbar.h
		src/April/Gui/Skillbar.cpp
		src/April/Gui/UwTimer.h
		src/April/Gui/UwTimer.cpp
		src/April/Module/ChatCommands.h
		src/April/Module/ChatCommands.cpp
		src/April/Module/ChatFilter.h
		src/April/Module/ChatFilter.cpp
		src/April/Module/ConsumablesMgr.h
		src/April/Module/ConsumablesMgr.cpp
		src/April/Module/GuildEditor.h
		src/April/Module/GuildEditor.cpp
		src/April/Module/ItemFilter.h
		src/April/Module/ItemFilter.cpp
		src/April/Module/NotifyOnEffect.h
		src/April/Module/NotifyOnEffect.cpp
		src/April/Module/ResignMessage.h
		src/April/Module/ResignMessage.cpp
		src/April/Module/ReturnToOutpost.h
		src/April/Module/ReturnToOutpost.cpp
		src/April/Module/ShowKitUses.h
		src/April/Module/ShowKitUses.cpp
		src/April/Module/SuppressSpeechBubbles.h
		src/April/Module/SuppressSpeechBubbles.cpp
		src/April/Module/UwTimer.h
		src/April/Module/UwTimer.cpp
		src/April/Utility/ChainedSouls.h
		src/April/Utility/ChainedSouls.cpp
		src/April/Utility/Consumables.h
		src/April/Utility/Consumables.cpp
		src/April/Utility/ConsumableState.h
		src/April/Utility/ConsumableState.cpp
		src/April/Utility/DefaultTheme.h
		src/April/Utility/DefaultTheme.cpp
		src/April/Utility/DhuumsJudgement.h
		src/April/Utility/DhuumsJudgement.cpp
		src/April/Utility/FileIO.h
		src/April/Utility/FileIO.cpp
		src/April/Utility/FileIO.BasicTypes.h
		src/April/Utility/FileIO.BasicTypes.cpp
		src/April/Utility/FileIO.ConfigTypes.h
		src/April/Utility/FileIO.ConfigTypes.cpp
		src/April/Utility/FileIO.JsonKey.h
		src/April/Utility/FileIO.JsonKey.cpp
		src/April/Utility/FontAtlas.h
		src/April/Utility/FontAtlas.cpp
		src/April/Utility/Mouse.h
		src/April/Utility/Mouse.cpp
		src/April/Utility/Reaper.h
		src/April/Utility/Reaper.cpp
		src/April/Utility/ReducedSkillRecharge.h
		src/April/Utility/ReducedSkillRecharge.cpp
		src/April/Utility/sorted_vector.h
		src/April/Utility/sorted_vector.cpp
		src/April/Utility/stl.h
		src/April/Utility/stl.cpp
		src/April/Utility/TimeFormatting.h
		src/April/Utility/TimeFormatting.cpp
		src/April/Utility/UwTimesHistory.h
		src/April/Utility/UwTimesHistory.cpp
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
		external/ImGui/misc/cpp/imgui_stdlib.cpp
		external/json/single_include/nlohmann/json.hpp )

if ( MSVC )
	target_compile_options(
		GW-April
		PUBLIC /permissive-
		PRIVATE /W4 /MP /utf-8 )
endif()

add_subdirectory( "external/GWCA" )
target_link_libraries( GW-April PUBLIC gwca )
