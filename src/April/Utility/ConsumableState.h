#pragma once

#include "April/Utility/Consumables.h"

#include <variant>


namespace April::detail {

	template<class T>
	struct ConsumableState;


	template<>
	struct ConsumableState<Trivial> {
		struct WrongInstanceType {};
		struct NotActive {};
		struct Active {};

		using State = std::variant<WrongInstanceType, NotActive, Active>;

		static auto get_State( Trivial const& ) -> State;
	};

	template<>
	struct ConsumableState<RockCandy> {
		struct WrongInstanceType {};
		struct NotSlowed {};
		struct NotActive {};
		struct Active {};

		using State =
			std::variant<WrongInstanceType, NotSlowed, NotActive, Active>;

		static auto get_State( RockCandy const& ) -> State;
	};

	template<>
	struct ConsumableState<Conset> {
		struct WrongInstanceType {};
		struct PartyNotLoaded {};
		struct NotActive {};
		struct Active{};

		using State =
			std::variant<WrongInstanceType, PartyNotLoaded, NotActive, Active>;

		static auto get_State( Conset const& ) -> State;
	};

	template<>
	struct ConsumableState<Lunar> {
		struct WrongInstanceType {};
		struct NotActive {};
		struct Active {};

		using State = std::variant<WrongInstanceType, NotActive, Active>;

		static auto get_State( Lunar ) -> State;
	};

	template<>
	struct ConsumableState<Alcohol> {
		struct WrongInstanceType {};
		struct NotActive {};
		struct Active {};

		using State = std::variant<WrongInstanceType, NotActive, Active>;

		static auto get_State( Alcohol ) -> State;
	};

	template<>
	struct ConsumableState<SummoningStone> {
		struct WrongInstanceType {};
		struct SummoningSickness {};
		struct NotActive {};

		using State =
			std::variant<WrongInstanceType, SummoningSickness, NotActive>;

		static auto get_State( SummoningStone ) -> State;
	};

	template<>
	struct ConsumableState<IdentKit> {
		struct WrongInstanceType {};
		struct UnidentFound { GW::Item const* unident; };
		struct AllIdent {};

		using State = std::variant<WrongInstanceType, UnidentFound, AllIdent>;

		static auto get_State( IdentKit ) -> State;
	};

	template<>
	struct ConsumableState<MoraleBooster> {
		struct WrongInstanceType {};
		struct CouldCause10 {};
		struct NotActive {};

		using State = std::variant<WrongInstanceType, CouldCause10, NotActive>;

		static auto get_State( MoraleBooster const& ) -> State;
	};

	template<>
	struct ConsumableState<DpRemover> {
		struct WrongInstanceType {};
		struct NoDP {};
		struct NotActive {};

		using State = std::variant<WrongInstanceType, NoDP, NotActive>;

		static auto get_State( DpRemover const& ) -> State;
	};

	template<>
	struct ConsumableState<Tonic> {
		struct WrongInstanceType {};
		struct Tipsiness {};
		struct NotActive {};
		struct Active {};

		using State =
			std::variant<WrongInstanceType, Tipsiness, NotActive, Active>;

		static auto get_State( Tonic ) -> State;
	};

}


namespace April {

	using ConsumableState =
		std::variant<
			detail::ConsumableState<Trivial>::State,
			detail::ConsumableState<RockCandy>::State,
			detail::ConsumableState<Conset>::State,
			detail::ConsumableState<Lunar>::State,
			detail::ConsumableState<Alcohol>::State,
			detail::ConsumableState<SummoningStone>::State,
			detail::ConsumableState<IdentKit>::State,
			detail::ConsumableState<MoraleBooster>::State,
			detail::ConsumableState<DpRemover>::State,
			detail::ConsumableState<Tonic>::State>;

	auto get_ConsumableState( Consumable const& ) -> ConsumableState;

}
