
#include "April/Gui/Inventory.h"
#include "April/Gui/Inventory.StateToColor.hpp"

#include "April/Config/Gui/Inventory.Config.hpp"
#include "April/Utility/ConsumableState.h"
#include "April/Utility/stl.h"
#include "April/Framework/WndProc.h"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"

#include <variant>

namespace config = April::Gui::InventoryConfig;

using April::ConsumablesMgr;
using April::RGBA;


namespace {

	auto item_color( GW::Item const* item ) -> RGBA
	{
		if ( item == nullptr )
			return config::no_item;

		auto const consumable = April::is_consumable( item->model_id );
		if ( consumable == nullptr )
			return config::unknown_item;

		auto const state = consumable_state( *consumable );
		return April::Gui::StateToColor( state );
	}

	constexpr auto surface_color( RGBA color ) -> RGBA
	{
		color.a *= config::button_alpha;
		return color;
	}

	auto get_label( GW::Item const* item, ConsumablesMgr const& mgr )
	{
		using April::ConsumablesMgr;
		using namespace config;

		if ( item == nullptr ) 
			return label_no_item;

		auto const visitor = std::overloaded{ 
			// need to pass &mgr to all lambdas to avoid corrupted stack error
			// probably connected to __declspec(empty_bases)
			[&mgr]( ConsumablesMgr::UntilLoad )
			{ 
				return mgr.deactivating_quest == 0
					? label_until_load : label_until_objective;
			},
			[&mgr]( ConsumablesMgr::Persistent ) { return label_persistent; },
			[&mgr, item]( ConsumablesMgr::Inactive ) 
			{ 
				if constexpr ( label_inactive == label_unknown_item )
					return label_inactive;
				else if ( April::is_consumable( item->model_id ) )
					return label_inactive;
				else label_unknown_item;
			}
		};
		
		return std::visit( visitor, mgr.IsActive( item->model_id ) );
	}

	bool leader_pressed()
	{
		return ImGui::GetIO().KeyCtrl || ImGui::GetIO().KeyShift;
	}

	bool next_slot_fits_on_same_line()
	{
		auto const last_slot_end = ImGui::GetItemRectMax().x;
		auto const window_size = 
			ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
		auto const next_slot_end = last_slot_end + config::slot_size.w;

		return next_slot_end < window_size;
	}

}


April::Gui::Inventory::Inventory( std::shared_ptr<ConsumablesMgr> mgr )
	:
	cons_mgr{ std::move( mgr ) },
	font{ LoadFont( config::font_path, config::font_size ) }
{
}

void April::Gui::Inventory::Display() const
{
	auto const bags = GW::Items::GetBagArray();
	if ( bags == nullptr ) return;

	ImGui::Begin( config::window_name, config::window_flags );
	ImGui::PushFont( font );
	ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, config::show_border );
	ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, config::item_spacing );
	for ( auto bag_it = 1; bag_it < 5; ++bag_it )
	{
		if ( bags[bag_it] == nullptr ) continue;

		for ( auto const* item : bags[bag_it]->items )
		{
			auto const color = item_color( item );
			auto const surface = surface_color( color );
			auto const label = get_label( item, *cons_mgr );

			ImGui::PushID( item );
			ImGui::PushStyleColor( ImGuiCol_Border, color );
			ImGui::PushStyleColor( ImGuiCol_Button, surface );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive, surface );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, surface );
			{
				char buf[2] = { label, '\0' };
				ImGui::Button( buf, config::slot_size );
				if ( ImGui::IsItemHovered() && leader_pressed() )
				{
					WndProc::BlockMouseInput();
					if ( ImGui::GetIO().MouseClicked[0] && item )
					{
						cons_mgr->Activate( item->model_id );
					}
					else if ( ImGui::GetIO().MouseClicked[1] && item )
					{
						cons_mgr->Deactivate( item->model_id );
					}
				}
			}
			ImGui::PopStyleColor( 4 );
			ImGui::PopID();

            if ( next_slot_fits_on_same_line() )
				ImGui::SameLine();
		}
	}
	ImGui::PopStyleVar( 2 );
	ImGui::PopFont();
	ImGui::End();
}
