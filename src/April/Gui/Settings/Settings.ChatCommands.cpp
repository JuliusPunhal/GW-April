
#include "April/Gui/Settings/Settings.ChatCommands.h"

#include "April/Module/ChatCommands.h"
#include "April/Utility/SettingsWidgets.h"
#include "April/Utility/stl.h"

#include "ImGui/ImGui.hpp"

using namespace April::Module;
using namespace std::string_literals;
using Abbreviation = ChatCommands::Abbreviation;
using Abbreviations = ChatCommands::Abbreviations;


namespace {

	constexpr auto popup_id_overwrite = "Overwrite existing abbreviation?";


	auto const desc_pcons_temp_on =
		"Enables consumables temporarily in Consumables Manager.\n"
		"- Takes 0...n ModelIDs as arguments\n"
		"- Providing no arguments will disable all temporary consumables.\n"
		"- Example: \""s + ChatCommands::cmd_pcons_temp_on + " 22269\" "
			"(enables cupcakes)\n"
		"- Example: \"" + ChatCommands::cmd_pcons_temp_on + " 24859 24860 "
			"24861\" (enables consets)\n"
		"- Example: \"" + ChatCommands::cmd_pcons_temp_on + "\""
			"(diables all)\n"
		"- Note: See abbreviations for more examples.\n\n";

	auto const desc_pcons_perm_on =
		"Enables persistent consumables in Consumables Manager.\n"
		"- Takes 0...n ModelIDs as arguments\n"
		"- Providing no arguments will disable all persistent consumables.\n"
		"- Example: \""s + ChatCommands::cmd_pcons_perm_on + " 22269\" "
			"(enables cupcakes)\n"
		"- Example: \"" + ChatCommands::cmd_pcons_perm_on + " 24859 24860 "
			"24861\" (enables consets)\n"
		"- Example: \"" + ChatCommands::cmd_pcons_perm_on + "\"(diables all)\n"
		"- Note: See abbreviations for more examples.\n\n";

	auto const desc_pcons_objective =
		"Sets objective for temporary consumables in Consumables Manager.\n"
		"- Takes 1 ObjectiveID as argument\n"
		"- Providing no arguments will disable objective\n"
		"- Example: \""s + ChatCommands::cmd_pcons_objective + " 151\" "
			"(disable when Wastes-Quest is completed)\n"
		"- Example: \"" + ChatCommands::cmd_pcons_objective + "\" or \""
			+ ChatCommands::cmd_pcons_objective + " 0\"\n"
		"- Note: See abbreviations for more examples.\n\n";

	auto const desc_toggle_window =
		"Toggles visiblity for specified window.\n"
		"- Takes 1 window-name (and optionally \"hide\" or \"show\") as "
			"arguments\n"
		"- Cannot toggle multiple Guis at once\n"
		"- Example: \""s + ChatCommands::cmd_toggle_window + " Settings "
			"show\"\n"
		"- Example: \"" + ChatCommands::cmd_toggle_window + " Settings\" "
			"(toggles visiblity)\n"
		"- Example: \"" + ChatCommands::cmd_toggle_window + " Underworld "
			"Times\" (even if there is a window called \"Underworld\", only "
			"\"Underworld Times\" is toggled.)\n"
		"- Note: See below for more information.\n\n";

