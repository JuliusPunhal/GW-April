
#include "April/Framework/Features.h"

#include "April/Utility/Fonts.h"

#include <type_traits>


namespace {

	template<typename T, typename = void>
	struct has_Display : std::false_type {};

	template<typename T>
	struct has_Display<T, std::void_t<decltype( std::declval<T>().Display() )>>
		: std::true_type
	{
	};


	template<typename T>
	void display( T& ft )
	{
		if constexpr ( has_Display<typename T::element_type>::value )
		{
			ft->Display();
		}
	}

}


auto April::make_Features() -> Features
{
	ImGui::GetIO().IniFilename = "GW-April.ini";

	if ( auto const abz16 = LoadFont( "ABeeZee-Regular.ttf", 16 ) )
		ImGui::GetIO().Fonts->AddFontDefault( abz16->ConfigData );

	auto const abz40 = LoadFont( "ABeeZee-Regular.ttf", 40 );

	auto const font_atlas = std::make_shared<FontAtlas>();
	auto const mouse = std::make_shared<Mouse>();

	return Features{
		std::make_unique<Gui::InstanceTimer>( abz40, mouse ),
		font_atlas,
		mouse
	};
}

void April::Display( Features& features )
{
	std::apply( []( auto&... ft ) { (..., display( ft )); }, features );
}
