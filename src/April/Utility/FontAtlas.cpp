
#include "April/Utility/FontAtlas.h"

#include "April/Utility/stl.h"

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

	auto get_file_name( std::string_view path )
	{
		constexpr auto not_found = decltype( path )::npos;

		auto const slash = path.find_last_of( "/\\" );
		if ( slash == not_found )
			return path;
		else
			return path.substr( slash + 1 );
	}

	auto get_imgui_name( ImFont* font )
	{
		auto const name = std::string_view{ font->GetDebugName() };
		return name.substr( 0, name.find( ',' ) );
	}

	auto is_imgui_loaded( April::FontInfo const& info ) -> ImFont*
	{
		auto const file_name = get_file_name( info.name );

		for ( auto* font : ImGui::GetIO().Fonts->Fonts )
		{
			if (
				get_imgui_name( font ) == file_name
				&& static_cast<int>( font->FontSize ) == info.size )
			{
				return font;
			}
		}

		return nullptr;
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

	auto try_load_font( April::FontInfo const& info ) -> ImFont*
	{
		if ( not is_font_file( info.name ) || info.size <= 0 )
			return nullptr;

		auto const path = get_font_path( info.name );
		if ( path.empty() )
			return nullptr;

		return
			ImGui::GetIO().Fonts->AddFontFromFileTTF(
				path.c_str(), static_cast<float>( info.size ) );
	}

}


bool April::operator==( FontInfo const& rhs, FontInfo const& lhs )
{
	return rhs.name == lhs.name && rhs.size == lhs.size;
}

auto April::FontAtlas::Get( std::optional<FontInfo> const& info ) const
	-> ImFont*
{
	if ( info == std::nullopt )
		return nullptr;

	if ( not is_font_file( info->name ) )
		return nullptr;

	if ( stl::find( requested_fonts, *info ) != requested_fonts.end() )
		return nullptr;

	auto* font = is_imgui_loaded( *info );
	if ( font == nullptr )
	{
		requested_fonts.push_back( *info );
		return nullptr;
	}

	return font;
}

bool April::FontAtlas::LoadRequestedFonts()
{
	auto& io = ImGui::GetIO();

	auto original_size = io.Fonts->Fonts.size();
	auto all_fonts_loaded = true;

	for ( auto const& info : requested_fonts )
	{
		if ( stl::find( unable_to_load, info ) != unable_to_load.end() )
			continue;

		if ( try_load_font( info ) == nullptr )
		{
			unable_to_load.push_back( info );
			all_fonts_loaded = false;
		}
		else loaded_fonts.push_back( info );
	}
	requested_fonts.clear();

	if ( io.Fonts->Fonts.size() > original_size )
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		io.Fonts->Build();
	}

	return all_fonts_loaded;
}

bool April::FontAtlas::CanLoad( FontInfo const& info )
{
	return
		is_font_file( info.name )
		&& info.size > 0
		&& not get_font_path( info.name ).empty();
}
