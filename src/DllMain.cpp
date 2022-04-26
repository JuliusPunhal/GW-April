
#include "April/Framework/Features.h"
#include "April/Framework/WndProc.h"
#include "April/Utility/FontAtlas.h"

#include "GWCA/GWCA.hpp"

#include "ImGui/ImGui.hpp"

struct IDirect3DDevice9;

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>


namespace {

	auto features = std::unique_ptr<April::Features>{};

	bool running = true;
	auto gw_wndproc = WNDPROC{};


	auto WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
		-> LRESULT
	{
		auto const& mouse =
			std::get<std::shared_ptr<April::Mouse>>( *features );

		if ( April::WndProc( hWnd, msg, wParam, lParam, *mouse ) )
			return 0;

		return CallWindowProc( gw_wndproc, hWnd, msg, wParam, lParam );
	}

	void RenderCallback_Shutdown( IDirect3DDevice9* )
	{
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		GW::DisableHooks();
		running = false;
	}

	void RenderCallback( IDirect3DDevice9* )
	{
		std::get<std::shared_ptr<April::Mouse>>( *features )->restore();

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		{
			April::Update( *features );
			April::Display( *features );
		}
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData() );

		using FontAtlas = std::shared_ptr<April::FontAtlas>;
		std::get<FontAtlas>( *features )->LoadRequestedFonts();

		if ( GetAsyncKeyState( VK_END ) )
		{
			April::Shutdown( *features );

			// Delays shutdown by one frame so that Enqueue()s in
			// April::Shutdown() are called.
			GW::SetRenderCallback( RenderCallback_Shutdown );
		}
	}

	void RenderCallback_Setup( IDirect3DDevice9* device )
	{
		ImGui::CreateContext();
		ImGui_ImplWin32_Init( GW::GetWindowHandle() );
		ImGui_ImplDX9_Init( device );

		features = std::make_unique<April::Features>( April::make_Features() );

		auto entry = GW::HookEntry{};
		auto on_packet = []( GW::HookStatus& status, auto& packet )
		{
			April::Update( *features, packet );
			April::Update( *features, status, packet );
		};

		using namespace GW::Packet::StoC;
		GW::RegisterCallback<AddEffect>(             &entry, on_packet );
		GW::RegisterCallback<AddExternalBond>(       &entry, on_packet );
		GW::RegisterCallback<AgentAdd>(              &entry, on_packet );
		GW::RegisterCallback<AgentName>(             &entry, on_packet );
		GW::RegisterCallback<AgentRemove>(           &entry, on_packet );
		GW::RegisterCallback<AgentUpdateAllegiance>( &entry, on_packet );
		GW::RegisterCallback<ItemGeneral>(           &entry, on_packet );
		GW::RegisterCallback<ItemGeneral_ReuseID>(   &entry, on_packet );
		GW::RegisterCallback<MapLoaded>(             &entry, on_packet );
		GW::RegisterCallback<MessageCore>(           &entry, on_packet );
		GW::RegisterCallback<MessageGlobal>(         &entry, on_packet );
		GW::RegisterCallback<MessageLocal>(          &entry, on_packet );
		GW::RegisterCallback<MessageServer>(         &entry, on_packet );
		GW::RegisterCallback<ObjectiveAdd>(          &entry, on_packet );
		GW::RegisterCallback<ObjectiveDone>(         &entry, on_packet );
		GW::RegisterCallback<ObjectiveUpdateName>(   &entry, on_packet );
		GW::RegisterCallback<PartyDefeated>(         &entry, on_packet );
		GW::RegisterCallback<PlayerIsPartyLeader>(   &entry, on_packet );
		GW::RegisterCallback<RemoveEffect>(          &entry, on_packet );
		GW::RegisterCallback<SkillRecharge>(         &entry, on_packet );
		GW::RegisterCallback<SkillRecharged>(        &entry, on_packet );
		GW::RegisterCallback<SpeechBubble>(          &entry, on_packet );
		GW::RegisterCallback<UpdateGuildInfo>(       &entry, on_packet );

		GW::RegisterLocalMessageCallback(
			&entry,
			[]( GW::HookStatus& status, GW::LocalMessageInfo msg )
			{
				April::Update( *features, status, msg );
			} );

		GW::RegisterSendChatCallback(
			&entry,
			[]( GW::HookStatus& status, GW::SendChatInfo info )
			{
				April::Update( *features, status, info );
			} );

		gw_wndproc =
			(WNDPROC)SetWindowLongPtr(
				GW::GetWindowHandle(), GWLP_WNDPROC, (LONG)WndProc );

		GW::SetRenderCallback( RenderCallback );
	}

	void ResetCallback( IDirect3DDevice9* )
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
	}

	void Init( HINSTANCE hDll )
	{
		GW::InitializeEx(); Sleep( 100 );
		GW::SetRenderCallback( RenderCallback_Setup );
		GW::SetResetCallback( ResetCallback );

		while ( running )
		{
			Sleep( 100 );
		}

		SetWindowLongPtr(
			GW::GetWindowHandle(), GWLP_WNDPROC, (LONG)gw_wndproc );

		GW::Terminate(); Sleep( 100 );
		FreeLibraryAndExitThread( hDll, 0 );
	}

}


BOOL WINAPI DllMain( HINSTANCE hDll, DWORD fdwReason, LPVOID )
{
	if ( fdwReason == DLL_PROCESS_ATTACH )
	{
		DisableThreadLibraryCalls( hDll );
		CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)Init, hDll, 0, 0 );
	}
	return TRUE;
}
