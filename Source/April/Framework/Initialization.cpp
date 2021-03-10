
#include "April/Framework/Initialization.h"

#include "April/Framework/Instance.h"
#include "April/Framework/WndProc.h"

#include "Dependencies/GWCA.hpp"

#include <memory>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>


namespace {

	auto instance = std::shared_ptr<April::Instance>{};

	auto die = false;
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

		// Terminate
		if ( die || GetAsyncKeyState( VK_END ) )
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
