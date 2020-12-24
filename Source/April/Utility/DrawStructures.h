#pragma once

struct ImFont;

#include <string>


namespace April {

	struct XY {
		float x, y;
	};

	struct WH {
		float w, h;
	};

	struct RGBA {
		float r, g, b, a;
	};


	constexpr auto Invisible()			{ return RGBA{ 0, 0, 0, 0 }; }
	constexpr auto White( float a = 1 ) { return RGBA{ 1, 1, 1, a }; }
	constexpr auto Black( float a = 1 ) { return RGBA{ 0, 0, 0, a }; }

	constexpr auto Red( float a = 1 )		{ return RGBA{ 1, 0, 0, a }; }
	constexpr auto Green( float a = 1 )		{ return RGBA{ 0, 1, 0, a }; }
	constexpr auto Blue( float a = 1 )		{ return RGBA{ 0, 0, 1, a }; }
	constexpr auto Yellow( float a = 1 )	{ return RGBA{ 1, 1, 0, a }; }
	constexpr auto Pink( float a = 1 )		{ return RGBA{ 1, 0.4f, 0.7f, a }; }


	constexpr auto operator+( XY const& lhs, XY const& rhs )
	{
		auto const x = lhs.x + rhs.x;
		auto const y = lhs.y + rhs.y;

		return XY{ x, y };
	}
	

	struct FontInfo {
		std::string path;
		int			size;
	};

	auto LoadFont( const char* path, int size ) -> ImFont*;
	auto LoadFont( FontInfo const& ) -> ImFont*;


	struct Window {
		std::string		name;
		bool mutable	visible;
		int				flags; // see ImGuiWindowFlags; cannot circular include
	};


}
