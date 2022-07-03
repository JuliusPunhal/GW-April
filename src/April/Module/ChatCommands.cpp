
#include "April/Module/ChatCommands.h"

#include "April/Gui/Settings.h"

#include "April/Utility/stl.h"

#include "ImGui/ImGui.hpp"

#include <charconv>
#include <optional>
#include <string>
#include <string_view>

using Abbreviation = April::Module::ChatCommands::Abbreviation;
using Config = April::Module::ChatCommands::Config;
using GuiConfigs = April::Module::ChatCommands::GuiConfigs;

using namespace std::string_view_literals;


namespace {

	template<typename Iter_t>
	auto find_space( Iter_t const begin, Iter_t const end )
	{
		return std::find( begin, end, ' ' );
	}

	template<typename Iter_t>
	auto find_word( Iter_t const begin, Iter_t const end )
	{
		constexpr auto is_space = []( char const ch ) { return ch == ' '; };
		return std::find_if_not( begin, end, is_space );
	}

	auto expand_abbreviation(
		std::string_view const abbreviated, Config const& config )
		-> std::optional<std::string>
	{
		auto const iter =
			std::find_if(
				config.abbreviations.begin(), config.abbreviations.end(),
				[&]( Abbreviation const& alias )
				{
					return alias.abbreviated() == abbreviated;
				} );

		if ( iter == std::end( config.abbreviations ) )
			return std::nullopt;

		return iter->expanded();
	}

	auto expand_abbreviations(
		std::string_view const msg, Config const& config )
	{
		auto result = std::string{ msg };

		for (
			auto word_begin = find_word( result.begin(), result.end() );
			word_begin != result.end();
			/**/ )
		{
			auto const word_end = find_space( word_begin, result.end() );

			auto const abbrev = stl::make_sv( word_begin, word_end );
			if ( auto const original = expand_abbreviation( abbrev, config ) )
			{
				// expanding might re-alloc -> save the position
				auto const pos = std::distance( result.begin(), word_begin );

				result.replace( word_begin, word_end, *original );

				// recheck if expanded itself is an abbreviation
				word_begin = result.begin() + pos;
				continue;
			}

			word_begin = find_word( word_end, result.end() );
		}

		return result;
	}

	auto to_int( std::string_view const str ) -> std::optional<int>
	{
		auto result = -1;
		std::from_chars( str.data(), str.data() + str.size(), result );

		// Treat negative numbers as errors, even if "-42" was passed in, since
		// we're dealing with ModelIDs, ObjectiveIDs, etc. in the scope of this
		// module.
		if ( result < 0 )
			return std::nullopt;

		return result;
	}

	auto to_ints( std::string_view const str ) -> std::vector<int>
	{
		auto ids = std::vector<int>{};

		for (
			auto word_begin = find_word( str.begin(), str.end() );
			word_begin != str.end();
			/**/ )
		{
			auto const word_end = find_space( word_begin, str.end() );
			auto const word = stl::make_sv( word_begin, word_end );

			if ( auto const id = to_int( word ) )
				ids.emplace_back( *id );

			word_begin = find_word( word_end, str.end() );
		}

		return ids;
	}

	void on_openxunlai()
	{
		if ( GW::GetInstanceType() != GW::InstanceType::Outpost )
		{
			GW::WriteChat(
				GW::PARTY, "Xunlai Chest can only be opened in Outposts!" );
			return;
		}

		if ( GW::GetCurrentMapInfo().region == GW::Region::Region_Presearing )
		{
			GW::WriteChat(
				GW::PARTY, "Xunlai Chest cannot be opened in Presearing!" );
			return;
		}

		GW::OpenXunlaiWindow();
	}

	auto trim_spaces( std::string_view result )
	{
		while ( result.size() > 0 && result.front() == ' ' )
			result.remove_prefix( 1 );

		while ( result.size() > 0 && result.back() == ' ' )
			result.remove_suffix( 1 );

		return result;
	}

	bool toggle_gui(
		GuiConfigs& configs,
		std::string_view const name,
		std::optional<bool> const visibility = std::nullopt )
	{
		bool window_found = false;

		auto set_visibility = [&]( ImGui::Window& window )
		{
			if ( window.name == name )
			{
				window.visible = visibility.value_or( not window.visible );
				window_found = true;
			}
		};
		auto toggle = [&set_visibility]( auto&... p_config )
		{
			(..., set_visibility( p_config->window ));
		};

		std::apply( toggle, configs );

		if ( not window_found )
		{
			GW::WriteChat(
				GW::PARTY, "Window " + std::string{ name } + " not found!" );
		}

		return window_found;
	}

}