	auto const desc_abbreviations =
		"Abbreviations / Aliases / Makros:\n"
		"Obviously, as is these chat-commands are virtually unusable. No one "
		"can type these convoluted commands or remember all necessary IDs.\n\n"
		"Therefore, whenever you send a command, April will check if any part "
		"of the message has been defined as an abbreviation and will replace "
		"it with its expanded version.\n"
		"With the help of abbreviations, you can:\n"
		"- Rename commands:\n"
		"\t- "s + ChatCommands::cmd_toggle_window + " -> /gui\n"
		"\t- " + ChatCommands::cmd_pcons_temp_on + " -> /s (for \"sweets\")\n"
		"\t- " + ChatCommands::cmd_openxunlai + " -> /x\n"
		"- Rename arguments:\n"
		"\t- " + std::to_string( GW::Constants::ItemID::Cupcakes ) +
			" -> cupcake\n"
		"\t- " + std::to_string( GW::Constants::ItemID::Apples ) +
			" -> apple\n"
		"\t- " + std::to_string( GW::Constants::ObjectiveID::Wastes ) +
			" -> wastes\n"
		"\t- " + std::to_string( GW::Constants::ObjectiveID::Planes ) +
			" -> planes\n"
		"- Combine arguments and commands\n"
		"\t- bu grail armor -> cons\n"
		"\t- /s cons -> /cons\n"
		"\t- /s cupcake corn egg /o planes /gui Chained Souls show -> /T4\n"
		"This way, you can use easy to type commands and give meaningful "
		"names to every ID you may need.\n\n"
		"When checking for abbreviations, April will examine each word "
		"(meaning all characters between two spaces) separately. Therefore, "
		"your abbreviations cannot themselves contain spaces. However, they "
		"can expand to expressions with spaces.\n"
		"Furthermore, when renaming commands, the forward-slash will also be "
		"replaced (since it's also a character between the two spaces). Make "
		"sure to include the slash in the expanded version, too.\n"
		"Example: rename \"" + ChatCommands::cmd_toggle_window + "\2 (with "
		"slash) to \"/gui\" (with slash).\n\n"
		"Said replacing happens recursively, meaning an abbreviation can be "
		"expanded into one or more another abbreviations, which in return "
		"will be replaced and so on.\n"
		"Abbreviations are case sensitive, so you can make /gui, /Gui, or "
		"/GUI expand into completely different commands; alternatively, "
		"make them expand to the same command to combat typos.\n"
		"Also remember that \"intermediate\" aliases are still recognized. "
		"This means even if you defined \"bu grail armor\" -> \"cons\", you "
		"can still use \"/s bu\" when you do not need a full conset.\n\n"
		"When chaining commands into one message, they will be evaluated and "
		"executed from left to right. This means, you can override a command "
		"in the same message:\n"
		"Example: given \"/s all_the_pcons /o planes\" -> \"/T4\" you can use "
		"\"/T4 /o pools\" to override the result of the \"/o\" command in "
		"\"/T4\"\n\n"
		"IMPORTANT: Defining loops (e.g. \"/gui\" -> \"/GUI\" and "
		"\"/GUI\" -> \"/gui\") will cause you to freeze or crash! This may be "
		"less obvious with real world examples.";

}


namespace {

	void draw_desc()
	{
		ImGui::TextWrapped(
			"The Chat-Commands module which will be triggered whenever you "
			"send a chat-message that begins with a forward-slash.\n\n"
			"The following commands will be recognized by April (click to "
			"expand):\n" );

		if ( ImGui::TreeNode( ChatCommands::cmd_openxunlai ) )
		{
			ImGui::TextWrapped(
				"Opens Xunlai Chest.\n"
				"- Will not work in instances or pre-searing\n\n" );
			ImGui::TreePop();
		}

		if ( ImGui::TreeNode( ChatCommands::cmd_pcons_temp_on ) )
		{
			ImGui::TextWrapped( desc_pcons_temp_on );
			ImGui::TreePop();
		}

		if ( ImGui::TreeNode( ChatCommands::cmd_pcons_temp_off ) )
		{
			ImGui::TextWrapped(
				"Disables all temporary consumables in Consumables "
				"Manager.\n\n" );
			ImGui::TreePop();
		}

		if ( ImGui::TreeNode( ChatCommands::cmd_pcons_perm_on ) )
		{
			ImGui::TextWrapped( desc_pcons_perm_on );
			ImGui::TreePop();
		}

		if ( ImGui::TreeNode( ChatCommands::cmd_pcons_perm_off ) )
		{
			ImGui::TextWrapped(
				"Disables all persistent consumables in Consumables "
				"Manager.\n\n" );
			ImGui::TreePop();
		}

		if ( ImGui::TreeNode( ChatCommands::cmd_pcons_objective ) )
		{
			ImGui::TextWrapped( desc_pcons_objective );
			ImGui::TreePop();
		}

		if ( ImGui::TreeNode( ChatCommands::cmd_toggle_window ) )
		{
			ImGui::TextWrapped( desc_toggle_window );
			ImGui::TreePop();
		}

		if ( ImGui::TreeNode( ChatCommands::cmd_suppressed_items ) )
		{
			ImGui::TextWrapped( "Shows items suppressed by Item Filter.\n\n" );
			ImGui::TreePop();
		}

		if ( ImGui::TreeNode( ChatCommands::cmd_reset_dx9 ) )
		{
			ImGui::TextWrapped(
				"Resets the DirectX9 device.\n"
				"This is mainly aimed at TexMod-users. When using April with "
				"TexMod, graphical glitches can occur when loading in a new "
				"map or changing the window size. Running this command can "
				"sometimes fix those.\n\n" );
			ImGui::TreePop();
		}

		if ( ImGui::TreeNode( ChatCommands::cmd_exit ) )
		{
			ImGui::TextWrapped(
				"Quits April. Unsaved changes will be lost.\n\n" );
			ImGui::TreePop();
		}

		ImGui::TextWrapped(
			"Chat Commands are case-sensitive and can be chained in one "
			"message!" );

		ImGui::SeparatorSpaced();

		ImGui::TextWrapped( desc_abbreviations );
	}

}


