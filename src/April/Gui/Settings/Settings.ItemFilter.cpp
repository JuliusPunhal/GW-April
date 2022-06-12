
#include "April/Gui/Settings/Settings.ItemFilter.h"

#include "April/Utility/SettingsWidgets.h"

#include "ImGui/ImGui.hpp"

#include <algorithm>

using namespace GW::Constants::ItemID;


namespace {

	struct NamedItemID {
		GW::ItemModelID  id = 0;
		const char*      name = "";
	};

	bool operator==( NamedItemID const& lhs, GW::ItemModelID const rhs )
	{
		return lhs.id == rhs;
	}

	constexpr NamedItemID item_names[] = {
		// Consumables
		{ Cupcakes,                   "Cupcakes" },
		{ Apples,                     "Candy Apple" },
		{ Corns,                      "Candy Corn" },
		{ Pies,                       "Pumpkin Pies" },
		{ Warsupplies,                "War Supplies" },
		{ Eggs,                       "Golden Eggs" },
		{ Kabobs,                     "Drake Kabobs" },
		{ SkalefinSoup,               "Skalefin Soups" },
		{ PahnaiSalad,                "Pahnai Salads" },
		{ PumpkinCookie,              "Pumpkin Cookies" },

		{ ConsEssence,                "Essences of Celerity" },
		{ ConsArmor,                  "Armors of Salvation" },
		{ ConsGrail,                  "Grails of Might" },
		{ Powerstone,                 "Powerstone of Courage" },
		{ SealOfTheDragonEmpire,      "Seal of the Dragon Empire" },
		{ BRC,                        "Blue Rock Candy" },
		{ GRC,                        "Green Rock Candy" },
		{ RRC,                        "Red Rock Candy" },

		{ LunarPig,                   "Lunars (Pig)" },
		{ LunarRat,                   "Lunars (Rat)" },
		{ LunarOx,                    "Lunars (Ox)" },
		{ LunarTiger,                 "Lunars (Tiger)" },
		{ LunarRabbit,                "Lunars (Rabbit)" },
		{ LunarDragon,                "Lunars (Dragon)" },
		{ LunarSnake,                 "Lunars (Snake)" },
		{ LunarHorse,                 "Lunars (Horse)" },
		{ LunarSheep,                 "Lunars (Sheep)" },
		{ LunarMonkey,                "Lunars (Monkey)" },
		{ LunarRooster,               "Lunars (Rooster)" },
		{ LunarDog,                   "Lunars (Dog)" },
		{ LunarToken,                 "Lunar Token" },

		{ Absinthe,                   "Absinthe" },
		{ AgedDwarvenAle,             "Aged Dwarven Ale" },
		{ AgedHuntersAle,             "Aged Hunter's Ale" },
		{ BottleOfJuniberryGin,       "Juniberry Gin" },
		{ BottleOfVabbianWine,        "Vabbian Wine" },
		{ Cider,                      "Cider" },
		{ DwarvenAle,                 "Dwarven Ale" },
		{ Eggnog,                     "Eggnog" },
		{ FlaskOfFirewater,           "Firewater" },
		{ Grog,                       "Grog" },
		{ HuntersAle,                 "Hunter's Ale" },
		{ Keg,                        "Keg" },
		{ KrytanBrandy,               "Krytan Brandy" },
		{ Ricewine,                   "Ricewine" },
		{ ShamrockAle,                "Shamrock Ale" },
		{ SpikedEggnog,               "Spiked Eggnog" },
		{ WitchsBrew,                 "Witch's Brew" },

		{ IdentificationKit,          "Identification Kit (cheap)" },
		{ IdentificationKit_Superior, "Identification Kit (Superior)" },
		{ SalvageKit,                 "Salvage Kit (cheap)" },
		{ SalvageKit_Expert,          "Salvage Kit (Expert)" },
		{ SalvageKit_Superior,        "Salvage Kit (Superior)" },

		{ GhostInTheBox,              "Ghosts in the Boxes" },
		{ Lockpick,                   "Lockpicks" },
		{ PhantomKey,                 "Phantom Keys" },
		{ ResScroll,                  "Scrolls of Resurrection" },

		{ GhastlyStone,               "Ghastly Summoning Stone" },
		{ GakiSummon,                 "Mystical Summoning Stone (Gaki)" },
		{ TurtleSummon,               "Jadeite Summoning Stone (Turtle)" },
		{ ImperialGuardSummon,        "Imperial Guard Reinforcement Order" },
		{ TenguSummon,                "Tengu Support Flare" },
		{ WarhornSummon,              "Shining Blade Warhorn" },

		{ ELGwen,                     "EL Gwen Tonic" },
		{ ELMiku,                     "EL Miku Tonic" },
		{ ELMargo,                    "EL Margo Tonic" },
		{ ELZenmai,                   "EL Zenmai Tonic" },

		{ Fruitcake ,                 "Fruitcakes" },
		{ RedBeanCake,                "Red Bean Cake" },
		{ CremeBrulee,                "Creme Brulees" },
		{ SugaryBlueDrink,            "Sugary Blue Drink" },
		{ ChocolateBunny,             "Chocolate Bunnies" },

		// Weapons
		{ DSR,                        "Dhuum's Soul Reaper" },
		{ EternalBlade,               "Eternal Blade" },
		{ VoltaicSpear,               "Voltaic Spear" },

		// Rare Drops
		{ ArmbraceOfTruth,            "Armbrace of Truth" },
		{ MargoniteGem,               "Margonite Gemstone" },
		{ StygianGem,                 "Stygian Gemstone" },
		{ TitanGem,                   "Titan Gemstone" },
		{ TormentGem,                 "Torment Gemstone" },

		{ MiniDhuum,                  "Miniature Dhuum" },

		// Materials
		{ GlobofEctoplasm,            "Glob of Ectoplasm" },
		{ ObsidianShard,              "Obsidian Shard" },
		{ Diamond,                    "Diamonds" },
		{ Ruby,                       "Rubies" },
		{ Sapphire,                   "Sapphires" },

		{ AmberChunk,                 "Amber Chunks" },
		{ JadeiteShard,               "Jadeite Shard" },
		{ MonstrousEye,               "Monstrous Eye" },

		{ Bone,                       "Bones" },
		{ PileofGlitteringDust,       "Dust" },
		{ Feather,                    "Feathers" },
		{ IronIngot,                  "Iron" },
		{ GraniteSlab,                "Granite" },

		// Bundles
		{ UnholyText,                 "Unholy Text (FoW)" },
	};


