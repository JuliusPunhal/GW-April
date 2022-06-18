
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

	template<typename T, typename Info_t, typename = void>
	struct has_ChatUpdate : std::false_type {};

	template<typename T, typename Info_t>
	struct has_ChatUpdate<
		T,
		Info_t,
		std::void_t<decltype(
			std::declval<T>().Update(
				std::declval<GW::HookStatus&>(),
				std::declval<Info_t>() ) )>>
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

	template<typename T, typename Info_t>
	void chat_update( T& ft, GW::HookStatus& status, Info_t info )
	{
		using Feature_t = typename T::element_type;
		if constexpr ( has_ChatUpdate<Feature_t, Info_t>::value )
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

	auto const cfg_gui_chainedsouls =
		from_json<Gui::ChainedSouls::Config>( json );
	auto const cfg_gui_dhuuminfo = from_json<Gui::DhuumInfo::Config>( json );
	auto const cfg_gui_energybar = from_json<Gui::Energybar::Config>( json );
	auto const cfg_gui_healthbar = from_json<Gui::Healthbar::Config>( json );
	auto const cfg_gui_instancetimer =
		from_json<Gui::InstanceTimer::Config>( json );
	auto const cfg_gui_inventory = from_json<Gui::Inventory::Config>( json );
	auto const cfg_gui_settings = from_json<Gui::Settings::Config>( json );
	auto const cfg_gui_skillbar = from_json<Gui::Skillbar::Config>( json );
	auto const cfg_gui_uwtimer = from_json<Gui::UwTimer::Config>( json );
	auto const cfg_default_theme = from_json<DefaultTheme::Config>( json );

	auto const cfg_mod_chatcommands =
		from_json<Module::ChatCommands::Config>( json );
	auto const cfg_mod_chatfilter =
		from_json<Module::ChatFilter::Config>( json );
	auto const cfg_mod_itemfilter =
		from_json<Module::ItemFilter::Config>( json );
	auto const cfg_mod_guildeditor =
		from_json<Module::GuildEditor::Config>( json );
	auto const cfg_mod_notifyeffect =
		from_json<Module::NotifyOnEffect::Config>( json );
	auto const cfg_mod_resignmsg =
		from_json<Module::ResignMessage::Config>( json );
	auto const cfg_mod_returnoutpost =
		from_json<Module::ReturnToOutpost::Config>( json );
	auto const cfg_mod_showkituses =
		from_json<Module::ShowKitUses::Config>( json );
	auto const cfg_mod_speechbubbles =
		from_json<Module::SuppressSpeechBubbles::Config>( json );

	auto const cfg_wgt_dialogs =
		from_json<Gui::Settings_Dialogs::Config>( json );

	auto const cfg_exit = from_json<Exit::Config>( json );

	auto const chained_souls = std::make_shared<ChainedSouls>();
	auto const consumables_mgr = std::make_shared<Module::ConsumablesMgr>();
	auto const dhuum_bot = std::make_shared<Module::DhuumBot>();
	auto const dhuums_judgement = std::make_shared<DhuumsJudgement>();
	auto const font_atlas = std::make_shared<FontAtlas>();
	auto const guild_editor =
		std::make_shared<Module::GuildEditor>( cfg_mod_guildeditor );
	auto const item_filter =
		std::make_shared<Module::ItemFilter>( cfg_mod_itemfilter );
	auto const mouse = std::make_shared<Mouse>();
	auto const reduced_recharge = std::make_shared<ReducedSkillRecharge>();
	auto const show_kit_uses =
		std::make_shared<Module::ShowKitUses>( cfg_mod_showkituses );
	auto const uwtimes = std::make_shared<UwTimesHistory>();

	auto const default_theme =
		std::make_shared<DefaultTheme>( cfg_default_theme, font_atlas );

	auto const exit = std::make_shared<Exit>( cfg_exit );

	return Features{
		std::make_unique<Gui::ChainedSouls>(
			cfg_gui_chainedsouls,
			mouse,
			chained_souls ),
		std::make_unique<Gui::DhuumInfo>(
			cfg_gui_dhuuminfo,
			dhuums_judgement,
			mouse ),
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
		std::make_unique<Gui::Settings>(
			cfg_gui_settings,
			font_atlas,
			mouse,
			std::tuple{
				cfg_mod_chatcommands,
				cfg_mod_chatfilter,
				cfg_mod_guildeditor,
				cfg_mod_itemfilter,
				cfg_mod_notifyeffect,
				cfg_mod_resignmsg,
				cfg_mod_returnoutpost,
				cfg_mod_showkituses,
				cfg_mod_speechbubbles,
				cfg_gui_chainedsouls,
				cfg_gui_dhuuminfo,
				cfg_gui_energybar,
				cfg_gui_healthbar,
				cfg_gui_instancetimer,
				cfg_gui_inventory,
				cfg_gui_settings,
				cfg_gui_skillbar,
				cfg_gui_uwtimer,
				cfg_default_theme,
				cfg_exit,
				cfg_wgt_dialogs
			},
			std::tuple{
				font_atlas,
				dhuum_bot,
				guild_editor,
				item_filter,
				show_kit_uses
			} ),
		std::make_unique<Gui::Skillbar>(
			cfg_gui_skillbar,
			font_atlas,
			reduced_recharge ),
		std::make_unique<Gui::UwTimer>(
			cfg_gui_uwtimer,
			mouse,
			uwtimes ),
		std::make_unique<Module::ChatCommands>(
			cfg_mod_chatcommands,
			consumables_mgr,
			item_filter,
			exit,
			std::forward_as_tuple(
				cfg_gui_chainedsouls,
				cfg_gui_dhuuminfo,
				cfg_gui_energybar,
				cfg_gui_healthbar,
				cfg_gui_instancetimer,
				cfg_gui_inventory,
				cfg_gui_settings,
				cfg_gui_skillbar,
				cfg_gui_uwtimer ) ),
		std::make_unique<Module::ChatFilter>(
			cfg_mod_chatfilter ),
		consumables_mgr,
		dhuum_bot,
		guild_editor,
		item_filter,
		std::make_unique<Module::NotifyOnEffect>(
			cfg_mod_notifyeffect ),
		std::make_unique<Module::ResignMessage>(
			cfg_mod_resignmsg ),
		std::make_unique<Module::ReturnToOutpost>(
			cfg_mod_returnoutpost ),
		show_kit_uses,
		std::make_unique<Module::SuppressSpeechBubbles>(
			cfg_mod_speechbubbles ),
		std::make_unique<Module::UwTimer>( uwtimes ),
		chained_souls,
		default_theme,
		dhuums_judgement,
		font_atlas,
		mouse,
		reduced_recharge,
		exit
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
	Features& features, GW::HookStatus& status, GW::LocalMessageInfo info )
{
	std::apply(
		[&status, info]( auto&... ft )
		{
			(..., chat_update( ft, status, info ));
		},
		features );
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
