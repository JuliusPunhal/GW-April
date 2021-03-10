
#include "April/Framework/Instance.h"
#include "April/Framework/WndProc.h"

#include <memory>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>


namespace {

	auto instance = std::shared_ptr<April::Instance>{};

	auto running = true;


	void RenderCallback_Shutdown( IDirect3DDevice9* )
	{
		GW::DisableHooks();
		running = false;
	}

	void RenderCallback( IDirect3DDevice9* )
	{
		April::WndProc::RestoreMouseInput();

		Update( *instance );
		Display( *instance );

		if ( instance->terminate || GetAsyncKeyState( VK_END ) )
		{
			Shutdown( *instance );
			GW::Render::SetRenderCallback( RenderCallback_Shutdown );
		}
	}

	void RenderCallback_Setup( IDirect3DDevice9* device )
	{
		instance = April::make_instance( device );

		GW::Render::SetRenderCallback( RenderCallback );
	}

	void ResetCallback( IDirect3DDevice9* device )
	{
		April::OnDeviceReset( device );
	}

}


void Init( HINSTANCE hDll )
{
	GW::Initialize(); Sleep( 100 );
	GW::Render::SetRenderCallback( RenderCallback_Setup );
	GW::Render::SetResetCallback( ResetCallback );

	April::WndProc::Initialize( GW::MemoryMgr::GetGWWindowHandle() );

	while ( running )
	{
		Sleep( 100 );
	}

	April::WndProc::Terminate();
	GW::Terminate(); Sleep( 100 );

	FreeLibraryAndExitThread( hDll, 0 );
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
