
#include "April/Gui/Inventory.h"
#include "April/Gui/Inventory.StateToColor.hpp"

#include "April/Utility/ConsumableState.h"
#include "April/Utility/stl.h"
#include "April/Framework/WndProc.h"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"

#include <variant>

using April::ConsumablesMgr;
using April::RGBA;
using April::WH;
using Config = April::Gui::Inventory::Config;


namespace {

	auto item_color( GW::Item const* item, Config const& config ) -> RGBA
	{
		if ( item == nullptr )
			return config.no_item;

		auto const consumable = April::is_consumable( item->model_id );
		if ( consumable == nullptr )
			return config.unknown_item;

		auto const state = consumable_state( *consumable );
		return April::Gui::StateToColor( state, config );
	}

	constexpr auto surface_color( RGBA color, Config const& config ) -> RGBA
	{
		color.a *= config.button_alpha;
		return color;
	}

	auto get_label(
		GW::Item const* item, ConsumablesMgr const& mgr, Config const& config )
	{
		if ( item == nullptr )
			return config.label_no_item;

		auto const active = mgr.is_active( item->model_id );
		if ( active.persistent )
		{
			return config.label_persistent;
		}
		else if ( active.temporary )
		{
			return mgr.deactivating_quest == 0
				? config.label_until_load : config.label_until_objective;
		}
		else
		{
			if ( config.label_inactive == config.label_unknown_item )
				return config.label_inactive;

			return April::is_consumable( item->model_id )
				? config.label_inactive : config.label_unknown_item;
		}
	}

	bool leader_pressed()
	{
		return ImGui::GetIO().KeyCtrl || ImGui::GetIO().KeyShift;
	}

	void activate_or_promote( GW::ItemID const id, ConsumablesMgr& mgr )
	{
		auto const active = mgr.is_active( id );
		if ( active.temporary )
			mgr.activate_persistent( id );
		else if ( not active.persistent )
			mgr.activate_temporary( id );
	}

	void deactivate( GW::ItemID const id, ConsumablesMgr& mgr )
	{
		mgr.deactivate_persistent( id );
		mgr.deactivate_temporary( id );
	}

	bool next_slot_fits_on_same_line( WH const& slot_size )
	{
		auto const last_slot_end = ImGui::GetItemRectMax().x;
		auto const window_size =
			ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
		auto const next_slot_end = last_slot_end + slot_size.w;

		return next_slot_end < window_size;
	}

}


April::Gui::Inventory::Inventory( Config const& style )
	: config{ style }, font{ LoadFont( config.font ) }
{
}

void April::Gui::Inventory::Display( ConsumablesMgr& cons_mgr ) const
{
	auto const bags = GW::Items::GetBagArray();
	if ( bags == nullptr ) return;

	if ( ImGui::Begin( config.window ) )
	{
		ImGui::PushFont( font );
		ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, config.border );
		ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, config.item_spacing );
		for ( auto bag_it = 1; bag_it < 5; ++bag_it )
		{
			if ( bags[bag_it] == nullptr ) continue;

			for ( auto const* item : bags[bag_it]->items )
			{
				auto const color = item_color( item, config );
				auto const surface = surface_color( color, config );
				auto const label = get_label( item, cons_mgr, config );

				ImGui::PushID( item );
				ImGui::PushStyleColor( ImGuiCol_Border, color );
				ImGui::PushStyleColor( ImGuiCol_Button, surface );
				ImGui::PushStyleColor( ImGuiCol_ButtonActive, surface );
				ImGui::PushStyleColor( ImGuiCol_ButtonHovered, surface );
				{
					char buf[2] = { label, '\0' };
					ImGui::Button( buf, config.slot_size );
					if ( ImGui::IsItemHovered() && leader_pressed() )
					{
						WndProc::BlockMouseInput();
						if ( ImGui::GetIO().MouseClicked[0] && item )
						{
							activate_or_promote( item->model_id, cons_mgr );
						}
						else if ( ImGui::GetIO().MouseClicked[1] && item )
						{
							deactivate( item->model_id, cons_mgr );
						}
					}
				}
				ImGui::PopStyleColor( 4 );
				ImGui::PopID();

				if ( next_slot_fits_on_same_line( config.slot_size ) )
					ImGui::SameLine();
			}
		}
		ImGui::PopStyleVar( 2 );
		ImGui::PopFont();
	}
	ImGui::End();
}

auto April::Gui::Inventory::Config::LoadDefault() -> Config
{
	constexpr auto window_flags =
		ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoScrollWithMouse
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoBackground
		| ImGuiWindowFlags_NoFocusOnAppearing
		| ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoNavInputs
		| ImGuiWindowFlags_NoNavFocus;

	return Config{
		{ "C:\\Windows\\Fonts\\Gothic.ttf", 30 },

		WH{ 36, 44 },
		XY{ 1, 1 },
		true,
		0.f,

		Invisible(),
		Invisible(),
		Blue(),
		Green(),
		White(),
		Red(),
		Yellow(),

		'\0', '\0', 'P', 'L', 'Q', '\0',

		{ "Inventory", true, window_flags }
	};
}
