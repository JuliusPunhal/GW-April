
#include "April/Gui/Settings.h"

#include "April/Utility/FileIO.h"
#include "April/Utility/stl.h"
#include "April/Utility/TimeFormatting.h"

#include "GWCA/GWCA.hpp"

#include "ImGui/ImGui.hpp"

using namespace April;
using namespace April::Gui;


namespace {

	constexpr auto desc_april =
		"Welcome to the GW-April project.\n"
		"April is a 3rd-party tool that tries to combine essential quality-of-"
		"life upgrades from GWToolbox++ with some additional features, which "
		"mainly aim to be used in The Underworld.\n\n"
		"When first starting April, every feature will be turned off. Use the "
		"panel on the left to opt into the features of your liking. Every "
		"feature provides a short documentation to explain what it does and "
		"how it is intended to be used.\n"
		"Features are divided into Modules and Windows; Modules work in the "
		"background and essentially manage some internal state, Windows are "
		"the visual part of the experience and sometimes interface modules.\n"
		"The final panel provides some informative widgets that do not "
		"require a separate window.\n\n"
		"Feel free to voice any and all problems or suggestions!";


	template<typename T>
	struct Name {
		using Configuration = T;
		const char* name = nullptr;
	};

	inline constexpr auto names = std::tuple{
		Name<Settings_Generic<Gui::ChainedSouls::Config>>{ "Chained Souls" },
		Name<Settings_DhuumBot>{ "Dhuum Bot" },
		Name<Settings_Generic<Gui::DhuumInfo::Config>>{ "Dhuum Info" },
		Name<Settings_Energybar>{ "Energybar" },
		Name<Settings_Healthbar>{ "Healthbar" },
		Name<Settings_InstanceTimer>{ "Instance Timer" },
		Name<Settings_Inventory>{ "Inventory" },
		Name<Settings_Settings>{ "Settings" },
		Name<Settings_Skillbar>{ "Skillbar" },
		Name<Settings_Generic<Gui::UwTimer::Config>>{ "Underworld Timer" },
		Name<Settings_ChatCommands>{ "Chat Commands" },
		Name<Settings_ChatFilter>{ "Chat Filter" },
		Name<Settings_GuildEditor>{ "Guild Editor" },
		Name<Settings_ItemFilter>{ "Item Filter" },
		Name<Settings_NotifyOnEffect>{ "Notify on Effect" },
		Name<Settings_Generic<Module::ResignMessage::Config>>{
			"Resign Message"
		},
		Name<Settings_Generic<Module::ResignReminder::Config>>{
			"Resign Reminder"
		},
		Name<Settings_Generic<Module::ReturnToOutpost::Config>>{
			"Return to Outpost"
		},
		Name<Settings_ShowKitUses>{ "Show Kit Uses" },
		Name<Settings_Generic<Module::SuppressSpeechBubbles::Config>>{
			"Speech Bubbles"
		},
		Name<Settings_DefaultTheme>{ "Default Theme" },
		Name<Settings_Dialogs>{ "Dialogs" },
		Name<Settings_ResignLog>{ "Resign Log" },
		Name<Settings_TargetInfo>{ "Target Info" }
	};

	template<typename T>
	constexpr auto get_name() -> const char*
	{
		return std::get<Name<stl::remove_cvref_t<T>>>( names ).name;
	}


	template<typename T, typename Tuple_t>
	auto get_sp( Tuple_t& tup )
	{
		return std::get<std::shared_ptr<T>>( tup );
	}

	float sidebar_width()
	{
		auto result = 0.f;

		auto impl = [&]( auto const& named )
		{
			auto const length = ImGui::CalcTextSize( named.name ).x;
			result = std::max( result, length );
		};

		std::apply(
			[&impl]( auto const&... named ) { (..., impl( named )); }, names );

		return
			result
			+ ImGui::GetStyle().WindowPadding.x
			+ ImGui::GetStyle().ItemInnerSpacing.x * 2
			+ ImGui::GetStyle().ItemSpacing.x;
	}

	template<typename... Ts>
	bool draw_sidebar(
		std::tuple<Ts...> const& widgets, std::string& selected )
	{
		auto const& color = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
		ImGui::PushStyleColor( ImGuiCol_HeaderHovered, color );

		auto selected_changed = false;
		auto draw_selectable = [&]( const char* label )
		{
			auto const is_selected = (selected == label);

			if ( is_selected )
			{
				auto const& color = ImGui::GetStyle().Colors[ImGuiCol_Header];
				ImGui::PushStyleColor( ImGuiCol_HeaderHovered, color );
			}

			if ( ImGui::Selectable( label, is_selected ) )
			{
				selected = selected == label ? "" : selected = label;
				selected_changed = true;
			}

			if ( is_selected )
				ImGui::PopStyleColor();
		};

		std::apply(
			[&draw_selectable]( auto const&... widget )
			{
				(..., draw_selectable( get_name<decltype( widget )>() ));
			},
			widgets );

		ImGui::PopStyleColor();

		return selected_changed;
	}

