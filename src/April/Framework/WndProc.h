#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>


namespace April {

	// returns true if msg should be suppressed
	bool WndProc( HWND, UINT, WPARAM, LPARAM );

}
