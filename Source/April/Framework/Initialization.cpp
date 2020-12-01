
#include "April/Framework/Initialization.h"

#include "April/Module/AgentFilter.h"
#include "April/Module/ChatCommands.h"
#include "April/Module/ChatFilter.h"
#include "April/Module/ConsumablesMgr.h"
#include "April/Module/CursorFix.h"
#include "April/Module/DhuumsJudgement.h"
#include "April/Module/NotifyEffectLoss.h"
#include "April/Module/ReturnToOutpost.h"
#include "April/Module/ShowKitUses.h"
#include "April/Module/SuppressSpeechBubbles.h"
#include "April/Module/UwTimer.h"

#include "April/Gui/DhuumInfo.h"
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

	auto agentfilter = std::unique_ptr<a::AgentFilter>{};
	auto chatcommands = std::unique_ptr<a::ChatCommands>{};
	auto chatfilter = std::unique_ptr<a::ChatFilter>{};
	auto consumables_mgr = std::shared_ptr<a::ConsumablesMgr>{};
	auto cursorfix = std::unique_ptr<a::CursorFix>{};
	auto dhuums_judgement = std::shared_ptr<a::DhuumsJudgement>{};
	auto notify_effect_lost = std::unique_ptr<a::NotifyEffectLoss>{};
	auto return_to_outpost = std::unique_ptr<a::ReturnToOutpost>{};
	auto show_kit_uses = std::unique_ptr<a::ShowKitUses>{};
	auto suppress_speech_bubbles = std::unique_ptr<a::SuppressSpeechBubbles>{};
	auto uw_timer = std::unique_ptr<a::UwTimer>{};

	auto gui_energy = std::unique_ptr<ag::Energybar>{};
	auto gui_dhuuminfo = std::unique_ptr<ag::DhuumInfo>{};
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
		dhuums_judgement->Update();
		uw_timer->Update();

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		{
			gui_energy->Display();
			gui_dhuuminfo->Display();
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

			agentfilter->DisplaySuppressedItems();

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

		agentfilter = std::make_unique<a::AgentFilter>();
		consumables_mgr = std::make_shared<a::ConsumablesMgr>();
		chatcommands = std::make_unique<a::ChatCommands>( consumables_mgr );
		chatfilter = std::make_unique<a::ChatFilter>();
		cursorfix = std::make_unique<a::CursorFix>();
		dhuums_judgement = std::make_shared<a::DhuumsJudgement>();
		notify_effect_lost = std::make_unique<a::NotifyEffectLoss>();
		return_to_outpost = std::make_unique<a::ReturnToOutpost>();
		show_kit_uses = std::make_unique<a::ShowKitUses>();
		suppress_speech_bubbles = std::make_unique<a::SuppressSpeechBubbles>();
		uw_timer = std::make_unique<a::UwTimer>( uw_times );

		gui_energy = std::make_unique<ag::Energybar>();
		gui_dhuuminfo = std::make_unique<ag::DhuumInfo>( dhuums_judgement );
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
