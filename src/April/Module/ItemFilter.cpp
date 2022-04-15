
#include "April/Module/ItemFilter.h"

using Config = April::Module::ItemFilter::Config;
using namespace GW::Packet::StoC;


namespace {

	auto is_item( AgentAdd const& packet ) -> GW::Item const*
	{
		// filter non-item-agents
		if ( packet.type != 4 || packet.unk3 != 0 )
			return nullptr;

		auto const& items = GW::GetItemArray();
		auto const item_id = packet.agent_type;
		if ( item_id >= items.size() )
			return nullptr;

		return items[item_id];
	}

	bool want_to_hide(
		GW::Item const& item,
		std::vector<GW::ItemID> const& ids,
		std::vector<GW::Rarity> const& rarities )
	{
		// Check for model_id
		if ( std::find( ids.begin(), ids.end(), item.model_id ) != ids.end() )
			return false;

		// Check for rarity
		auto const item_rarity = GW::GetRarity( item );
		auto const rarity =
			std::find( rarities.begin(), rarities.end(), item_rarity );
		if ( rarity != rarities.end() )
			return false;

		return true;
	}

}


April::Module::ItemFilter::ItemFilter( std::shared_ptr<Config const> config )
	: config{ config }
{
}

void April::Module::ItemFilter::SpawnSuppressedItems()
{
	for ( auto const& packet : suppressed_packets )
		GW::EmulatePacket( packet );

	suppressed_packets.clear();
}

void April::Module::ItemFilter::Update(
	GW::HookStatus& status, AgentAdd const& packet )
{
	if ( not config->active )
		return;

	auto const* item = is_item( packet );
	if ( not item )
		return;

	auto const* player = GW::GetCharacter();
	if ( player == nullptr )
		return; // spectating, do not suppress

	auto const owner = GW::GetItemOwner( *item );
	auto const can_pick_up =
		owner == std::nullopt          // not reserved
		|| *owner == player->agent_id; // reserved for user

	if ( can_pick_up )
	{
		if (
			want_to_hide(
				*item,
				config->visible_user_items,
				config->visible_user_rarities ) )
		{
			status.blocked = true;
			suppressed_packets.push_back( packet );
		}
	}
	else
	{
		if (
			want_to_hide(
				*item,
				config->visible_party_items,
				config->visible_party_rarities ) )
		{
			status.blocked = true;
			suppressed_packets.push_back( packet );
		}
	}
}

void April::Module::ItemFilter::Update(
	GW::HookStatus& status, AgentRemove const& packet )
{
	// Block despawning the agent if the client never spawned it.

	auto const found =
		std::find_if(
			suppressed_packets.begin(), suppressed_packets.end(),
			[&packet]( auto const& suppressed_packet )
			{
				return suppressed_packet.agent_id == packet.agent_id;
			} );

	if ( found == suppressed_packets.end() )
		return;

	suppressed_packets.erase( found );
	status.blocked = true;
}

void April::Module::ItemFilter::Update( MapLoaded const& )
{
	suppressed_packets.clear();
}

auto April::Module::ItemFilter::Config::user_items()
	-> std::vector<GW::ItemModelID>
{
	return {
		// Rare and Valuable Items
		GW::Constants::ItemID::DSR,
		GW::Constants::ItemID::EternalBlade,
		GW::Constants::ItemID::MiniDhuum,
		GW::Constants::ItemID::VoltaicSpear,
		GW::Constants::ItemID::ArmbraceOfTruth,
		GW::Constants::ItemID::MargoniteGem,
		GW::Constants::ItemID::StygianGem,
		GW::Constants::ItemID::TitanGem,
		GW::Constants::ItemID::TormentGem,

		// Crafting Items
		GW::Constants::ItemID::Diamond,
		GW::Constants::ItemID::Ruby,
		GW::Constants::ItemID::Sapphire,
		GW::Constants::ItemID::GlobofEctoplasm,
		GW::Constants::ItemID::ObsidianShard,

		// Consumables
		GW::Constants::ItemID::Cupcakes,
		GW::Constants::ItemID::Apples,
		GW::Constants::ItemID::Corns,
		GW::Constants::ItemID::Pies,
		GW::Constants::ItemID::Eggs,
		GW::Constants::ItemID::Warsupplies,
		GW::Constants::ItemID::SkalefinSoup,
		GW::Constants::ItemID::PahnaiSalad,
		GW::Constants::ItemID::Kabobs,
		GW::Constants::ItemID::PumpkinCookie,

		// Rock Candy
		GW::Constants::ItemID::GRC,
		GW::Constants::ItemID::BRC,
		GW::Constants::ItemID::RRC,

		// Conset
		GW::Constants::ItemID::ConsEssence,
		GW::Constants::ItemID::ConsArmor,
		GW::Constants::ItemID::ConsGrail,

		// Lunars
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

		// Alcohol
		GW::Constants::ItemID::Absinthe,
		GW::Constants::ItemID::AgedDwarvenAle,
		GW::Constants::ItemID::AgedHuntersAle,
		GW::Constants::ItemID::BottleOfJuniberryGin,
		GW::Constants::ItemID::BottleOfVabbianWine,
		GW::Constants::ItemID::Cider,
		GW::Constants::ItemID::DwarvenAle,
		GW::Constants::ItemID::Eggnog,
		GW::Constants::ItemID::FlaskOfFirewater,
		GW::Constants::ItemID::Grog,
		GW::Constants::ItemID::HuntersAle,
		GW::Constants::ItemID::Keg,
		GW::Constants::ItemID::KrytanBrandy,
		GW::Constants::ItemID::Ricewine,
		GW::Constants::ItemID::ShamrockAle,
		GW::Constants::ItemID::SpikedEggnog,
		GW::Constants::ItemID::WitchsBrew,

		// Summons
		GW::Constants::ItemID::GhastlyStone,
		GW::Constants::ItemID::GakiSummon,
		GW::Constants::ItemID::TurtleSummon,

		// Summons x3
		GW::Constants::ItemID::TenguSummon,
		GW::Constants::ItemID::ImperialGuardSummon,
		GW::Constants::ItemID::WarhornSummon,

		// Tonics
		GW::Constants::ItemID::ELGwen,
		GW::Constants::ItemID::ELMiku,
		GW::Constants::ItemID::ELMargo,
		GW::Constants::ItemID::ELZenmai,

		// Other
		GW::Constants::ItemID::IdentificationKit,
		GW::Constants::ItemID::IdentificationKit_Superior,
		GW::Constants::ItemID::SalvageKit,
		GW::Constants::ItemID::SalvageKit_Expert,
		GW::Constants::ItemID::SalvageKit_Superior,
		GW::Constants::ItemID::PhantomKey,
		GW::Constants::ItemID::Lockpick,
		GW::Constants::ItemID::ResScroll,

		// Quest Items
		GW::Constants::ItemID::UnholyText,
	};
}

auto April::Module::ItemFilter::Config::party_items()
	-> std::vector<GW::ItemModelID>
{
	return {
		GW::Constants::ItemID::EternalBlade,
		GW::Constants::ItemID::VoltaicSpear,
		GW::Constants::ItemID::MiniDhuum,
		GW::Constants::ItemID::DSR
	};
}
