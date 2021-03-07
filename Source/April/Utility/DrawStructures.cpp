
#include "April/Utility/DrawStructures.h"

#include "April/Utility/stl.h"

#include "Dependencies/ImGui.hpp"

#include <string>


auto April::LoadFont( const char* path, int const size ) -> ImFont*
{
	auto const font =
		ImGui::GetIO().Fonts->AddFontFromFileTTF(
			path, static_cast<float>( size ) );

	return font;
}

auto April::LoadFont( FontInfo const& info ) -> ImFont*
{
	return LoadFont( info.path.c_str(), info.size );
}
