#pragma once

#pragma warning( push, 0 )
#include "imgui.h"
#include "backends/imgui_impl_dx9.h"
#include "backends/imgui_impl_win32.h"

#include "imgui_internal.h"
#pragma warning( pop )

#include "April/Utility/DrawStructures.h"

#include <string>


namespace ImGui {

	bool Begin( April::Window const& );
	void Text( std::string const& );
	bool Button( std::string const&, ImVec2 const& size = { 0, 0 } );
	bool SmallButton( std::string const& );

}