	bool draw_selected(
		Settings::Sections& sections,
		int&                selected_section,
		std::string&        selected_feature )
	{
		if ( selected_section == 0 )
			return draw_sidebar( sections.modules, selected_feature );

		if ( selected_section == 1 )
			return draw_sidebar( sections.guis, selected_feature );

		if ( selected_section == 2 )
			return draw_sidebar( sections.info, selected_feature );

		return false;
	}

	template<typename... Ts>
	void draw_save_discard( std::tuple<std::shared_ptr<Ts>...>& configs )
	{
		auto const& space = ImGui::GetStyle().ItemInnerSpacing;
		ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, space );

		auto const width = ImGui::WidthToFitNItems( 2 );
		if ( ImGui::Button( "Save", { width, 0 } ) )
		{
			auto json = nlohmann::json{};

			auto impl = [&json]( auto const& config )
			{
				using T = stl::remove_cvref_t<decltype( config )>;
				json[JsonKey<T::element_type>::key] = *config;
			};

			std::apply(
				[&]( auto const&... config ) { (..., impl( config ) ); },
				configs );

			save_json_to_file( json );
		}
		ImGui::SameLine();
		if ( ImGui::Button( "Discard", { width, 0 } ) )
		{
			auto const json = load_json_from_file();
			auto impl /*assign_json_to_config*/ = [&]( auto& config )
			{
				using T = stl::remove_cvref_t<decltype( config )>;
				config = *from_json<T>( json );
			};
			std::apply(
				[&]( auto&... config ) { (..., impl( *config )); },
				configs );
		}

		ImGui::PopStyleVar();
	}

	template<typename... Ts>
	void draw_selected(
		std::string const& selected,
		std::tuple<Ts...>& widgets,
		Gui::Settings::Config const& config,
		FontAtlas const& fonts )
	{
		auto impl = [&]( auto& selected_config )
		{
			using T = stl::remove_cvref_t<decltype( selected_config )>;
			if ( selected != get_name<T>() )
				return;

			ImGui::PushFont( fonts.Get( config.heading ) );
			ImGui::TextCentered( get_name<T>() );
			ImGui::PopFont();

			ImGui::SeparatorSpaced( 0.0f, 0.5f );

			selected_config.Draw();
		};

		std::apply(
			[&]( auto&... widget ) { ( ..., impl( widget ) ); }, widgets );
	}

	bool draw_sidebar_and_saveload(
		Settings::Sections&       sections,
		int&                      selected_section,
		std::string&              selected_feature,
		Settings::Configurations& configs )
	{
		ImGui::BeginChild( "##SidebarAndSaveLoad", { sidebar_width(), 0 } );

		auto const sidebar_height =
			ImGui::GetContentRegionAvail().y
			- ImGui::GetFrameHeight()
			- ImGui::GetStyle().ItemSpacing.y;

		ImGui::BeginChild( "##Sidebar", { 0, sidebar_height }, true );

		ImGui::PushItemWidth( -1 );
		ImGui::Combo(
			"##SelectSection",
			&selected_section,
			"Modules\0Windows\0Other\0" );

		ImGui::Separator();

		auto const selected_changed =
			draw_selected(
				sections, selected_section, selected_feature );

		ImGui::EndChild();

		draw_save_discard( configs );

		ImGui::EndChild();

		return selected_changed;
	}

}


