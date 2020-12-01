
#include "April/Utility/DrawStructures.h"

#include "Dependencies/ImGui.hpp"


auto April::LoadFont( const char* path, int const size ) -> ImFont*
{
	auto const font =
		ImGui::GetIO().Fonts->AddFontFromFileTTF(
			path, static_cast<float>( size ) );

	return font;
}