	void draw_ids( std::vector<GW::ItemModelID>& ids, int& new_model_id )
	{
		auto const x = ImGui::GetFrameHeight();
		auto const square = ImVec2{ x, x };

		auto const& y = ImGui::GetStyle().ItemSpacing.y ;
		ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { y, y } );

		auto const& color = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
		ImGui::PushStyleColor( ImGuiCol_Button, color );

		for ( auto it = ids.begin(); it != ids.end(); /**/ )
		{
			auto& id = *it;

			ImGui::PushID( &id );
			ImGui::InputIntegral( "##input", id, 0 );
			ImGui::SameLine();
			auto const clicked = ImGui::Button( "X", square );

			auto const name =
				std::find(
					std::begin( item_names ), std::end( item_names ), id );

			if ( name != std::end( item_names ) )
			{
				ImGui::SameLine();
				ImGui::Text( name->name );
			}

			ImGui::PopID();

			if ( clicked )
			{
				it = ids.erase( it );
			}
			else ++it;
		}

		ImGui::PopStyleColor();

		ImGui::PushID( &ids );

		ImGui::InputIntegral( "##New", new_model_id, 0 );
		ImGui::SameLine();
		if ( ImGui::Button( "Add ModelID" ) && new_model_id != 0 )
		{
			ids.push_back( new_model_id );
			std::sort( ids.begin(), ids.end() );
			new_model_id = 0;
		}
		ImGui::PopID();

		ImGui::PopStyleVar();
	}

	auto draw_rarity(
		const char* label,
		GW::Rarity rarity,
		std::vector<GW::Rarity>& rarities )
	{
		auto const it =
			std::find( rarities.begin(), rarities.end(), rarity );

		auto checked = (it != rarities.end());
		if ( ImGui::Checkbox( label, &checked ) )
		{
			if ( it == rarities.end() )
				rarities.emplace_back( rarity );
			else
				rarities.erase( it );
		}
	}

	void draw_rarities( std::vector<GW::Rarity>& rarities )
	{
		ImGui::PushID( &rarities );
		draw_rarity( "White",  GW::Rarity::White,  rarities );
		draw_rarity( "Blue",   GW::Rarity::Blue,   rarities );
		draw_rarity( "Purple", GW::Rarity::Purple, rarities );
		draw_rarity( "Gold",   GW::Rarity::Gold,   rarities );
		draw_rarity( "Green",  GW::Rarity::Green,  rarities );
		ImGui::PopID();
	}

}


April::Gui::Settings_ItemFilter::Settings_ItemFilter(
	std::shared_ptr<Module::ItemFilter>         item_filter,
	std::shared_ptr<Module::ItemFilter::Config> config )
	: item_filter{ item_filter }, config{ config }
{
}

void April::Gui::Settings_ItemFilter::Draw()
{
	ImGui::TextWrapped(
		"When activated, this module will block items from spawning on the "
		"floor.\n\n"
		"By default, *all* items will be hidden. This not only includes loot "
		"from dying foes, but also items you and party-members drop out of "
		"their inventory, or mission-critical bundles in dungeons, missions, "
		"and pvp.\n"
		"Define exceptions below to avoid softlocking yourself or missing out "
		"on loot. You can also spawn all hidden items at any time." );

	ImGui::SeparatorSpaced();

	DrawActivateButton( config->active );

	ImGui::SeparatorSpaced();

	auto const label =
		"Show "
		+ std::to_string( item_filter->size() )
		+ " suppressed Items###Show suppressed";

	if ( ImGui::Button( label ) )
		item_filter->SpawnSuppressedItems();

	ImGui::SeparatorSpaced();
	ImGui::TextWrapped(
		"Items with these ModelIDs will not be suppressed if they are "
		"reserved for you." );
	draw_ids( config->visible_user_items, new_model_id );

	ImGui::SeparatorSpaced();
	ImGui::TextWrapped(
		"Items with the selected rarities will not be suppressed if they are "
		"reserved for you." );
	draw_rarities( config->visible_user_rarities );

	ImGui::SeparatorSpaced();
	ImGui::TextWrapped(
		"Items with these ModelIDs will not be suppressed if they are "
		"reserved for other players." );
	draw_ids( config->visible_party_items, new_model_id );

	ImGui::SeparatorSpaced();
	ImGui::TextWrapped(
		"Items with the selected rarities will not be suppressed if they are "
		"reserved for other players." );
	draw_rarities( config->visible_party_rarities );
}
