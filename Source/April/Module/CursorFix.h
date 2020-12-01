#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


namespace April {

	// Based off of https://github.com/HasKha/GWToolboxpp.git


	class CursorFix {
	public:
		CursorFix();


	private:
		using GetClipCursor_pt = BOOL (WINAPI *)( _Out_ LPRECT lpRect );
		GetClipCursor_pt GetClipCursor_Func = nullptr;
		GetClipCursor_pt RetGetClipCursor = nullptr;
	};

}
