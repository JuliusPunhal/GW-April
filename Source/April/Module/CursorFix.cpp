
#include "April/Module/CursorFix.h"

#include "Dependencies/GWCA.hpp"


namespace {

	BOOL WINAPI fnGetClipCursor( LPRECT lpRect )
	{
		return GetWindowRect( GW::MemoryMgr::GetGWWindowHandle(), lpRect );
	}

}


April::CursorFix::CursorFix()
{
	// Hook will only be cleaned up during GWCA shutdown.
	auto const hUser32 = GetModuleHandleA("user32.dll");
	GetClipCursor_Func = (GetClipCursor_pt)((uintptr_t)GetProcAddress( hUser32, "GetClipCursor" ));
	if ( GetClipCursor_Func )
	{
		GW::HookBase::CreateHook(
			 GetClipCursor_Func, fnGetClipCursor, (void **)&RetGetClipCursor );
		GW::HookBase::EnableHooks( GetClipCursor_Func );
	}
}

/*
April::CursorFix::~CursorFix()
{
	if ( GetClipCursor_Func )
	{
		GW::HookBase::DisableHooks( GetClipCursor_Func );
		GW::HookBase::RemoveHook( GetClipCursor_Func );
	}
}
*/
