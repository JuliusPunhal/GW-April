
#include "April/Gui/Settings/Settings.ShowKitUses.h"

#include "April/Utility/SettingsWidgets.h"

#include "ImGui/ImGui.hpp"

using namespace April::Module;


namespace {

	void emulate_item(
		GW::Item const& item, bool const uses_qty, ShowKitUses& show_kit_uses )
	{
		auto get_id = []( auto const& packet ) { return packet.item_id; };
		auto is_item = [&]( auto const& packet )
		{
			return std::visit( get_id, packet ) == item.item_id;
		};

		auto const& packets = show_kit_uses.GetUnchanged();

		auto const it =
			std::find_if( packets.begin(), packets.end(), is_item );

		if ( it != packets.end() ) // if item is a ident/salvage kit
		{
			auto emulate = [&]( auto const& packet )
			{
				if ( uses_qty )
				{
					auto cpy = packet;
					show_kit_uses.Update( cpy );
					GW::EmulatePacket( cpy );
				}
				else GW::EmulatePacket( packet );
			};
			std::visit( emulate, *it );
		}
	}

	void emulate_inventory( bool const uses_qty, ShowKitUses& show_kit_uses )
	{
		auto const* bags = GW::GetInventoryBags();
		if ( bags == nullptr )
			return;

		for ( auto const* bag : *bags )
		{
			if ( bag == nullptr )
				continue;

			for ( auto const* item : bag->items )
			{
				if ( item == nullptr )
					continue;

				emulate_item( *item, uses_qty, show_kit_uses );
			}
		}
	}

}


April::Gui::Settings_ShowKitUses::Settings_ShowKitUses(
	std::shared_ptr<Module::ShowKitUses>         show_kit_uses,
	std::shared_ptr<Module::ShowKitUses::Config> config )
	: show_kit_uses{ show_kit_uses }, config{ config }
{
}

void April::Gui::Settings_ShowKitUses::Draw()
{
	ImGui::TextWrapped(
		"When activated, this module will show the remaining uses of "
		"Identification- and Salvage-Kits in your Inventory.\n\n"
		"This module is implemented by making the game think that there are "
		"multiple kits stacked in one slot, which will have some undesirable "
		"side effects.\n"
		"Therefore, make sure the module is deactivated and the kits are "
		"reset, if you want to drop, sell, or trade them. Otherwise, you may "
		"be sending packets to the server, which you could normally not "
		"send!" );

	ImGui::SeparatorSpaced();

	if ( DrawActivateButton( config->active ) )
	{
		emulate_inventory( config->active, *show_kit_uses );
	}

	if ( show_kit_uses->GetUnchanged().empty() )
	{
		ImGui::PushStyleColor( ImGuiCol_Button, Invisible() );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, Invisible() );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, Invisible() );
		{
			ImGui::Button(
				"Kit-uses will be shown after next map-change.", { -1, 0 } );
		}
		ImGui::PopStyleColor( 3 );
	}
}
