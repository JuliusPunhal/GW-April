#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>


namespace April::WndProc {

	void Initialize( HWND );
	void Terminate();

}
