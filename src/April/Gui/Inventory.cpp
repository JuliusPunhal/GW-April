
#include "April/Gui/Inventory.h"

#include "April/Utility/Consumables.h"
#include "April/Utility/ConsumableState.h"
#include "April/Utility/stl.h"

#include "GWCA/GWCA.hpp"

#include <variant>

using namespace April;
using Config = April::Gui::Inventory::Config;

template<typename T>
using CS = typename April::detail::ConsumableState<T>::State;


namespace {

	auto to_color( CS<Trivial> const state, Config const& config )
	{
		using S = detail::ConsumableState<Trivial>;

		auto const visitor = stl::overloaded{
			[&]( S::WrongInstanceType ) { return config.wrong_instance_type; },
			[&]( S::NotActive )         { return config.suggest_use; },
			[&]( S::Active )            { return config.need_not_use; }
		};
		return std::visit( visitor, state );
	}

	auto to_color( CS<RockCandy> const state, Config const& config )
	{
		using S = detail::ConsumableState<RockCandy>;

		auto const visitor = stl::overloaded{
			[&]( S::WrongInstanceType ) { return config.wrong_instance_type; },
			[&]( S::NotSlowed )         { return config.need_not_use; },
			[&]( S::NotActive )         { return config.suggest_use; },
			[&]( S::Active )            { return config.need_not_use; }
		};
		return std::visit( visitor, state );
	}

	auto to_color( CS<Conset> const state, Config const& config )
	{
		using S = detail::ConsumableState<Conset>;

		auto const visitor = stl::overloaded{
			[&]( S::WrongInstanceType ) { return config.wrong_instance_type; },
			[&]( S::PartyNotLoaded )    { return config.special_state; },
			[&]( S::NotActive )         { return config.suggest_use; },
			[&]( S::Active )            { return config.need_not_use; }
		};
		return std::visit( visitor, state );
	}

	auto to_color( CS<Lunar> const state, Config const& config )
	{
		using S = detail::ConsumableState<Lunar>;

		auto const visitor = stl::overloaded{
			[&]( S::WrongInstanceType ) { return config.wrong_instance_type; },
			[&]( S::NotActive )         { return config.suggest_use; },
			[&]( S::Active )            { return config.need_not_use; }
		};
		return std::visit( visitor, state );
	}

	auto to_color( CS<Alcohol> const state, Config const& config )
	{
		using S = detail::ConsumableState<Alcohol>;

		auto const visitor = stl::overloaded{
			[&]( S::WrongInstanceType ) { return config.wrong_instance_type; },
			[&]( S::NotActive )         { return config.suggest_use; },
			[&]( S::Active )            { return config.need_not_use; }
		};
		return std::visit( visitor, state );
	}

	auto to_color( CS<SummoningStone> const state, Config const& config )
	{
		using S = detail::ConsumableState<SummoningStone>;

		// Using config.need_not_use when user has no summoning sickness,
		// so that summoning stones do not constantly share the color with
		// inactive pcons.
		auto const visitor = stl::overloaded{
			[&]( S::WrongInstanceType ) { return config.wrong_instance_type; },
			[&]( S::SummoningSickness ) { return config.can_not_use; },
			[&]( S::NotActive )         { return config.need_not_use; }
		};
		return std::visit( visitor, state );
	}

	auto to_color( CS<IdentKit> const state, Config const& config )
	{
		using S = detail::ConsumableState<IdentKit>;

		auto const visitor = stl::overloaded{
			[&]( S::WrongInstanceType ) { return config.wrong_instance_type; },
			[&]( S::UnidentFound )      { return config.suggest_use; },
			[&]( S::AllIdent )          { return config.need_not_use; }
		};
		return std::visit( visitor, state );
	}

	auto to_color( CS<MoraleBooster> const state, Config const& config )
	{
		using S = detail::ConsumableState<MoraleBooster>;

		auto const visitor = stl::overloaded{
			[&]( S::WrongInstanceType ) { return config.wrong_instance_type; },
			[&]( S::CouldCause10 )      { return config.need_not_use; },
			[&]( S::NotActive )         { return config.suggest_use; }
		};
		return std::visit( visitor, state );
	}

	auto to_color( CS<DpRemover> const state, Config const& config )
	{
		using S = detail::ConsumableState<DpRemover>;

		auto const visitor = stl::overloaded{
			[&]( S::WrongInstanceType ) { return config.wrong_instance_type; },
			[&]( S::NoDP )              { return config.need_not_use; },
			[&]( S::NotActive )         { return config.suggest_use; }
		};
		return std::visit( visitor, state );
	}

	auto to_color( CS<Tonic> const state, Config const& config )
	{
		using S = detail::ConsumableState<Tonic>;

		// Using config.need_not_use when user is not disguised, so that tonics
		// do not constantly share the color with inactive pcons.
		auto const visitor = stl::overloaded{
			[&]( S::WrongInstanceType ) { return config.wrong_instance_type; },
			[&]( S::Tipsiness )         { return config.can_not_use; },
			[&]( S::NotActive )         { return config.need_not_use; },
			[&]( S::Active )            { return config.need_not_use; }
		};
		return std::visit( visitor, state );
	}

	auto get_color( Consumable const& cons, Config const& config )
	{
		auto const get_col = [&]( auto&& state )
		{
			return to_color( state, config );
		};

		return std::visit( get_col, get_ConsumableState( cons ) );
	}

	void draw_frame( ImVec4 const& border, Config const& config )
	{
		ImGui::PushStyleColor( ImGuiCol_Border, border );
		ImGui::Button( "##btn", config.slot_size );
		ImGui::PopStyleColor();
	}

	void draw_frame( GW::Item const* item, Config const& config )
	{
		if ( item == nullptr )
		{
			draw_frame( config.no_item, config );
		}
		else if ( auto const* cons = is_Consumable( item->model_id ) )
		{
			draw_frame( get_color( *cons, config ), config );
		}
		else draw_frame( config.unknown_item, config );
	}

	void draw_bag( GW::Bag const* bag, Config const& config, int& column )
	{
		if ( bag == nullptr )
			return;

		for ( auto const* item : bag->items )
		{
			draw_frame( item, config );

			if ( ++column != config.items_per_row  )
			{
				ImGui::SameLine();
			}
			else column = 0;
		}
	}

}


April::Gui::Inventory::Inventory( std::shared_ptr<Config> config )
	: config{ config }
{
}

void April::Gui::Inventory::Display()
{
	auto const* bags = GW::GetInventoryBags();
	if ( bags == nullptr )
		return;

	ImGui::SetNextWindowPosCentered( ImGuiCond_FirstUseEver );

	if ( ImGui::Begin( config->window ) )
	{
		ImGui::PushStyleColor( ImGuiCol_Button, Invisible() );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, Invisible() );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, Invisible() );
		ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1 );
		ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { 1, 1 } );

		if ( config->collapsed_view ) // draw concatenated bags
		{
			auto column = 0;
			for ( auto const* bag : *bags )
			{
				draw_bag( bag, *config, column );
			}
		}
		else // draw bags separately with spacing between them
		{
			for ( auto const* bag : *bags )
			{
				auto column = 0;
				draw_bag( bag, *config, column );
				ImGui::NewLine();
				ImGui::MoveCursorPosY(
					static_cast<float>( config->bag_spacing ) );
			}
		}

		ImGui::PopStyleVar( 2 );
		ImGui::PopStyleColor( 3 );
	}
	ImGui::End();
}
