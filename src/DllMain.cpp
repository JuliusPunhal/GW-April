
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>


namespace {

	void Init( HINSTANCE hDll )
	{
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
