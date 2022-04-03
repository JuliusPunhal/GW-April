#pragma once

#include "GWCA/GWCA.hpp"

#include <variant>


namespace April {

	struct Trivial {
		GW::ItemModelID model_id;
		GW::SkillID     skill_id;
	};

	struct RockCandy {
		GW::ItemModelID model_id;
		GW::SkillID     skill_id;
	};

	struct Conset {
		GW::ItemModelID model_id;
		GW::SkillID     skill_id;
	};

	struct Lunar {
		GW::ItemModelID model_id;
	};

	struct Alcohol {
		GW::ItemModelID model_id;
	};

	struct SummoningStone {
		GW::ItemModelID model_id;
	};

	struct IdentKit {
		GW::ItemModelID model_id;
	};

	struct MoraleBooster {
		GW::ItemModelID model_id;
		int             max_boost;
	};

	struct DpRemover {
		GW::ItemModelID model_id;
		int             max_removal;
	};

	struct Tonic {
		GW::ItemModelID model_id;
	};

	using Consumable =
		std::variant<
			Trivial, RockCandy, Conset, Lunar, Alcohol, SummoningStone,
			IdentKit, MoraleBooster, DpRemover, Tonic>;


	auto is_Consumable( GW::ItemModelID const ) -> Consumable const*;

	bool operator==( Consumable const&, GW::ItemModelID );

}
