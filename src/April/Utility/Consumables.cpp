
#include "April/Utility/Consumables.h"

using namespace April;
using namespace GW::Constants::ItemModelID;


namespace April {

	bool operator==( Consumable const& consumable, GW::ItemModelID const id )
	{
		return
			std::visit(
				[id]( auto const& cons ) { return cons.model_id == id; },
				consumable );
	}

}


namespace {

	Consumable cons[] = {
		Trivial{ Cupcakes,     GW::SkillID::Birthday_Cupcake_skill },
		Trivial{ Apples,       GW::SkillID::Candy_Apple_skill },
		Trivial{ Corns,        GW::SkillID::Candy_Corn_skill },
		Trivial{ Pies,         GW::SkillID::Pie_Induced_Ecstasy },
		Trivial{ Eggs,         GW::SkillID::Golden_Egg_skill },
		Trivial{ Warsupplies,  GW::SkillID::Well_Supplied },
		Trivial{ SkalefinSoup, GW::SkillID::Skale_Vigor },
		Trivial{ PahnaiSalad,  GW::SkillID::Pahnai_Salad_item_effect },
		Trivial{ Kabobs,       GW::SkillID::Drake_Skin },

		RockCandy{ GRC,        GW::SkillID::Green_Rock_Candy_Rush },
		RockCandy{ BRC,        GW::SkillID::Blue_Rock_Candy_Rush },
		RockCandy{ RRC,        GW::SkillID::Red_Rock_Candy_Rush },

		Conset{ ConsEssence,   GW::SkillID::Essence_of_Celerity_item_effect },
		Conset{ ConsArmor,     GW::SkillID::Armor_of_Salvation_item_effect },
		Conset{ ConsGrail,     GW::SkillID::Grail_of_Might_item_effect },

		Lunar{ LunarDragon },
		Lunar{ LunarHorse },
		Lunar{ LunarMonkey },
		Lunar{ LunarOx },
		Lunar{ LunarRabbit },
		Lunar{ LunarRat },
		Lunar{ LunarRooster },
		Lunar{ LunarSheep },
		Lunar{ LunarSnake },
		Lunar{ LunarTiger },
		Lunar{ LunarDog },
		Lunar{ LunarPig },

		Alcohol{ Absinthe },
		Alcohol{ AgedDwarvenAle },
		Alcohol{ AgedHuntersAle },
		Alcohol{ BottleOfJuniberryGin },
		Alcohol{ BottleOfVabbianWine },
		Alcohol{ Cider },
		Alcohol{ DwarvenAle },
		Alcohol{ Eggnog },
		Alcohol{ FlaskOfFirewater },
		Alcohol{ Grog },
		Alcohol{ HuntersAle },
		Alcohol{ Keg },
		Alcohol{ KrytanBrandy },
		Alcohol{ Ricewine },
		Alcohol{ ShamrockAle },
		Alcohol{ SpikedEggnog },
		Alcohol{ WitchsBrew },
		Alcohol{ ZehtukasJug },

		SummoningStone{ GhastlyStone },
		SummoningStone{ LegionnaireStone },
		SummoningStone{ TurtleSummon },
		SummoningStone{ GakiSummon },
		SummoningStone{ TenguSummon },
		SummoningStone{ ImperialGuardSummon },
		SummoningStone{ WarhornSummon },

		IdentKit{ IdentificationKit },
		IdentKit{ IdentificationKit_Superior },

		MoraleBooster{ PumpkinCookie, 10 },

		DpRemover{ FourLeafClover, 15 },
		DpRemover{ OathOfPurity, 15 },
		DpRemover{ PeppermintCandyCane, 60 },
		DpRemover{ RefinedJelly, 15 },
		DpRemover{ ShiningBladeRations, 15 },
		DpRemover{ WintergreenCandyCane, 15 },

		Tonic{ ELGwen },
		Tonic{ ELMargo },
		Tonic{ ELMiku },
		Tonic{ ELZenmai },
	};

}


auto April::is_Consumable( GW::ItemModelID const id ) -> Consumable const*
{
	auto const it = std::find( std::begin( cons ), std::end( cons ), id );
	return it != std::end( cons ) ? it : nullptr;
}