April::Module::ChatCommands::ChatCommands(
	std::shared_ptr<Config const>   config,
	std::shared_ptr<ConsumablesMgr> consumables_mgr,
	std::shared_ptr<ItemFilter>     item_filter,
	std::shared_ptr<Exit>           exit,
	GuiConfigs                      gui_configs )
	:
	config{ config },
	consumables_mgr{ consumables_mgr },
	item_filter{ item_filter },
	exit{ exit },
	gui_configs{ gui_configs }
{
}

void April::Module::ChatCommands::Update(
	GW::HookStatus& status, GW::SendChatInfo info )
{
	if ( info.msg[0] != L'/' )
		return;

	auto const msg = stl::wstr_to_utf8( std::wstring{ info.msg } );
	auto const expanded = expand_abbreviations( msg, *config );

	for ( auto cmd_begin = expanded.begin(); cmd_begin < expanded.end(); /**/ )
	{
		auto const cmd_end = std::find( cmd_begin + 1, expanded.end(), '/' );

		auto const space = find_space( cmd_begin, cmd_end );
		auto const cmd = stl::make_sv( cmd_begin, space );
		auto const args = trim_spaces( stl::make_sv( space, cmd_end ) );

		if ( cmd == cmd_exit )
		{
			exit->initiate_exit();
			status.blocked = true;
		}
		else if ( cmd == cmd_reset_dx9 )
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			status.blocked = true;
		}
		else if ( cmd == cmd_openxunlai )
		{
			on_openxunlai();
			status.blocked = true;
		}
		else if ( cmd == cmd_pcons_perm_on )
		{
			auto const ids = to_ints( args );
			for ( auto const id : ids )
				consumables_mgr->activate_perm( id );

			if ( ids.empty() )
				consumables_mgr->deactivate_all_perm();

			status.blocked = true;
		}
		else if ( cmd == cmd_pcons_perm_off )
		{
			consumables_mgr->deactivate_all_perm();
			status.blocked = true;
		}
		else if ( cmd == cmd_pcons_temp_on )
		{
			auto const ids = to_ints( args );
			for ( auto const id : ids )
				consumables_mgr->activate_temp( id );

			if ( ids.empty() )
				consumables_mgr->deactivate_all_temp();

			status.blocked = true;
		}
		else if ( cmd == cmd_pcons_temp_off )
		{
			consumables_mgr->deactivate_all_temp();
			status.blocked = true;
		}
		else if ( cmd == cmd_pcons_objective )
		{
			auto const objective = to_int( args );
			consumables_mgr->objective = objective;
			status.blocked = true;

			if ( objective )
			{
				GW::WriteChat(
					GW::EMOTE,
					"Set Consumables-Objective to "
					+ std::to_string( *objective ) );
			}
			else GW::WriteChat( GW::EMOTE, "Reset Consumables-Objective" );
		}
		else if ( cmd == cmd_settings_panel )
		{
			status.blocked = true;

			using Gui_t = std::shared_ptr<Gui::Settings_Config>;
			auto const gui = std::get<Gui_t>( gui_configs );
			auto const found =
				Gui::SetSettingsPanel( std::string{ args }, *gui );

			if ( not found )
				GW::WriteChat(
					GW::EMOTE,
					"Settings Panel " + std::string{ args } + " not found!" );
		}
		else if ( cmd == cmd_suppressed_items )
		{
			auto chat_msg = [&]() -> std::string
			{
				if ( item_filter->size() == 1 )
					return "Releaving 1 item.";

				auto str = std::to_string( item_filter->size() );
				return "Revealing " + str + " items.";
			};

			GW::WriteChat( GW::EMOTE, chat_msg() );
			item_filter->SpawnSuppressedItems();
			status.blocked = true;
		}
		else if ( cmd == cmd_toggle_window )
		{
			if ( args.substr( args.size() - 5, 5 ) == " show" )
			{
				auto const name = args.substr( 0, args.size() - 5 );
				if ( toggle_gui( gui_configs, name, true ) )
					status.blocked = true;
			}
			else if ( args.substr( args.size() - 5, 5 ) == " hide" )
			{
				auto const name = args.substr( 0, args.size() - 5 );
				if ( toggle_gui( gui_configs, name, false ) )
					status.blocked = true;
			}
			else if ( toggle_gui( gui_configs, args ) )
			{
				status.blocked = true;
			}
		}

		cmd_begin = cmd_end;
	}
}

