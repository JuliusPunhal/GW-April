#pragma once

#include "April/Gui/Inventory.h"

#include "April/Config/Gui/Inventory.Config.hpp"
#include "April/Utility/ConsumableState.h"
#include "April/Utility/stl.h"


namespace April::Gui {

	constexpr auto StateToColor( CState<Trivial>::State const state )
	{
		namespace c = April::Gui::InventoryConfig;
		using S = CState<Trivial>;

		auto const visitor = std::overloaded{
			[&]( S::WrongInstanceType )	{ return c::wrong_instance_type; },
			[&]( S::NotActive )			{ return c::suggest_use; },
			[&]( S::Active )			{ return c::need_not_use; }
		};
		return std::visit( visitor, state );
	}
	constexpr auto StateToColor( CState<RockCandy>::State const state )
	{
		namespace c = April::Gui::InventoryConfig;
		using S = CState<RockCandy>;

		auto const visitor = std::overloaded{
			[&]( S::WrongInstanceType )		{ return c::wrong_instance_type; },
			[&]( S::NotActive_NotSlowed )	{ return c::need_not_use; },
			[&]( S::NotActive_Slowed )		{ return c::suggest_use; },
			[&]( S::Active )				{ return c::need_not_use; }
		};
		return std::visit( visitor, state );
	}
	constexpr auto StateToColor( CState<Conset>::State const state )
	{
		namespace c = April::Gui::InventoryConfig;
		using S = CState<Conset>;

		auto const visitor = std::overloaded{
			[&]( S::WrongInstanceType )			{ return c::wrong_instance_type; },
			[&]( S::NotActive_PartyNotLoaded )	{ return c::special_state; },
			[&]( S::NotActive_PartyLoaded )		{ return c::suggest_use; },
			[&]( S::Active )					{ return c::need_not_use; }
		};
		return std::visit( visitor, state );
	}

	constexpr auto StateToColor( CState<LunarFortune>::State const state )
	{
		namespace c = April::Gui::InventoryConfig;
		using S = CState<LunarFortune>;

		auto const visitor = std::overloaded{
			[&]( S::WrongInstanceType )	{ return c::wrong_instance_type; },
			[&]( S::NotActive )			{ return c::suggest_use; },
			[&]( S::Active )			{ return c::need_not_use; }
		};
		return std::visit( visitor, state );
	}
	constexpr auto StateToColor( CState<Alcohol>::State const state )
	{
		namespace c = April::Gui::InventoryConfig;
		using S = CState<Alcohol>;

		auto const visitor = std::overloaded{
			[&]( S::WrongInstanceType )	{ return c::wrong_instance_type; },
			[&]( S::NotDrunk )			{ return c::suggest_use; },
			[&]( S::Drunk )				{ return c::need_not_use; },
		};
		return std::visit( visitor, state );
	}
	constexpr auto StateToColor( CState<SummoningStone>::State const state )
	{
		namespace c = April::Gui::InventoryConfig;
		using S = CState<SummoningStone>;

		// Using c::need_not_use when user has no summoning sickness, so
		// that summoning stones do not constantly share the color with inactive
		// pcons which may need to be activated urgently.
		auto const visitor = std::overloaded{
			[&]( S::WrongInstanceType )		{ return c::wrong_instance_type; },
			[&]( S::NoSummoningSickness )	{ return c::need_not_use; },
			[&]( S::SummoningSickness )		{ return c::can_not_use; },
		};
		return std::visit( visitor, state );
	}
	constexpr auto StateToColor( CState<IdentKit>::State const state )
	{
		namespace c = April::Gui::InventoryConfig;
		using S = CState<IdentKit>;

		auto const visitor = std::overloaded{
			[&]( S::WrongInstanceType )		{ return c::wrong_instance_type; },
			[&]( S::NotAllItemsIdentified )	{ return c::suggest_use; },
			[&]( S::AllItemsIdentified )	{ return c::need_not_use; },
		};
		return std::visit( visitor, state );
	}
	constexpr auto StateToColor( CState<MoraleBooster>::State const state )
	{
		namespace c = April::Gui::InventoryConfig;
		using S = CState<MoraleBooster>;

		auto const visitor = std::overloaded{
			[&]( S::WrongInstanceType )	{ return c::wrong_instance_type; },
			[&]( S::AtPlus10 )			{ return c::need_not_use; },
			[&]( S::CouldCause10 )		{ return c::need_not_use; },
			[&]( S::CouldNotCause10 )	{ return c::suggest_use; },
		};
		return std::visit( visitor, state );
	}
	constexpr auto StateToColor( CState<Tonic>::State const state )
	{
		namespace c = April::Gui::InventoryConfig;
		using S = CState<Tonic>;

		// Using c::need_not_use when user is not disguised, so that tonics do
		// not constantly share the color with inactive pcons which may need to
		// be activated urgently.
		auto const visitor = std::overloaded{
			[&]( S::WrongInstanceType )	{ return c::wrong_instance_type; },
			[&]( S::Tipsiness )			{ return c::can_not_use; },
			[&]( S::Disguised )			{ return c::need_not_use; },
			[&]( S::NotDisguised )		{ return c::need_not_use; }
		};
		return std::visit( visitor, state );
	}	

	constexpr auto StateToColor( ConsumableState const& state )
	{
		auto const get_col = [&]( auto&& cstate )
		{
			return StateToColor( cstate );
		};

		return std::visit( get_col, state );
	}

}
