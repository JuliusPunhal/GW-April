
#include "April/Framework/Features.h"
#include "April/Utility/FileIO.h"

#include <type_traits>


namespace {

	template<typename T, typename = void>
	struct has_Update : std::false_type {};

	template<typename T>
	struct has_Update<T, std::void_t<decltype( std::declval<T>().Update() )>>
		: std::true_type
	{
	};

	template<typename T, typename = void>
	struct has_Display : std::false_type {};

	template<typename T>
	struct has_Display<T, std::void_t<decltype( std::declval<T>().Display() )>>
		: std::true_type
	{
	};


	template<typename T>
	void update( T& ft )
	{
		if constexpr ( has_Update<typename T::element_type>::value )
		{
			ft->Update();
		}
	}

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

	auto const font_atlas = std::make_shared<FontAtlas>();
	auto const mouse = std::make_shared<Mouse>();
	auto const reduced_recharge = std::make_shared<ReducedSkillRecharge>();
	auto const uwtimes = std::make_shared<UwTimesHistory>();

	font_atlas->Get( FontInfo{ "ABeeZee-Regular.ttf", 16 } );
	font_atlas->LoadRequestedFonts(); // loads default font

	auto const json = load_json_from_file();

	return Features{
		std::make_unique<Gui::InstanceTimer>(
			from_json<Gui::InstanceTimer::Config>( json ),
			font_atlas,
			mouse ),
		std::make_unique<Gui::Inventory>(
			from_json<Gui::Inventory::Config>( json ) ),
		std::make_unique<Gui::Skillbar>(
			from_json<Gui::Skillbar::Config>( json ),
			font_atlas,
			reduced_recharge ),
		std::make_unique<Gui::UwTimer>(
			from_json<Gui::UwTimer::Config>( json ),
			mouse,
			uwtimes ),
		std::make_unique<Module::ConsumablesMgr>(),
		std::make_unique<Module::UwTimer>( uwtimes ),
		font_atlas,
		mouse,
		reduced_recharge
	};
}

void April::Update( Features& features )
{
	std::apply( []( auto&... ft ) { (..., update( ft )); }, features );
}

void April::Display( Features& features )
{
	std::apply( []( auto&... ft ) { (..., display( ft )); }, features );
}
