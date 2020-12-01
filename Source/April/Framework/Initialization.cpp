
#include "April/Framework/Initialization.h"

#include "April/Gui/InstanceTimer.h"

#include "April/Framework/WndProc.h"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"

#include <filesystem>
#include <memory>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace a = April;
namespace ag = April::Gui;
namespace fs = std::filesystem;


namespace {
	
	auto gui_instancetimer = std::unique_ptr<ag::InstanceTimer>{};

	auto running = true;
	

	void RenderCallback_Shutdown( IDirect3DDevice9* )
	{
		GW::DisableHooks();
		running = false;
	}

	void RenderCallback( IDirect3DDevice9* )
	{
		April::WndProc::RestoreMouseInput();

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		{
			gui_instancetimer->Display();
		}
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData() );

		// Terminate
		if ( GetAsyncKeyState( VK_END ) )
		{
			ImGui_ImplDX9_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
		
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

		gui_instancetimer = std::make_unique<ag::InstanceTimer>();

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
