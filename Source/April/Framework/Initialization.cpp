
#include "April/Framework/Initialization.h"

#include "April/Framework/Instance.h"
#include "April/Framework/WndProc.h"
#include "April/Utility/FileIO.h"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"

#include <filesystem>
#include <memory>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace fs = std::filesystem;


namespace {

	auto instance = std::unique_ptr<April::Instance>{};

	auto die = false;
	auto running = true;


	void default_style( ImGuiStyle& style )
	{
		style.Alpha                     = 1.0f;

		style.WindowPadding             = ImVec2{ 8, 8 };
		style.WindowRounding            = 3.f;
		style.WindowBorderSize          = 1.0f;
		style.WindowMinSize             = ImVec2{ 32, 32 };
		style.WindowTitleAlign          = ImVec2{ 0.0f, 0.5f };
		style.WindowMenuButtonPosition  = ImGuiDir_Left;

		style.ChildRounding             = 3.f;
		style.ChildBorderSize           = 1.0f;

		style.PopupRounding             = 3.f;
		style.PopupBorderSize           = 1.0f;

		style.FramePadding              = ImVec2{ 4, 3 };
		style.FrameRounding             = 2.f;
		style.FrameBorderSize           = 0.0f;

		style.ItemSpacing               = ImVec2{ 8, 4 };
		style.ItemInnerSpacing          = ImVec2{ 4, 4 };

		style.TouchExtraPadding         = ImVec2{ 0, 0 };

		style.IndentSpacing             = 26.0f;
		style.ColumnsMinSpacing         = 6.0f;

		style.ScrollbarSize             = 14.0f;
		style.ScrollbarRounding         = 2.f;

		style.GrabMinSize               = 10.0f;
		style.GrabRounding              = 2.f;

		style.LogSliderDeadzone         = 4.0f;

		style.TabRounding               = 2.f;
		style.TabBorderSize             = 0.0f;
		style.TabMinWidthForCloseButton = 0.0f;

		style.ColorButtonPosition       = ImGuiDir_Right;

		style.ButtonTextAlign           = ImVec2{ 0.5f, 0.5f };
		style.SelectableTextAlign       = ImVec2{ 0.0f, 0.0f };

		style.DisplayWindowPadding      = ImVec2{ 19, 19 };
		style.DisplaySafeAreaPadding    = ImVec2{ 3, 3 };

		style.MouseCursorScale          = 1.0f;

		style.AntiAliasedLines          = true;
		style.AntiAliasedLinesUseTex    = true;
		style.AntiAliasedFill           = true;

		style.CurveTessellationTol      = 1.25f;
		style.CircleSegmentMaxError     = 1.60f;


		using namespace April;
		auto const neutral_darker = RGBA{ 15/255.f, 15/255.f, 15/255.f, 0.9f };
		auto const neutral_dark = RGBA{ 76/255.f, 76/255.f, 76/255.f, 1 };
		auto const neutral_light = RGBA{ 127/255.f, 127/255.f, 127/255.f, 1 };
		auto const neutral_lighter = RGBA{ 186/255.f, 186/255.f, 186/255.f, 1 };

		auto const accent_dark = RGBA{ 23/255.f, 105/255.f, 6/255.f, 1 };
		auto const accent_light = RGBA{ 31/255.f, 148/255.f, 29/255.f, 1 };
		auto const accent_lighter = RGBA{ 21/255.f, 187/255.f, 37/255.f, 1 };

		auto& col = style.Colors;
		col[ImGuiCol_Text]                  = White();
		col[ImGuiCol_TextDisabled]          = { 0.5f, 0.5f, 0.5f, 1 };
		col[ImGuiCol_TextSelectedBg]        = accent_dark;
		col[ImGuiCol_DragDropTarget]        = accent_lighter;

		col[ImGuiCol_WindowBg]              = neutral_darker;
		col[ImGuiCol_ChildBg]               = Invisible();
		col[ImGuiCol_MenuBarBg]             = neutral_darker;
		col[ImGuiCol_PopupBg]               = neutral_darker;

		col[ImGuiCol_Border]                = neutral_light;
		col[ImGuiCol_BorderShadow]          = Invisible();

		col[ImGuiCol_FrameBg]               = neutral_dark;
		col[ImGuiCol_FrameBgHovered]        = neutral_light;
		col[ImGuiCol_FrameBgActive]         = neutral_lighter;

		col[ImGuiCol_TitleBg]               = neutral_dark;
		col[ImGuiCol_TitleBgActive]         = accent_dark;
		col[ImGuiCol_TitleBgCollapsed]      = neutral_darker;

		col[ImGuiCol_ScrollbarBg]           = neutral_darker;
		col[ImGuiCol_ScrollbarGrab]         = neutral_dark;
		col[ImGuiCol_ScrollbarGrabHovered]  = neutral_light;
		col[ImGuiCol_ScrollbarGrabActive]   = neutral_lighter;

		col[ImGuiCol_CheckMark]             = accent_lighter;

		col[ImGuiCol_SliderGrab]            = accent_light;
		col[ImGuiCol_SliderGrabActive]      = accent_lighter;

		col[ImGuiCol_Button]                = accent_dark;
		col[ImGuiCol_ButtonHovered]         = accent_light;
		col[ImGuiCol_ButtonActive]          = accent_lighter;

		col[ImGuiCol_Header]                = accent_dark;
		col[ImGuiCol_HeaderHovered]         = accent_light;
		col[ImGuiCol_HeaderActive]          = accent_lighter;

		col[ImGuiCol_Separator]             = neutral_light;
		col[ImGuiCol_SeparatorHovered]      = neutral_lighter;
		col[ImGuiCol_SeparatorActive]       = neutral_lighter;

		col[ImGuiCol_ResizeGrip]            = neutral_dark;
		col[ImGuiCol_ResizeGripHovered]     = neutral_light;
		col[ImGuiCol_ResizeGripActive]      = neutral_lighter;

		col[ImGuiCol_Tab]                   = neutral_dark;
		col[ImGuiCol_TabHovered]            = neutral_light;
		col[ImGuiCol_TabActive]             = accent_dark;
		col[ImGuiCol_TabUnfocused]          = neutral_dark;
		col[ImGuiCol_TabUnfocusedActive]    = accent_dark;

		col[ImGuiCol_PlotLines]             = accent_light;
		col[ImGuiCol_PlotLinesHovered]      = accent_lighter;
		col[ImGuiCol_PlotHistogram]         = accent_light;
		col[ImGuiCol_PlotHistogramHovered]  = accent_lighter;

		//col[ImGuiCol_NavHighlight]          = { 0.26f, 0.59f, 0.98f, 1.00f };
		//col[ImGuiCol_NavWindowingHighlight] = { 1.00f, 1.00f, 1.00f, 0.70f };
		//col[ImGuiCol_NavWindowingDimBg]     = { 0.80f, 0.80f, 0.80f, 0.20f };
		//col[ImGuiCol_ModalWindowDimBg]      = { 0.80f, 0.80f, 0.80f, 0.35f };
	}

