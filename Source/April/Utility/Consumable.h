#pragma once

#include "Dependencies/GWCA.hpp"

#include <array>
#include <variant>


namespace April {

	struct Trivial {
		GW::ItemID	model_id;
		GW::SkillID	skill_id;
	};

	struct RockCandy {
		GW::ItemID	model_id;
		GW::SkillID	skill_id;
	};

	struct Conset {
		GW::ItemID	model_id;
		GW::SkillID	skill_id;
	};

	struct LunarFortune {
		GW::ItemID model_id;
	};

	struct Alcohol {
		GW::ItemID model_id;
	};

	struct SummoningStone {
		GW::ItemID model_id;
	};

	struct IdentKit {
		GW::ItemID model_id;
	};

	struct MoraleBooster {
		GW::ItemID	model_id;
		int			max_boost; // e.g. PumpkinCookie: max_boost = 10;
	};

	struct Tonic {
		GW::ItemID model_id;
	};


	using Consumable = std::variant<
		Trivial, RockCandy, Conset, LunarFortune, Alcohol, SummoningStone,
		IdentKit, MoraleBooster, Tonic>;

	constexpr bool operator==( Consumable const& consumable, GW::ItemID const id )
	{
		return std::visit(
			[id]( auto&& cons ) { return cons.model_id == id; },
			consumable );
	}
	constexpr bool operator==( Consumable const& lhs, Consumable const rhs )
	{
		return std::visit(
			[]( auto const& left, auto const& right )
			{
				return left.model_id == right.model_id;
			},
			lhs, rhs );
	}


	inline constexpr Trivial cupcake = { GW::Constants::ItemID::Cupcakes, GW::SkillID::Birthday_Cupcake_skill };
	inline constexpr Trivial candy_apple = { GW::Constants::ItemID::Apples, GW::SkillID::Candy_Apple_skill };
	inline constexpr Trivial candy_corn = { GW::Constants::ItemID::Corns, GW::SkillID::Candy_Corn_skill };
	inline constexpr Trivial pumpkin_pie = { GW::Constants::ItemID::Pies, GW::SkillID::Pie_Induced_Ecstasy };
	inline constexpr Trivial golden_egg = { GW::Constants::ItemID::Eggs, GW::SkillID::Golden_Egg_skill };
	inline constexpr Trivial war_supply = { GW::Constants::ItemID::Warsupplies, GW::SkillID::Well_Supplied };
	inline constexpr Trivial skalefin_soup = { GW::Constants::ItemID::SkalefinSoup, GW::SkillID::Skale_Vigor };
	inline constexpr Trivial pahnai_salad = { GW::Constants::ItemID::PahnaiSalad, GW::SkillID::Pahnai_Salad_item_effect };
	inline constexpr Trivial drake_kabob = { GW::Constants::ItemID::Kabobs, GW::SkillID::Drake_Skin };

	inline constexpr RockCandy green_rock_candy = { GW::Constants::ItemID::GRC, GW::SkillID::Green_Rock_Candy_Rush };
	inline constexpr RockCandy blue_rock_candy = { GW::Constants::ItemID::BRC, GW::SkillID::Blue_Rock_Candy_Rush };
	inline constexpr RockCandy red_rock_candy = { GW::Constants::ItemID::RRC, GW::SkillID::Red_Rock_Candy_Rush };

	inline constexpr Conset essence = { GW::Constants::ItemID::ConsEssence, GW::SkillID::Essence_of_Celerity_item_effect };
	inline constexpr Conset armor = { GW::Constants::ItemID::ConsArmor, GW::SkillID::Armor_of_Salvation_item_effect };
	inline constexpr Conset grail = { GW::Constants::ItemID::ConsGrail, GW::SkillID::Grail_of_Might_item_effect };

	inline constexpr LunarFortune lunar_dragon = { GW::Constants::ItemID::LunarDragon };
	inline constexpr LunarFortune lunar_horse = { GW::Constants::ItemID::LunarHorse };
	inline constexpr LunarFortune lunar_monkey = { GW::Constants::ItemID::LunarMonkey };
	inline constexpr LunarFortune lunar_ox = { GW::Constants::ItemID::LunarOx };
	inline constexpr LunarFortune lunar_rabbit = { GW::Constants::ItemID::LunarRabbit };
	inline constexpr LunarFortune lunar_rat = { GW::Constants::ItemID::LunarRat };
	inline constexpr LunarFortune lunar_rooster = { GW::Constants::ItemID::LunarRooster };
	inline constexpr LunarFortune lunar_sheep = { GW::Constants::ItemID::LunarSheep };
	inline constexpr LunarFortune lunar_snake = { GW::Constants::ItemID::LunarSnake };
	inline constexpr LunarFortune lunar_tiger = { GW::Constants::ItemID::LunarTiger };
	inline constexpr LunarFortune lunar_dog = { GW::Constants::ItemID::LunarDog };
	inline constexpr LunarFortune lunar_pig = { GW::Constants::ItemID::LunarPig };

	inline constexpr Alcohol grog = { GW::Constants::ItemID::Grog };
	inline constexpr Alcohol eggnog = { GW::Constants::ItemID::SpikedEggnog };
	inline constexpr Alcohol firewater = { GW::Constants::ItemID::FlaskOfFirewater };
	inline constexpr Alcohol absinthe = { GW::Constants::ItemID::Absinthe };
	inline constexpr Alcohol hunters_ale = { GW::Constants::ItemID::HuntersAle };

	inline constexpr SummoningStone ghastly = { GW::Constants::ItemID::GhastlyStone };
	inline constexpr SummoningStone legionnaire = { GW::Constants::ItemID::LegionnaireStone };
	inline constexpr SummoningStone turtle = { GW::Constants::ItemID::TurtleSummon };
	inline constexpr SummoningStone gaki = { GW::Constants::ItemID::GakiSummon };
	inline constexpr SummoningStone imperial_guards = { GW::Constants::ItemID::ImperialGuardSummon };

	inline constexpr IdentKit ident_regular = { GW::Constants::ItemID::IdentKit };
	inline constexpr IdentKit ident_superior = { GW::Constants::ItemID::IdentKit_Superior };

	inline constexpr MoraleBooster pumpkin_cookie = { GW::Constants::ItemID::PumpkinCookie, 10 };

	inline constexpr Tonic el_miku = { GW::Constants::ItemID::ELMiku };


	inline constexpr auto consumables = std::array<Consumable, 41>{
		cupcake, candy_apple, candy_corn, pumpkin_pie, golden_egg, war_supply,
		skalefin_soup, pahnai_salad, drake_kabob,

		green_rock_candy, blue_rock_candy, red_rock_candy,
		essence, armor, grail,

		lunar_dragon, lunar_horse, lunar_monkey, lunar_ox, lunar_rabbit,
		lunar_rat, lunar_rooster, lunar_sheep, lunar_snake, lunar_tiger,
		lunar_dog, lunar_pig,

		grog, eggnog, firewater, absinthe, hunters_ale,

		ghastly, legionnaire, turtle, gaki, imperial_guards,

		ident_regular, ident_superior,

		pumpkin_cookie,

		el_miku
	};


	constexpr auto is_consumable( GW::ItemID const model_id )
		-> Consumable const*
	{
		for ( auto const& cons : consumables )
		{
			if ( cons == model_id ) return &cons;
		}
		return nullptr;
	}

}
