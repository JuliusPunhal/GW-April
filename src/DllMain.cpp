
#include "April/Framework/Features.h"
#include "April/Framework/WndProc.h"

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

	void RenderCallback( IDirect3DDevice9* )
	{
		std::get<std::shared_ptr<April::Mouse>>( *features )->restore();

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		{
			April::Display( *features );
		}
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData() );

		if ( GetAsyncKeyState( VK_END ) )
		{
			ImGui_ImplDX9_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();

			GW::DisableHooks();
			running = false;
		}
	}

	void RenderCallback_Setup( IDirect3DDevice9* device )
	{
		ImGui::CreateContext();
		ImGui_ImplWin32_Init( GW::GetWindowHandle() );
		ImGui_ImplDX9_Init( device );

		features = std::make_unique<April::Features>( April::make_Features() );

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
		GW::Initialize(); Sleep( 100 );
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
