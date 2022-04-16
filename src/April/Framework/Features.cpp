
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
	struct has_ChatUpdate : std::false_type {};

	template<typename T>
	struct has_ChatUpdate<
		T,
		std::void_t<decltype(
			std::declval<T>().Update(
				std::declval<GW::HookStatus&>(),
				std::declval<GW::SendChatInfo>() ) )>>
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
	void chat_update( T& ft, GW::HookStatus& status, GW::SendChatInfo info )
	{
		if constexpr ( has_ChatUpdate<typename T::element_type>::value )
		{
			ft->Update( status, info );
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

	auto const json = load_json_from_file();

	auto const consumables_mgr = std::make_shared<Module::ConsumablesMgr>();
	auto const font_atlas = std::make_shared<FontAtlas>();
	auto const item_filter =
		std::make_shared<Module::ItemFilter>(
			from_json<Module::ItemFilter::Config>( json ) );
	auto const mouse = std::make_shared<Mouse>();
	auto const reduced_recharge = std::make_shared<ReducedSkillRecharge>();
	auto const uwtimes = std::make_shared<UwTimesHistory>();

	font_atlas->Get( FontInfo{ "ABeeZee-Regular.ttf", 16 } );
	font_atlas->LoadRequestedFonts(); // loads default font

	auto const cfg_gui_energybar = from_json<Gui::Energybar::Config>( json );
	auto const cfg_gui_healthbar = from_json<Gui::Healthbar::Config>( json );
	auto const cfg_gui_instancetimer =
		from_json<Gui::InstanceTimer::Config>( json );
	auto const cfg_gui_inventory = from_json<Gui::Inventory::Config>( json );
	auto const cfg_gui_skillbar = from_json<Gui::Skillbar::Config>( json );
	auto const cfg_gui_uwtimer = from_json<Gui::UwTimer::Config>( json );

	return Features{
		std::make_unique<Gui::Energybar>(
			cfg_gui_energybar,
			font_atlas ),
		std::make_unique<Gui::Healthbar>(
			cfg_gui_healthbar,
			font_atlas ),
		std::make_unique<Gui::InstanceTimer>(
			cfg_gui_instancetimer,
			font_atlas,
			mouse ),
		std::make_unique<Gui::Inventory>(
			cfg_gui_inventory,
			font_atlas,
			mouse,
			consumables_mgr ),
		std::make_unique<Gui::Skillbar>(
			cfg_gui_skillbar,
			font_atlas,
			reduced_recharge ),
		std::make_unique<Gui::UwTimer>(
			cfg_gui_uwtimer,
			mouse,
			uwtimes ),
		std::make_unique<Module::ChatCommands>(
			from_json<Module::ChatCommands::Config>( json ),
			consumables_mgr,
			item_filter,
			std::forward_as_tuple(
				cfg_gui_energybar,
				cfg_gui_healthbar,
				cfg_gui_instancetimer,
				cfg_gui_inventory,
				cfg_gui_skillbar,
				cfg_gui_uwtimer ) ),
		consumables_mgr,
		std::make_unique<Module::GuildEditor>(
			from_json<Module::GuildEditor::Config>( json ) ),
		item_filter,
		std::make_unique<Module::UwTimer>( uwtimes ),
		font_atlas,
		mouse,
		reduced_recharge
	};
}

void April::Shutdown( Features& features )
{
	using ItemFilter = std::shared_ptr<Module::ItemFilter>;
	std::get<ItemFilter>( features )->SpawnSuppressedItems();
}

void April::Update( Features& features )
{
	std::apply( []( auto&... ft ) { (..., update( ft )); }, features );
}

void April::Update(
	Features& features, GW::HookStatus& status, GW::SendChatInfo info )
{
	std::apply(
		[&status, info]( auto&... ft )
		{
			(..., chat_update( ft, status, info ));
		},
		features );
}

void April::Display( Features& features )
{
	std::apply( []( auto&... ft ) { (..., display( ft )); }, features );
}
