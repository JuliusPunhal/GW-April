
#include "April/Gui/Settings/Settings.Inventory.h"

#include "April/Utility/SettingsWidgets.h"

#include "ImGui/ImGui.hpp"


April::Gui::Settings_Inventory::Settings_Inventory(
	std::shared_ptr<Inventory::Config> config,
	std::shared_ptr<FontAtlas const>   fonts )
	: config{ config }, fonts{ fonts }
{
}

void April::Gui::Settings_Inventory::Draw()
{
	ImGui::TextWrapped(
		"The Inventory Overlay is an interactive overlay which\n"
		"\t- displays the state of consumables in the inventory,\n"
		"\t- displays the state of the Consumables Manager, and\n"
		"\t- interfaces the Consumables Manager\n\n"
		"Use the settings below to create an immovable overlay over your F9-"
		"view inventory. If you do not use the F9-window, hide the window and "
		"skip this feature.\n"
		"When first showing the window, April will draw the background for "
		"you to more easily drag and drop the window into place; this will "
		"only work outside of the boxes, i.e. at the very edges of the "
		"window. Make sure to check both \"Hide Background \" and \"Disable "
		"Movement\" via Style -> Custom when you are finished. When "
		"everything is set up correctly, April will draw boxes around all of "
		"your inventory-slots.\n"
		"The colors indicate whether the item is recognized as a consumable "
		"and in what state the item currently is. It is recommended to set "
		"the color of \"No Item\" and \"Unknown Item\" to invisible.\n\n"
		"Shift+Left clicking a box will activate the consumable in the "
		"Consumables-Manager temporarily (see below), Shift+Left click again "
		"to activate it permanently. Shift+Right click will deactivate it.\n"
		"Whether a consumable is active is shown my the label inside the "
		"box:\n"
		"\tT = temporarily active (until next load into outpost)\n"
		"\tO = temporarily active (until objective is completed or load "
		"into outpost)\n"
		"\tP = permanently active" );

	ImGui::SeparatorSpaced();

	DrawWindowSettings( config->window );

	ImGui::SeparatorSpaced();
	if ( ImGui::InputInt( "Slots per row", &config->items_per_row ) )
	{
		if ( config->items_per_row < 5 )
			config->items_per_row = 5;
		if ( config->items_per_row > 15 )
			config->items_per_row = 15;
	}

	auto const str = config->collapsed_view ? "Collapsed" : "Default";
	if ( ImGui::BeginCombo( "Inventory Style", str ) )
	{
		if ( ImGui::Selectable( "Collapsed", config->collapsed_view ) )
			config->collapsed_view = true;
		if ( ImGui::Selectable( "Default", not config->collapsed_view ) )
			config->collapsed_view = false;

		ImGui::EndCombo();
	}

	if ( not config->collapsed_view )
	{
		ImGui::DragInt( "Spacing between bags", &config->bag_spacing );
	}

	ImGui::DragFloat2(
		"Slot Size", &config->slot_size.x, 0.25f, 0, 0, "%.0f" );

	ImGui::SeparatorSpaced();
	ImGui::Text( "Consumable State" );
	ImGui::ColorEdit4( "Empty Slot", &config->no_item.x );
	ImGui::ColorEdit4( "Unknown Item", &config->unknown_item.x );
	ImGui::ColorEdit4( "Wrong Instance Type", &config->wrong_instance_type.x );
	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::HelpMarker( "e.g. Cupcakes in Outpost." );

	ImGui::ColorEdit4( "Need not use", &config->need_not_use.x );
	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::HelpMarker(
		"Mostly when consumable already is active.\n"
		"Also Rock Candy when not crippled!" );

	ImGui::ColorEdit4( "Can not use", &config->can_not_use.x );
	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::HelpMarker(
		"e.g. Summoning Sickness preventing Summoning Stones." );

	ImGui::ColorEdit4( "Should Use", &config->suggest_use.x );
	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::HelpMarker( "Mostly when consumable is not active." );

	ImGui::ColorEdit4( "Special State", &config->special_state.x );
	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::HelpMarker(
		"e.g. Consets not active but not all players are loaded." );
}
