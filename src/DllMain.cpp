
#include "GWCA/GWCA.hpp"

struct IDirect3DDevice9;

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>


namespace {

	bool running = true;


	void RenderCallback( IDirect3DDevice9* )
	{
		if ( GetAsyncKeyState( VK_END ) )
		{
			GW::DisableHooks();
			running = false;
		}
	}

	void ResetCallback( IDirect3DDevice9* )
	{
	}

	void Init( HINSTANCE hDll )
	{
		GW::Initialize(); Sleep( 100 );
		GW::SetRenderCallback( RenderCallback );
		GW::SetResetCallback( ResetCallback );

		while ( running )
		{
			Sleep( 100 );
		}

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