April::Gui::Settings::Settings(
	std::shared_ptr<Config>          config,
	std::shared_ptr<FontAtlas const> fonts,
	std::shared_ptr<Mouse>           mouse,
	Configurations const&            configs,
	Features const&                  features )
	:
	config{ config },
	fonts{ fonts },
	mouse{ mouse },
	sections{
		std::tuple{
			Settings_ChatCommands{
				get_sp<Module::ChatCommands::Config>( configs )
			},
			Settings_ChatFilter{
				get_sp<Module::ChatFilter::Config>( configs )
			},
			Settings_GuildEditor{
				get_sp<Module::GuildEditor>( features ),
				get_sp<Module::GuildEditor::Config>( configs )
			},
			Settings_ItemFilter{
				get_sp<Module::ItemFilter>( features ),
				get_sp<Module::ItemFilter::Config>( configs )
			},
			Settings_NotifyOnEffect{
				get_sp<Module::NotifyOnEffect::Config>( configs )
			},
			Settings_Generic{
				"When activated, this module will change the in-game resign-"
				"message to show how many party-members have resigned:\n\n"
				"Your Character Name (1/8) has resigned.",
				get_sp<Module::ResignMessage::Config>( configs )
			},
			Settings_Generic{
				"When activated, this module will write a message into chat "
				"if all players except you have resigned.",
				get_sp<Module::ResignReminder::Config>( configs )
			},
			Settings_Generic{
				"When activated, this module will automatically return you to "
				"the outpost when your party has been defeated. Will also do "
				"so when the party-leader leaves, making you leader.",
				get_sp<Module::ReturnToOutpost::Config>( configs )
			},
			Settings_ShowKitUses{
				get_sp<Module::ShowKitUses>( features ),
				get_sp<Module::ShowKitUses::Config>( configs )
			},
			Settings_Generic{
				"When activated, this module suppresses Speech Bubbles over "
				"your character's head when drunk or under the effects of "
				"Lunar Blessing.",
				get_sp<Module::SuppressSpeechBubbles::Config>( configs )
			}
		},
		std::tuple{
			Settings_Generic{
				"The Chained Soul Info Window will show the duration until "
				"death or respawn of the Chained Souls in The Bone Pits.\n\n"
				"You can send the displayed information to Team Chat by "
				"clicking on it.",
				get_sp<Gui::ChainedSouls::Config>( configs )
			},
			Settings_Generic{
				"The Dhuum Info Window is inspired by the in-game \"Mission "
				"Progress\"-window, will however display the precise "
				"percentage of both your party's Dhuum's Rest progress and "
				"time since Dhuum last used Dhuum's Judgement.\n"
				"Usually, Dhuum will use Dhuum's Judgement about every "
				"45s.\n\n"
				"You can send the displayed information to Team Chat by "
				"clicking on it.",
				get_sp<Gui::DhuumInfo::Config>( configs )
			},
			Settings_Energybar{
				get_sp<Gui::Energybar::Config>( configs ),
				get_sp<FontAtlas>( features )
			},
			Settings_Healthbar{
				get_sp<Gui::Healthbar::Config>( configs ),
				get_sp<FontAtlas>( features )
			},
			Settings_InstanceTimer{
				get_sp<Gui::InstanceTimer::Config>( configs ),
				get_sp<FontAtlas>( features )
			},
			Settings_Inventory{
				get_sp<Gui::Inventory::Config>( configs ),
				get_sp<FontAtlas>( features )
			},
			Settings_Settings{
				get_sp<Gui::Settings::Config>( configs ),
				get_sp<FontAtlas>( features )
			},
			Settings_Skillbar{
				get_sp<Gui::Skillbar::Config>( configs ),
				get_sp<FontAtlas>( features )
			},
			Settings_Generic{
				"The Underworld Timer displays the times when each of the "
				"objectives in The Underworld was taken, finished, and how "
				"long the completion took; as well as the times when the "
				"Reapers were spawned.\n\n"
				"Note that the Reaper-Pop times may differ within your party: "
				"Players near the Reaper-spawn will see the time when the "
				"Reaper is visible in the world while players out of compass "
				"range will see the time when that reaper was added to the "
				"party list, which usually happens a few seconds later.\n\n"
				"The information displayed for the Dhuum-objective show (Pop) "
				"when your party completed 10/10 objectives, (Take) when "
				"Dhuum turned hostile, (Done) Dhuum was slain, and (Time) how "
				"long the Dhuum-fight lasted.\n\n"
				"You can send the displayed information to Team Chat by "
				"clicking on it. Clicking on the objective name will send all "
				"four times, clicking on a specific time will only send that "
				"time to chat.",
				get_sp<Gui::UwTimer::Config>( configs )
			},
			Settings_DefaultTheme{
				get_sp<DefaultTheme::Config>( configs ),
				get_sp<Exit::Config>( configs ),
				get_sp<FontAtlas>( features )
			}
		},
		std::tuple{
			Settings_DhuumBot{
				get_sp<Module::DhuumBot>( features )
			},
			Settings_Dialogs{
				get_sp<Settings_Dialogs::Config>( configs )
			},
			Settings_ResignLog{},
			Settings_TargetInfo{},
		},
	},
	drawables{
		std::tuple_cat( sections.modules, sections.guis, sections.info )
	},
	configs{ configs }
{
}

void April::Gui::Settings::Display()
{
	ImGui::SetNextWindowPosCentered( ImGuiCond_FirstUseEver );
	ImGui::SetNextWindowSize( { 770, 390 }, ImGuiCond_FirstUseEver );

	if ( ImGui::Begin( config->window, *mouse ) )
	{
		auto const selected_changed =
			draw_sidebar_and_saveload(
				sections,
				selected_section,
				config->selected_feature,
				configs );

		ImGui::SameLine();

		ImGui::BeginChild( "Settings" );
		{
			if ( selected_changed )
				ImGui::SetScrollY( 0 );

			if ( config->selected_feature == "" )
			{
				ImGui::PushFont( fonts->Get( config->heading ) );
				ImGui::TextCentered( "Welcome" );
				ImGui::PopFont();

				ImGui::SeparatorSpaced( 0, 0.5f );
				ImGui::TextWrapped( desc_april );
				ImGui::SeparatorSpaced();
			}
			else
				draw_selected(
					config->selected_feature, drawables, *config, *fonts );
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

bool April::Gui::SetSettingsPanel(
	std::string const& name, Settings::Config& config )
{
	auto found = false;
	auto set_feature = [&]( auto const& ft )
	{
		if ( ft.name == name )
		{
			config.selected_feature = name;
			found = true;
		}
	};

	std::apply(
		[&set_feature]( auto&... ft ) { (..., set_feature( ft )); },
		names );

	return found;
}
