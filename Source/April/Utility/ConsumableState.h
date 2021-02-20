#pragma once

#include "April/Utility/Consumable.h"

#include <variant>


namespace April {

	template<class T>
	struct CState;

	template<>
	struct CState<Trivial> {
		struct WrongInstanceType {};
		struct NotActive {};
		struct Active {};

		using State = std::variant<
			WrongInstanceType, NotActive, Active>;

		static auto get_state( Trivial const& ) -> State;
	};

	template<>
	struct CState<RockCandy> {
		struct WrongInstanceType {};
		struct NotActive_NotSlowed {};
		struct NotActive_Slowed {};
		struct Active {};

		using State = std::variant<
			WrongInstanceType, NotActive_NotSlowed, NotActive_Slowed, Active>;

		static auto get_state( RockCandy const& ) -> State;
	};

	template<>
	struct CState<Conset> {
		struct WrongInstanceType {};
		struct NotActive_PartyNotLoaded {};
		struct NotActive_PartyLoaded {};
		struct Active{};

		using State = std::variant<
			WrongInstanceType, NotActive_PartyNotLoaded, NotActive_PartyLoaded,
			Active>;

		static auto get_state( Conset const& ) -> State;
	};

	template<>
	struct CState<LunarFortune> {
		struct WrongInstanceType {};
		struct NotActive {};
		struct Active {};

		using State = std::variant<
			WrongInstanceType, NotActive, Active>;

		static auto get_state( LunarFortune ) -> State;
	};

	template<>
	struct CState<Alcohol> {
		struct WrongInstanceType {};
		struct NotDrunk {};
		struct Drunk {};

		using State = std::variant<
			WrongInstanceType, NotDrunk, Drunk>;

		static auto get_state( Alcohol ) -> State;
	};

	template<>
	struct CState<SummoningStone> {
		struct WrongInstanceType {};
		struct NoSummoningSickness {};
		struct SummoningSickness {};
		// struct NoSummoningSickness_ButSummonAlive {};

		using State = std::variant<
			WrongInstanceType, NoSummoningSickness, SummoningSickness>;

		static auto get_state( SummoningStone ) -> State;
	};

	template<>
	struct CState<IdentKit> {
		struct WrongInstanceType {};
		struct NotAllItemsIdentified {};
		struct AllItemsIdentified {};

		using State = std::variant<
			WrongInstanceType, NotAllItemsIdentified, AllItemsIdentified>;

		static auto get_state( IdentKit ) -> State;
	};

	template<>
	struct CState<MoraleBooster> {
		struct WrongInstanceType {};
		struct AtPlus10 {};
		struct CouldCause10 {};
		struct CouldNotCause10 {};

		using State = std::variant<
			WrongInstanceType, AtPlus10, CouldCause10, CouldNotCause10>;

		static auto get_state( MoraleBooster const& ) -> State;
	};

	template<>
	struct CState<Tonic> {
		struct WrongInstanceType {};
		struct Tipsiness {};
		struct Disguised {};
		struct NotDisguised {};

		using State = std::variant<
			WrongInstanceType, Tipsiness, Disguised, NotDisguised>;

		static auto get_state( Tonic ) -> State;
	};


	using ConsumableState = std::variant<
		CState<Trivial>::State, CState<RockCandy>::State,
		CState<Conset>::State, CState<LunarFortune>::State,
		CState<Alcohol>::State, CState<SummoningStone>::State,
		CState<IdentKit>::State, CState<MoraleBooster>::State,
		CState<Tonic>::State>;

	auto consumable_state( Consumable const& consumable )
		-> ConsumableState;

}
