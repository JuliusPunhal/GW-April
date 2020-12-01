#pragma once

#include "Dependencies/GWCA.hpp"


namespace April::AgentFilterConfig {

	inline constexpr GW::ItemID allowed_ids_player[]{
		// Rare and valuable items
		GW::Constants::ItemID::EternalBlade,
		GW::Constants::ItemID::VoltaicSpear,
		GW::Constants::ItemID::Lockpick,
		GW::Constants::ItemID::ResScroll,
		GW::Constants::ItemID::GakiSummon,
		GW::Constants::ItemID::TurtleSummon,
		GW::Constants::ItemID::MiniDhuum,
		GW::Constants::ItemID::DSR,
		GW::Constants::ItemID::ELMiku,
		GW::Constants::ItemID::Diamond,
		GW::Constants::ItemID::Ruby,
		GW::Constants::ItemID::Sapphire,
		GW::Constants::ItemID::PhantomKey,
		GW::Constants::ItemID::GhastlyStone,
		GW::Constants::ItemID::GlobofEctoplasm,
		GW::Constants::ItemID::ObsidianShard,

		// consumables
		GW::Constants::ItemID::Cupcakes,
		GW::Constants::ItemID::Apples,
		GW::Constants::ItemID::Corns,
		GW::Constants::ItemID::Pies,
		GW::Constants::ItemID::Eggs,
		GW::Constants::ItemID::Warsupplies,
		GW::Constants::ItemID::SkalefinSoup,
		GW::Constants::ItemID::PahnaiSalad,
		GW::Constants::ItemID::Kabobs,

		GW::Constants::ItemID::GRC,
		GW::Constants::ItemID::BRC,
		GW::Constants::ItemID::RRC,

		GW::Constants::ItemID::ConsEssence,
		GW::Constants::ItemID::ConsArmor,
		GW::Constants::ItemID::ConsGrail,

		GW::Constants::ItemID::LunarDragon,
		GW::Constants::ItemID::LunarHorse,
		GW::Constants::ItemID::LunarMonkey,
		GW::Constants::ItemID::LunarOx,
		GW::Constants::ItemID::LunarRabbit,
		GW::Constants::ItemID::LunarRat,
		GW::Constants::ItemID::LunarRooster,
		GW::Constants::ItemID::LunarSheep,
		GW::Constants::ItemID::LunarSnake,
		GW::Constants::ItemID::LunarTiger,
		GW::Constants::ItemID::LunarDog,
		GW::Constants::ItemID::LunarPig,

		GW::Constants::ItemID::Grog,
		GW::Constants::ItemID::SpikedEggnog,
		GW::Constants::ItemID::FlaskOfFirewater,
		GW::Constants::ItemID::Absinthe,
		GW::Constants::ItemID::HuntersAle,

		GW::Constants::ItemID::GhastlyStone,
		GW::Constants::ItemID::LegionnaireStone,
		GW::Constants::ItemID::TurtleSummon,
		GW::Constants::ItemID::GakiSummon,
		GW::Constants::ItemID::ImperialGuardSummon,

		GW::Constants::ItemID::IdentKit,
		GW::Constants::ItemID::IdentKit_Superior,

		GW::Constants::ItemID::PumpkinCookie,

		GW::Constants::ItemID::ELMiku,
	};
	inline constexpr GW::Rarity allowed_rarities_player[]{
		GW::Rarity::Gold
	};

	inline constexpr GW::ItemID allowed_ids_party[]{
		GW::Constants::ItemID::EternalBlade,
		GW::Constants::ItemID::VoltaicSpear,
		GW::Constants::ItemID::MiniDhuum,
		GW::Constants::ItemID::DSR
	};
	inline constexpr GW::Rarity allowed_rarities_party[]{
		GW::Rarity::Unknown // Hide all rarities
	};

}
