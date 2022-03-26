
#include "April/Utility/Fonts.h"

#include <filesystem>
#include <stdlib.h>
#include <string_view>

namespace fs = std::filesystem;
using namespace std::string_literals;


namespace {

	bool is_font_file( std::string_view const name )
	{
		return name.size() > 4 && name.substr( name.size() - 4 ) == ".ttf";
	}

	auto appdata_path() -> std::string
	{
		auto appdata = std::string{};
		char* buf = nullptr;

		auto const error = _dupenv_s( &buf, nullptr, "APPDATA" );
		if ( not error && buf != nullptr )
		{
			appdata = buf;
		}

		free( buf );
		return appdata + "\\GW-April\\Fonts\\";
	}

	auto get_font_path( std::string_view const name ) -> std::string
	{
		if ( fs::exists( name ) ) // full path or current dir
			return std::string{ name };


		if ( // appdata
			auto const path = appdata_path().append( name );
			fs::exists( path ) )
		{
			return path;
		}

		if ( // windows fonts
			auto const path = "C:\\Windows\\Fonts\\"s.append( name );
			fs::exists( path ) )
		{
			return path;
		}

		return "";
	}

}


auto April::LoadFont( std::string_view const name, int const size ) -> ImFont*
{
	if ( not is_font_file( name ) || size <= 0 )
		return nullptr;

	auto const path = get_font_path( name );
	if ( path.empty() )
		return nullptr;

	return
		ImGui::GetIO().Fonts->AddFontFromFileTTF(
			path.c_str(), static_cast<float>( size ) );
}