April::Module::ChatCommands::Abbreviation::Abbreviation(
	std::string const& abbr, std::string const& exp )
	: abbreviated_{ abbr }, expanded_{ exp }
{
}

auto April::Module::make_Abbreviation(
	std::string const& abbr, std::string const& exp )
	-> std::optional<Abbreviation>
{
	if (
		find_space( abbr.begin(), abbr.end() ) == abbr.end()
		&& abbr.size() > 0 )
	{
		return Abbreviation{ abbr, exp };
	}
	else return std::nullopt;
}

bool April::Module::ChatCommands::Abbreviation::operator==(
	std::string const& str ) const
{
	return abbreviated_ == str;
}

auto April::Module::ChatCommands::Config::default_Abbreviations()
	-> Abbreviations
{
	using namespace GW::Constants::ItemModelID;
	using namespace GW::Constants::ObjectiveID;

	auto make = []( const char* abbr, auto&& orig )
	{
		using T = stl::remove_cvref_t<decltype( orig )>;
		if constexpr ( std::is_integral_v<T> )
		{
			return *make_Abbreviation( abbr, std::to_string( orig ) );
		}
		else return *make_Abbreviation( abbr, orig );
	};

	auto const lunars_concat =
		std::to_string( LunarPig )
		+ " " + std::to_string( LunarRat )
		+ " " + std::to_string( LunarOx )
		+ " " + std::to_string( LunarTiger )
		+ " " + std::to_string( LunarRabbit )
		+ " " + std::to_string( LunarDragon )
		+ " " + std::to_string( LunarSnake )
		+ " " + std::to_string( LunarHorse )
		+ " " + std::to_string( LunarSheep )
		+ " " + std::to_string( LunarMonkey )
		+ " " + std::to_string( LunarRooster )
		+ " " + std::to_string( LunarDog );

	return std::vector<Abbreviation>{
		// Commands
		make( "/x",               cmd_openxunlai ),
		make( "/sp",              cmd_pcons_perm_on ),
		make( "/sp_off",          cmd_pcons_perm_off ),
		make( "/s",               cmd_pcons_temp_on ),
		make( "/s_off",           cmd_pcons_temp_off ),
		make( "/o",               cmd_pcons_objective ),
		make( "/texmod",          cmd_reset_dx9 ),
		make( "/settings_panel",  cmd_settings_panel ),
		make( "/show_suppressed", cmd_suppressed_items ),
		make( "/wnd",             cmd_toggle_window ),

		// Consumables
		make( "cupcake",   Cupcakes ),
		make( "apple",     Apples ),
		make( "corn",      Corns ),
		make( "pie",       Pies ),
		make( "egg",       Eggs ),
		make( "warsupply", Warsupplies ),
		make( "grog",      Grog ),
		make( "lunars",    lunars_concat ),
		make( "lunar",     "lunars" ),
		make( "soup",      SkalefinSoup ),
		make( "kabob",     Kabobs ),
		make( "salad",     PahnaiSalad ),
		make( "bu",        ConsEssence ),
		make( "grail",     ConsGrail ),
		make( "armor",     ConsArmor ),
		make( "cons",      "bu grail armor" ),
		make( "rrc",       RRC ),
		make( "brc",       BRC ),
		make( "grc",       GRC ),
		make( "rainbow",   "rrc brc grc" ),
		make( "miku",      ELMiku ),
		make( "ident",     IdentificationKit_Superior ),

		// Objectives
		make( "restore", Restore ),
		make( "escort",  Escort ),
		make( "uwg",     UWG ),
		make( "vale",    Vale ),
		make( "wastes",  Wastes ),
		make( "pits",    Pits ),
		make( "planes",  Planes ),
		make( "mnts",    Mnts ),
		make( "pools",   Pools ),
		make( "dhuum",   Dhuum ),
	};
}
