#pragma once

#include "April/Utility/ConsumableState.h"

#include "April/Utility/stl.h"

#include <variant>


namespace April {

	constexpr bool AllowUse( CState<Trivial>::State const state )
	{
		auto const visitor = std::overloaded{
			[]( CState<Trivial>::WrongInstanceType )	{ return false; },
			[]( CState<Trivial>::NotActive )			{ return true; },
			[]( CState<Trivial>::Active )				{ return false; }
		};
		return std::visit( visitor, state );
	}
	constexpr bool AllowUse( CState<RockCandy>::State const state )
	{
		auto const visitor = std::overloaded{
			[]( CState<RockCandy>::WrongInstanceType )		{ return false; },
			[]( CState<RockCandy>::NotActive_NotSlowed )	{ return false; },
			[]( CState<RockCandy>::NotActive_Slowed )		{ return true; },
			[]( CState<RockCandy>::Active )					{ return false; }
		};
		return std::visit( visitor, state );
	}
	constexpr bool AllowUse( CState<Conset>::State const state )
	{
		auto const visitor = std::overloaded{
			[]( CState<Conset>::WrongInstanceType )			{ return false; },
			[]( CState<Conset>::NotActive_PartyNotLoaded )	{ return false; },
			[]( CState<Conset>::NotActive_PartyLoaded )		{ return true; },
			[]( CState<Conset>::Active )					{ return false; }
		};
		return std::visit( visitor, state );
	}
	constexpr bool AllowUse( CState<LunarFortune>::State const state )
	{
		auto const visitor = std::overloaded{
			[]( CState<LunarFortune>::WrongInstanceType )	{ return false; },
			[]( CState<LunarFortune>::NotActive )			{ return true; },
			[]( CState<LunarFortune>::Active )				{ return false; }
		};
		return std::visit( visitor, state );
	}
	constexpr bool AllowUse( CState<Alcohol>::State const state )
	{
		auto const visitor = std::overloaded{
			[]( CState<Alcohol>::WrongInstanceType )	{ return false; },
			[]( CState<Alcohol>::NotDrunk )				{ return true; },
			[]( CState<Alcohol>::Drunk )				{ return false; },
		};
		return std::visit( visitor, state );
	}
	constexpr bool AllowUse( CState<SummoningStone>::State const state )
	{
		auto const visitor = std::overloaded{
			[]( CState<SummoningStone>::WrongInstanceType ) { return false; },
			[]( CState<SummoningStone>::NoSummoningSickness ) { return true; },
			[]( CState<SummoningStone>::SummoningSickness ) { return false; },
		};
		return std::visit( visitor, state );
	}
	constexpr bool AllowUse( CState<IdentKit>::State const state )
	{
		auto const visitor = std::overloaded{
			[]( CState<IdentKit>::WrongInstanceType )		{ return false; },
			[]( CState<IdentKit>::NotAllItemsIdentified )	{ return true; },
			[]( CState<IdentKit>::AllItemsIdentified )		{ return false; },
		};
		return std::visit( visitor, state );
	}
	constexpr bool AllowUse( CState<MoraleBooster>::State const state )
	{
		auto const visitor = std::overloaded{
			[]( CState<MoraleBooster>::WrongInstanceType )	{ return false; },
			[]( CState<MoraleBooster>::AtPlus10 )			{ return false; },
			[]( CState<MoraleBooster>::CouldCause10 )		{ return false; },
			[]( CState<MoraleBooster>::CouldNotCause10 )	{ return true; },
		};
		return std::visit( visitor, state );
	}
	constexpr bool AllowUse( CState<Tonic>::State const state )
	{
		auto const visitor = std::overloaded{
			[]( CState<Tonic>::WrongInstanceType )	{ return false; },
			[]( CState<Tonic>::Tipsiness )			{ return false; },
			[]( CState<Tonic>::Disguised )			{ return false; },
			[]( CState<Tonic>::NotDisguised )		{ return true; }
		};
		return std::visit( visitor, state );
	}

	constexpr bool AllowUse( ConsumableState const& state )
	{
		auto const allow = []( auto&& cstate ) { return AllowUse( cstate ); };

		return std::visit( allow, state );
	}

}
