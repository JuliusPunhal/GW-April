
#include "April/Framework/WndProc.h"

#include "ImGui/ImGui.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND, UINT, WPARAM, LPARAM );


bool April::WndProc(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, Mouse const& mouse )
{
	// This is basically a copy of ImGui_ImplWin32_WndProcHandler() but
	// ::SetCapture() and ::ReleaseCapture() seem to mess with window-
	// messages thus disabling right-click + drag, i.e. camera-movement.
	// Therefore, these are simply deleted here.

	if ( ImGui::GetCurrentContext() == NULL )
		return false;

	ImGuiIO& io = ImGui::GetIO();
	switch ( msg )
	{
		case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
		case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
		{
			int button = 0;
			if ( msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK )
				button = 0;
			else if ( msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK )
				button = 1;
			else if ( msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK )
				button = 2;
			else if ( msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK )
				button = (GET_XBUTTON_WPARAM( wParam ) == XBUTTON1) ? 3 : 4;

			io.MouseDown[button] = true;
			return mouse.is_suppressed();
		}
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_XBUTTONUP:
		{
			int button = 0;
			if ( msg == WM_LBUTTONUP ) button = 0;
			if ( msg == WM_RBUTTONUP ) button = 1;
			if ( msg == WM_MBUTTONUP ) button = 2;
			if ( msg == WM_XBUTTONUP )
				button = (GET_XBUTTON_WPARAM( wParam ) == XBUTTON1) ? 3 : 4;

			io.MouseDown[button] = false;
			return false;
		}
		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
		{
			ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam );
			return mouse.is_suppressed();
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_CHAR:
		{
			ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam );
			return io.WantTextInput;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam );
			return false;
		}
	}
	return false;
}
