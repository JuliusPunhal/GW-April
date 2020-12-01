
#include "April/Framework/WndProc.h"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"


namespace {
	
	auto hwnd = HWND{};
	auto original = WNDPROC{};
	

	bool ImGui_Proc( HWND, UINT msg, WPARAM wParam, LPARAM )
	{
		/*	This is basically a copy of ImGui_ImplWin32_WndProcHandler() but
			::SetCapture() and ::ReleaseCapture() seem to mess with window-
			messages thus disabling right-click + drag, i.e. camera-movement.
			Therefore, these are simply deleted here.
		*/

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
				return false;
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
			{
				auto const scroll_distance = GET_WHEEL_DELTA_WPARAM( wParam );
				io.MouseWheel += (float)scroll_distance / (float)WHEEL_DELTA;
				return false;
			}
			case WM_MOUSEHWHEEL:
			{
				auto const scroll_distance = GET_WHEEL_DELTA_WPARAM( wParam );
				io.MouseWheelH += (float)scroll_distance / (float)WHEEL_DELTA;
				return false;
			}
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				if ( wParam < 256 )
					io.KeysDown[wParam] = 1;
				return false;
			case WM_KEYUP:
			case WM_SYSKEYUP:
				if ( wParam < 256 )
					io.KeysDown[wParam] = 0;
				return false;
			case WM_CHAR:
				io.AddInputCharacter( (unsigned int)wParam );
				return false;
		}
		return false;
	}
	
	auto WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) -> LRESULT
	{
		if ( ImGui_Proc( hWnd, msg, wParam, lParam ) )
			return 0;

		return CallWindowProc( original, hWnd, msg, wParam, lParam );
	}

}


void April::WndProc::Initialize( HWND const handle )
{
	if ( original == nullptr )
	{
		hwnd = handle;
		original = (WNDPROC)SetWindowLong( hwnd, GWL_WNDPROC, (LONG)::WndProc );
	}
}

void April::WndProc::Terminate()
{
	if ( original != nullptr )
	{
		SetWindowLong( hwnd, GWL_WNDPROC, (LONG)original );
		original = WNDPROC{};
		hwnd = HWND{};
	}
}