April::Gui::Settings_ChatCommands::Settings_ChatCommands(
	std::shared_ptr<ChatCommands::Config> config )
	: config{ config }
{
}

void April::Gui::Settings_ChatCommands::Draw()
{
	draw_desc();

	ImGui::SeparatorSpaced();

	auto const& col = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
	ImGui::PushStyleColor( ImGuiCol_Button, col );

	auto const& space = ImGui::GetStyle().ItemSpacing;
	auto const& inner = ImGui::GetStyle().ItemInnerSpacing;
	ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { inner.x, space.y } );

	if ( config->abbreviations.empty() )
		ImGui::Text( "No abbreviations found!" );

	auto const x = ImGui::GetFrameHeight();
	for (
		auto it = config->abbreviations.begin();
		it != config->abbreviations.end();
		/**/ )
	{
		ImGui::PushID( &*it );

		ImGui::InputText( "##no_label", &it->expanded() );

		ImGui::SameLine();
		auto const clicked = ImGui::Button( "X", { x, x } );

		ImGui::SameLine();
		ImGui::Text( it->abbreviated() );

		if ( clicked )
			it = config->abbreviations.erase( it );
		else
			++it;

		ImGui::PopID();
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();

	ImGui::SeparatorSpaced();

	ImGui::Text( "Add new abbreviation" );

	ImGui::InputText( "Abbreviated", &new_abbreviated );
	ImGui::SameLine();
	ImGui::HelpMarker( "Must not contain spaces!" );

	ImGui::InputText( "Expanded", &new_expanded );

	if ( ImGui::Button( "Add Abbreviation", { ImGui::CalcItemWidth(), 0 } ) )
	{
		if (
			new_abbreviation =
				make_Abbreviation( new_abbreviated, new_expanded ) )
		{
			conflict =
				std::find(
					config->abbreviations.begin(),
					config->abbreviations.end(),
					new_abbreviated );

			if ( conflict == config->abbreviations.end() )
			{
				config->abbreviations.push_back( *new_abbreviation );
				new_abbreviation = {};
				conflict =         {};
			}
			else ImGui::OpenPopup( popup_id_overwrite );

			new_abbreviated = {};
			new_expanded =    {};
		}
	}

	constexpr auto wnd_flags = ImGuiWindowFlags_NoSavedSettings;
	if ( ImGui::BeginPopupModal( popup_id_overwrite, nullptr,wnd_flags ) )
	{
		if (
			new_abbreviation.has_value()
			&& conflict != config->abbreviations.end() )
		{
			ImGui::Text(
				"You are trying to create an already existing abbreviation." );

			ImGui::SeparatorSpaced();

			ImGui::Text( "Existing Abbreviation:" );
			ImGui::PushID( "existing" );
			ImGui::InputText(
				conflict->abbreviated().c_str(),
				&conflict->expanded(),
				ImGuiInputTextFlags_ReadOnly );
			ImGui::PopID();

			ImGui::MoveCursorPosY( ImGui::GetFrameHeight() );

			ImGui::PushID( "new" );
			ImGui::Text( "New Abbreviation:" );
			ImGui::InputText(
				new_abbreviation->abbreviated().c_str(),
				&new_abbreviation->expanded(),
				ImGuiInputTextFlags_ReadOnly );
			ImGui::PopID();

			ImGui::SeparatorSpaced();

			auto const width = ImGui::WidthToFitNItems( 2 );
			ImGui::PushStyleColor( ImGuiCol_Button, MutedRed() );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive, White() );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, Red() );
			if ( ImGui::Button( "Overwrite", { width, 0 } ) )
			{
				*conflict =        *new_abbreviation;
				new_abbreviation = {};
				conflict =         {};
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopStyleColor( 3 );

			ImGui::SameLine();
			if ( ImGui::Button( "Cancel", { width, 0 } ) )
			{
				new_abbreviated =  new_abbreviation->abbreviated();
				new_expanded =     new_abbreviation->expanded();
				new_abbreviation = {};
				conflict =         {};
				ImGui::CloseCurrentPopup();
			}
		}
		else ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}
}
