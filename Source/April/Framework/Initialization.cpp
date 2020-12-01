
#include "April/Framework/Initialization.h"

#include "April/Module/ChatCommands.h"
#include "April/Module/ChatFilter.h"
#include "April/Module/ConsumablesMgr.h"
#include "April/Module/UwTimer.h"

#include "April/Gui/HealthEnergy.h"
#include "April/Gui/InstanceTimer.h"
#include "April/Gui/Inventory.h"
#include "April/Gui/Skillbar.h"
#include "April/Gui/UwTimes.h"

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

	auto chatcommands = std::unique_ptr<a::ChatCommands>{};
	auto chatfilter = std::unique_ptr<a::ChatFilter>{};
	auto consumables_mgr = std::shared_ptr<a::ConsumablesMgr>{};
	auto uw_timer = std::unique_ptr<a::UwTimer>{};

	auto gui_energy = std::unique_ptr<ag::Energybar>{};
	auto gui_health = std::unique_ptr<ag::Healthbar>{};
	auto gui_instancetimer = std::unique_ptr<ag::InstanceTimer>{};
	auto gui_inventory = std::unique_ptr<ag::Inventory>{};
	auto gui_skillbar = std::unique_ptr<ag::Skillbar>{};
	auto gui_uwtimer = std::unique_ptr<ag::UwTimesGui>{};

	auto running = true;
	

	void RenderCallback_Shutdown( IDirect3DDevice9* )
	{
		GW::DisableHooks();
		running = false;
	}

	void RenderCallback( IDirect3DDevice9* )
	{
		April::WndProc::RestoreMouseInput();

		consumables_mgr->Update();
		uw_timer->Update();

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		{
			gui_energy->Display();
			gui_health->Display();
			gui_instancetimer->Display();
			gui_inventory->Display();
			gui_skillbar->Display();
			gui_uwtimer->Display();
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

		auto const uw_times = std::make_shared<a::UwTimes>();

		consumables_mgr = std::make_shared<a::ConsumablesMgr>();
		chatcommands = std::make_unique<a::ChatCommands>( consumables_mgr );
		chatfilter = std::make_unique<a::ChatFilter>();
		uw_timer = std::make_unique<a::UwTimer>( uw_times );

		gui_energy = std::make_unique<ag::Energybar>();
		gui_health = std::make_unique<ag::Healthbar>();
		gui_instancetimer = std::make_unique<ag::InstanceTimer>();
		gui_inventory = std::make_unique<ag::Inventory>( consumables_mgr );
		gui_skillbar = std::make_unique<ag::Skillbar>();
		gui_uwtimer = std::make_unique<ag::UwTimesGui>( uw_times );

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