	template<typename T>
	auto load_config()
	{
		auto const config = April::IO::from_file<T>( T::path );
		if ( config == std::nullopt )
			return T::LoadDefault();

		return *config;
	}


	void RenderCallback_Shutdown( IDirect3DDevice9* )
	{
		GW::DisableHooks();
		running = false;
	}

	void RenderCallback( IDirect3DDevice9* )
	{
		April::WndProc::RestoreMouseInput();

		instance->Update();

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		{
			instance->Display();
		}
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData() );

		// Terminate
		if ( die || GetAsyncKeyState( VK_END ) )
		{
			ImGui_ImplDX9_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();

			instance->Shutdown();

			GW::Render::SetRenderCallback( RenderCallback_Shutdown );
		}
	}

	void RenderCallback_Setup( IDirect3DDevice9* device )
	{
		ImGui::CreateContext();
		ImGui_ImplWin32_Init( GW::MemoryMgr::GetGWWindowHandle() );
		ImGui_ImplDX9_Init( device );

		if ( not fs::directory_entry( "April" ).exists() )
		{
			fs::create_directory( "April" );
		}

		auto& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\Consola.ttf", 14.f );
		io.IniFilename = "April\\imgui.ini";
		default_style( ImGui::GetStyle() );


		using namespace April;

		auto config = ModuleConfigurations{
			{
				load_config<ConsumablesMgr::Config>(),
			},
			{
				load_config<AgentFilter::Config>(),
				load_config<ChatCommands::Config>(),
				load_config<ChatFilter::Config>(),
				load_config<NotifyEffectLoss::Config>(),
				load_config<ReturnToOutpost::Config>(),
				load_config<ShowKitUses::Config>(),
				load_config<SuppressSpeechBubbles::Config>(),
			},
			{
				load_config<Gui::ChainedSoulGui::Config>(),
				load_config<Gui::DhuumBotGui::Config>(),
				load_config<Gui::DhuumInfo::Config>(),
				load_config<Gui::Dialogs::Config>(),
				load_config<Gui::Energybar::Config>(),
				load_config<Gui::Healthbar::Config>(),
				load_config<Gui::InstanceTimer::Config>(),
				load_config<Gui::Inventory::Config>(),
				load_config<Gui::Settings::Config>(),
				load_config<Gui::Skillbar::Config>(),
				load_config<Gui::TargetInfo::Config>(),
				load_config<Gui::UwTimesGui::Config>(),
			}
		};

		auto modules = Instance::Modules{
			ConsumablesMgr{},
			ChainedSoul{},
			DhuumBot{},
			DhuumsJudgement{},
			UwTimer{},
			WindowMgr{},
			AgentFilter{},
			ChatCommands{},
			ChatFilter{},
			CursorFix{},
			NotifyEffectLoss{},
			ReturnToOutpost{},
			ShowKitUses{},
			SuppressSpeechBubbles{},
			Gui::ChainedSoulGui{},
			Gui::Energybar{
				std::get<Gui::Energybar::Config>( config.gui )
			},
			Gui::DhuumBotGui{},
			Gui::DhuumInfo{},
			Gui::Dialogs{},
			Gui::Healthbar{
				std::get<Gui::Healthbar::Config>( config.gui )
			},
			Gui::InstanceTimer{
				std::get<Gui::InstanceTimer::Config>( config.gui )
			},
			Gui::Inventory{
				std::get<Gui::Inventory::Config>( config.gui )
			},
			Gui::Settings{},
			Gui::Skillbar{
				std::get<Gui::Skillbar::Config>( config.gui )
			},
			Gui::TargetInfo{},
			Gui::UwTimesGui{}
		};

		instance =
			std::make_unique<Instance>(
				std::move( modules ), std::move( config ) );


		GW::Render::SetRenderCallback( RenderCallback );
	}

	void ResetCallback( IDirect3DDevice9* )
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
	}

}


void April::Run()
{
	GW::Initialize(); Sleep( 100 );
	GW::Render::SetRenderCallback( RenderCallback_Setup );
	GW::Render::SetResetCallback( ResetCallback );

	WndProc::Initialize( GW::MemoryMgr::GetGWWindowHandle() );

	while ( running )
	{
		Sleep( 100 );
	}

	WndProc::Terminate();
	GW::Terminate(); Sleep( 100 );
}

void April::Die()
{
	die = true;
}
