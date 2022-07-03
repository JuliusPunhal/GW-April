
#include "GWCA/GWCA.hpp"

#pragma warning( push, 0 )
#include "GWCA/Managers/AgentMgr.h"
#include "GWCA/Managers/CameraMgr.h"
#include "GWCA/Managers/ChatMgr.h"
#include "GWCA/Managers/CtoSMgr.h"
#include "GWCA/Managers/EffectMgr.h"
#include "GWCA/Managers/FriendListMgr.h"
#include "GWCA/Managers/GameThreadMgr.h"
#include "GWCA/Managers/GuildMgr.h"
#include "GWCA/Managers/ItemMgr.h"
#include "GWCA/Managers/MapMgr.h"
#include "GWCA/Managers/MemoryMgr.h"
#include "GWCA/Managers/MerchantMgr.h"
#include "GWCA/Managers/PartyMgr.h"
#include "GWCA/Managers/PlayerMgr.h"
#include "GWCA/Managers/RenderMgr.h"
#include "GWCA/Managers/SkillbarMgr.h"
#include "GWCA/Managers/StoCMgr.h"
#include "GWCA/Managers/TradeMgr.h"
#include "GWCA/Managers/UIMgr.h"
#pragma warning(pop)

#include "April/Utility/stl.h"

#include <algorithm>
#include <tuple>
#include <type_traits>
#include <vector>

using namespace GW::literals;


namespace {

	template<typename T, typename Packet_t, typename = void>
	struct has_PacketUpdate : std::false_type {};

	template<typename T, typename Packet_t>
	struct has_PacketUpdate<
		T,
		Packet_t,
		std::void_t<decltype(
			std::declval<T>().Update( std::declval<Packet_t&>() ) )>>
		: std::true_type
	{
	};


	class Drunk {
	public:
		Drunk() = default;

		auto GetDrunkTime() const -> GW::ms32
		{
			auto const time_since_update = GW::GetInstanceTime() - last_update;
			auto const remaining = duration - time_since_update;

			return std::max( remaining, 0_ms );
		}

		void Update( GW::Packet::StoC::UpdateTitle const& packet )
		{
			constexpr auto drunkard_title_id = 7;

			if ( packet.title_id != drunkard_title_id )
				return;

			auto const& titles = GW::GameContext::instance()->world->titles;
			auto const gained_points =
				packet.new_value - titles[drunkard_title_id].current_points;

			if ( gained_points == 1 )
			{
				g_Drunk.last_update = GW::GetInstanceTime();
				g_Drunk.duration =
					std::min( GW::GetDrunkTime() + 1_min, 5_min + 0_ms );
			}
			else if ( gained_points >= 3 )
			{
				g_Drunk.last_update = GW::GetInstanceTime();
				g_Drunk.duration = 5_min;
			}
		}

		void Update( GW::Packet::StoC::MapLoaded const& )
		{
			*this = {};
		}


	private:
		GW::ms32 last_update = 0_ms;
		GW::ms32 duration = 0_ms;
	} g_Drunk;


	class ItemOwners {
	public:
		ItemOwners() = default;

		auto GetItemOwner( GW::ItemID const id ) const
			-> std::optional<GW::AgentID>
		{
			auto const it = std::find( owners.begin(), owners.end(), id );
			if ( it == owners.end() )
				return std::nullopt;

			return it->owner;
		}

		void Update( GW::Packet::StoC::ItemGeneral_ReuseID const& packet )
		{
			auto const it =
				std::find( owners.begin(), owners.end(), packet.item_id );

			if ( it != owners.end() )
				owners.erase( it );
		}

		void Update( GW::Packet::StoC::MapLoaded const& )
		{
			owners.clear();
		}

		void Update( GW::Packet::StoC::UpdateItemOwner const& packet )
		{
			auto const it =
				std::find( owners.begin(), owners.end(), packet.item_id );

			if ( it == owners.end() )
			{
				owners.push_back( { packet.item_id, packet.owner_agent_id } );
			}
			else it->owner = packet.owner_agent_id;
		}


	private:
		struct Owner {
			GW::ItemID  item;
			GW::AgentID owner;

			bool operator==( GW::ItemID const id ) const { return item == id; }
		};

		std::vector<Owner> owners;
	} g_ItemOwners;


	class ResignLog {
	public:
		ResignLog() = default;

		auto Get() const -> auto const& { return resigners; }

		void Update( GW::Packet::StoC::MapLoaded const& )
		{
			resigners.clear();
		}

		void Update( GW::Packet::StoC::MessageCore const& packet )
		{
			auto const name = GW::GetResignerName( packet );
			if ( name.empty() )
				return;

			auto const* party = GW::GetPartyInfo();
			auto const& players = GW::GetPlayerArray();
			if ( party == nullptr
				|| not party->players.valid()
				|| not players.valid() )
			{
				return;
			}

			auto const resigner = find_player( name, *party, players );
			if ( resigner == nullptr )
				return;

			resigners.push_back( resigner->login_number );
		}


	private:
		std::vector<GW::LoginNumber> resigners{};


		static auto remove_pvp_suffix( wchar_t const* begin )
		{
			constexpr auto max_name_length = 19;
			auto const* end =
				std::find( begin, begin + max_name_length, L'\0' );

			auto const pvp_tag = std::find( begin, end, L'(' );
			if ( pvp_tag != end && pvp_tag > begin )
				end = pvp_tag - 1; // Player Name (#) -> end == space before (

			return std::wstring_view( begin, std::distance( begin, end ) );
		}

		static auto find_player(
			std::wstring_view const name,
			GW::PartyInfo const& party,
			GW::PlayerArray const& players )
			-> GW::PlayerPartyMember const*
		{
			auto const iter =
				std::find_if(
					party.players.begin(), party.players.end(),
					[&]( GW::PlayerPartyMember const& player )
					{
						auto const id = player.login_number;
						return
							id < players.size()
							&& remove_pvp_suffix( players[id].name ) == name;
					} );

			return iter != party.players.end() ? iter : nullptr;
		}
	} g_ResignLog;


	template<typename Module_t, typename Packet_t>
	void impl_UpdateModules( Module_t& mod, Packet_t const& packet  )
	{
		if constexpr ( has_PacketUpdate<Module_t, Packet_t>::value )
			mod.Update( packet );
	}

	template<typename Packet_t>
	void UpdateModules( Packet_t const& packet )
	{
		std::apply(
			[&packet]( auto&... mod )
			{
				(..., impl_UpdateModules( mod, packet ));
			},
			std::forward_as_tuple( g_Drunk, g_ItemOwners, g_ResignLog ) );
	}

}


bool GW::InitializeEx()
{
	using namespace GW::Packet::StoC;

	auto const success = Initialize();
	if ( not success )
		return false;

	static auto entry = GW::HookEntry{};
	auto on_packet = []( auto const& packet ) { UpdateModules( packet ); };

	GW::RegisterCallback<ItemGeneral_ReuseID>( &entry, on_packet );
	GW::RegisterCallback<MapLoaded>( &entry, on_packet );
	GW::RegisterCallback<MessageCore>( &entry, on_packet );
	GW::RegisterCallback<UpdateItemOwner>( &entry, on_packet );
	GW::RegisterCallback<UpdateTitle>( &entry, on_packet );

	return success;
}

void GW::detail::Enqueue( std::function<void()> const& fn )
{
	GW::GameThread::Enqueue( fn );
}

auto GW::GetAsAgentGadget( GW::Agent const* agent ) -> GW::AgentGadget const*
{
	return agent ? agent->GetAsAgentGadget() : nullptr;
}

auto GW::GetAsAgentItem( GW::Agent const* agent ) -> GW::AgentItem const*
{
	return agent ? agent->GetAsAgentItem() : nullptr;
}

auto GW::GetAsAgentLiving( GW::Agent const* agent ) -> GW::AgentLiving const*
{
	return agent ? agent->GetAsAgentLiving() : nullptr;
}

auto GW::GetAgentArray() -> GW::AgentArray
{
	return GW::Agents::GetAgentArray();
}

auto GW::GetAgentByID( GW::AgentID const id ) -> GW::Agent const*
{
	return GW::Agents::GetAgentByID( id );
}

auto GW::GetAgentLivingByID( GW::AgentID const id ) -> GW::AgentLiving const*
{
	return GetAsAgentLiving( GetAgentByID( id ) );
}

auto GW::GetCharacter() -> GW::AgentLiving const*
{
	auto const player = GW::Agents::GetCharacter();
	if (
		player == nullptr
		|| player->max_energy == 0     // spectating other player
		|| player->login_number == 0 ) // spectating hero
	{
		return nullptr;
	}

	return player;
}

auto GW::GetTarget() -> GW::Agent const*
{
	return GW::Agents::GetTarget();
}

auto GW::GetPlayerLoginNumber() -> GW::LoginNumber
{
	return GW::GameContext::instance()->character->player_number;
}

void GW::ChangeTarget( GW::Agent const& agent )
{
	GW::ChangeTarget( agent.agent_id );
}

void GW::ChangeTarget( GW::AgentID const agent_id )
{
	GW::detail::Enqueue(
		[=]() { GW::Agents::ChangeTarget( agent_id ); } );
}

void GW::ClearTarget()
{
	GW::ChangeTarget( 0u );
}

auto GW::GetPlayerAttribute( GW::AttributeID const id ) -> GW::Attribute const*
{
	auto const& party = GW::GameContext::instance()->world->attributes;

	auto const player = GW::GetCharacter();
	if ( player == nullptr )
		return nullptr;

	auto const player_attrs =
		std::find_if(
			party.begin(), party.end(),
			[player]( auto const& attr )
			{
				return attr.agent_id == player->agent_id;
			} );

	if ( player_attrs == party.end() || player_attrs == nullptr )
		return nullptr;

	return &player_attrs->attribute[static_cast<int>( id )];
}

int GW::GetHealthPoints( GW::AgentLiving const& agent )
{
	return static_cast<int>( agent.hp * agent.max_hp );
}

int GW::GetEnergyPoints( GW::AgentLiving const& agent )
{
	return static_cast<int>( agent.energy * agent.max_energy );
}

float GW::GetMaxAvailableEnergy( GW::AgentLiving const& agent )
{
	return *reinterpret_cast<float const*>( &agent.h0118 );
}

auto GW::GetSkill( GW::AgentLiving const& agent ) -> GW::SkillID
{
	return static_cast<GW::SkillID>( agent.skill );
}

auto GW::GetMorale( GW::AgentID const agent_id ) -> Morale
{
	struct Sub {
		char unk[0x18];
		int morale;
	};
	struct Unk {
		char unk[8];
		Sub* sub;
	};

	// world->h62c does not seem to ever be nullptr,
	// garbage values when loading
	auto const* p_array =
		reinterpret_cast<GW::Array<Unk>*>(
			&GW::GameContext::instance()->world->h062C );

	auto const index = agent_id & p_array->m_param;
	auto const morale = (*p_array)[index].sub->morale;
	return morale - 100;
}

auto GW::GetMorale() -> Morale
{
	return GW::GetMorale( GW::Agents::GetPlayerId() );
}

auto GW::GetInstanceTime() -> InstanceTime
{
	return InstanceTime{ GW::Map::GetInstanceTime() };
}

auto GW::GetInstanceType() -> GW::InstanceType
{
	return GW::Map::GetInstanceType();
}

auto GW::GetMapID() -> MapID
{
	return GW::Map::GetMapID();
}

auto GW::GetCurrentMapInfo() -> GW::AreaInfo const&
{
	return *GW::Map::GetCurrentMapInfo();
}

auto GW::GetMissionProgress() -> GW::MissionProgress
{
	// ptr become invalid but not nullptr
	return GameContext::instance()->character->progress_bar->progress;
}

bool GW::InCinematic()
{
	return GW::Map::GetIsInCinematic();
}

auto GW::GetSkillConstantData() -> GW::SkillDataArray const&
{
	auto const begin = &GW::SkillbarMgr::GetSkillConstantData( 0 );
	return *reinterpret_cast<GW::SkillDataArray const*>( begin );
}

auto GW::GetSkillConstantData( GW::SkillID const id ) -> GW::Skill const&
{
	auto const raw_id = static_cast<uint32_t>( id );
	return GW::GetSkillConstantData()[raw_id];
}

auto GW::GetSkillType( GW::SkillID const skill_id ) -> GW::SkillType
{
	auto const& skill = GW::GetSkillConstantData( skill_id );
	return static_cast<SkillType>( skill.type );
}

auto GW::GetPlayerSkillbar() -> GW::Skillbar const*
{
	return GW::SkillbarMgr::GetPlayerSkillbar();
}

auto GW::GetRecharge( GW::SkillbarSkill const& skill ) -> GW::ms32
{
	return ms32{ skill.GetRecharge() };
}

auto GW::GetSkillID( GW::SkillbarSkill const& skill ) -> GW::SkillID
{
	return static_cast<GW::SkillID>( skill.skill_id );
}

auto GW::GetPartyEffects() -> GW::AgentEffectsArray const&
{
	return GW::GameContext::instance()->world->party_effects;
}

auto GW::GetPlayerEffects() -> GW::EffectArray const*
{
	auto const player = GW::GetCharacter();
	if ( player == nullptr )
		return nullptr;

	for ( auto const& effects : GW::GetPartyEffects() )
	{
		if ( effects.agent_id == player->agent_id )
			return &effects.effects;
	}

	return nullptr;
}

auto GW::GetPlayerEffect( GW::SkillID const id ) -> GW::Effect const*
{
	auto const effects = GW::GetPlayerEffects();
	if ( effects == nullptr )
		return nullptr;

	for ( auto const& effect : *effects )
	{
		if ( GW::GetSkillID( effect ) == id )
			return &effect;
	}

	return nullptr;
}

auto GW::GetSkillID( GW::Effect const& effect ) -> GW::SkillID
{
	return static_cast<GW::SkillID>( effect.skill_id );
}

bool GW::UseSkill(
	int const slot, GW::AgentID const target, bool const ping )
{
	using namespace GW::Constants::Allegiance;
	using namespace GW::Constants::TargetType;

	auto const* skillbar = GW::GetPlayerSkillbar();
	if ( skillbar == nullptr )
		return false;

	auto const* player = GW::GetCharacter();
	if ( player == nullptr )
		return false;

	auto const* current_target = GW::GetAsAgentLiving( GW::GetTarget() );
	auto const  skill_id = GW::GetSkillID( skillbar->skills[slot] );
	auto const& skill_info = GW::GetSkillConstantData( skill_id );

	if ( skill_info.target == Self )
	{
		detail::Enqueue(
			[=]() { GW::SkillbarMgr::UseSkill( slot, 0u, ping ); } );
		return true;
	}

	if ( skill_info.target == AllyOrSelf && target == 0 )
	{
		// When GW::UseSkill() is called with target == 0, the skill should
		// be used on self. However, when using a skill that can target
		// others, the packet will actually be sent with the player's
		// agent_id.

		if (
			current_target
			&& current_target->allegiance != Hostile
			&& current_target->agent_id != player->agent_id )
		{
			GW::ChangeTarget( target );
		}

		auto const id = player->agent_id;
		detail::Enqueue(
			[=]() { GW::SkillbarMgr::UseSkill( slot, id, ping ); } );
		return true;
	}

	if ( current_target == nullptr || current_target->agent_id != target )
		GW::ChangeTarget( target );

	detail::Enqueue(
		[=]() { GW::SkillbarMgr::UseSkill( slot, target, ping ); } );
	return true;
}

auto GW::GetTimeRemaining( GW::Effect const& effect ) -> GW::ms32
{
	return ms32{ effect.GetTimeRemaining() };
}

auto GW::GetTimeRemaining( GW::SkillID const id ) -> GW::ms32
{
	auto longest = 0_ms;

	auto const effects = GW::GetPlayerEffects();
	if ( effects == nullptr )
		return longest;

	for ( auto const& effect : *effects )
	{
		if ( GW::GetSkillID( effect ) == id )
			longest = std::max( longest, GW::GetTimeRemaining( effect ) );
	}

	return longest;
}

auto GW::GetDrunkTime() -> GW::ms32
{
	return g_Drunk.GetDrunkTime();
}

bool GW::IsIdentified( GW::Item const& item )
{
	return item.interaction & 1;
}

auto GW::GetItemArray() -> GW::ItemArray const&
{
	return GameContext::instance()->items->item_array;
}

auto GW::GetInventoryBags() -> InventoryBags const*
{
	auto const* inventory = GameContext::instance()->items->inventory;
	if ( inventory == nullptr )
		return nullptr;

	return reinterpret_cast<InventoryBags const*>( &inventory->backpack );
}

auto GW::GetItemOwner( GW::ItemID const id ) -> std::optional<GW::AgentID>
{
	return g_ItemOwners.GetItemOwner( id );
}

auto GW::GetItemOwner( GW::Item const& item ) -> std::optional<GW::AgentID>
{
	return GW::GetItemOwner( item.item_id );
}

auto GW::GetRarity( GW::Item const& item ) -> Rarity
{
	if ( item.complete_name_enc == nullptr )
		return Rarity::Unknown;

	switch ( item.complete_name_enc[0] )
	{
		case 2621: return Rarity::White;
		case 2623: return Rarity::Blue;
		case 2626: return Rarity::Purple;
		case 2624: return Rarity::Gold;
		case 2627: return Rarity::Green;
		default: return Rarity::Unknown;
	}
}

auto GW::GetIdentKitUses(
	GW::ItemModelID const model_id, uint32_t const value )
	-> std::optional<uint32_t>
{
	using namespace GW::Constants::ItemID;

	switch ( model_id )
	{
		case IdentificationKit:
			return static_cast<uint32_t>( std::round( value / 2.0f ) );

		case IdentificationKit_Superior:
			return static_cast<uint32_t>( std::round( value / 2.5f ) );
	}

	return std::nullopt;
}

auto GW::GetSalvageKitUses(
	GW::ItemModelID const model_id, uint32_t const value )
	-> std::optional<uint32_t>
{
	using namespace GW::Constants::ItemID;

	switch ( model_id )
	{
		case SalvageKit:
			return static_cast<uint32_t>( std::round( value /  2.f ) );

		case SalvageKit_Expert:
			return static_cast<uint32_t>( std::round( value /  8.f ) );

		case SalvageKit_Superior:
			return static_cast<uint32_t>( std::round( value / 10.f ) );
	}

	return std::nullopt;
}

auto GW::GetIdentKitUses( GW::Item const& item ) -> std::optional<uint32_t>
{
	return GetIdentKitUses( item.model_id, item.value );
}

auto GW::GetSalvageKitUses( GW::Item const& item ) -> std::optional<uint32_t>
{
	return GetSalvageKitUses( item.model_id, item.value );
}

auto GW::SearchInventory( GW::ItemModelID const id ) -> GW::Item const*
{
	auto const* bags = GW::GetInventoryBags();
	if ( bags == nullptr )
		return nullptr;

	for ( auto const* bag : *bags )
	{
		if ( bag == nullptr )
			continue;

		for ( auto const* item : bag->items )
		{
			if ( item && item->model_id == id )
				return item;
		}
	}
	return nullptr;
}

void GW::UseItem( GW::Item const& item )
{
	return GW::Items::UseItem( &item );
}

void GW::IdentifyItem( GW::Item const& item, GW::Item const& kit )
{
	GW::CtoS::SendPacket(
		0xC, GAME_CMSG_ITEM_IDENTIFY, kit.item_id, item.item_id );
}

void GW::OpenXunlaiWindow()
{
	GW::GameThread::Enqueue( [](){ GW::Items::OpenXunlaiWindow(); } );
}

auto GW::GetPartyContext() -> GW::PartyContext const&
{
	// does not seem to be nullptr
	return *GW::GameContext::instance()->party;
}

auto GW::GetPartyInfo() -> GW::PartyInfo const*
{
	return GW::GetPartyContext().player_party;
}

auto GW::GetPlayerArray() -> PlayerArray const&
{
	return GameContext::instance()->world->players;
}

bool GW::GetIsPlayerLoaded()
{
	auto const* party = GW::PartyMgr::GetPartyInfo();

	if ( party == nullptr || not party->players.valid() )
		return false;

	auto const* player = GW::GetCharacter();
	if ( player == nullptr )
		return false;

	auto const* player_in_party =
		std::find_if(
			party->players.begin(), party->players.end(),
			[player]( auto const& member )
			{
				return member.login_number == player->login_number;
			} );

	if ( player_in_party == party->players.end() )
		return false;

	return player_in_party->connected();
}

bool GW::GetIsPlayerLeader()
{
	return GW::GetPartyContext().flag & 0x80;
}

bool GW::GetIsPartyLoaded()
{
	return GW::PartyMgr::GetIsPartyLoaded();
}

auto GW::GetPlayerPartyMember( GW::LoginNumber const login_number )
	-> GW::PlayerPartyMember const*
{
	auto const* party = GW::GetPartyInfo();
	if ( party == nullptr || not party->players.valid() )
		return nullptr;

	auto const party_member =
		std::find_if(
			party->players.begin(),
			party->players.end(),
			[&]( GW::PlayerPartyMember const& member )
			{
				return member.login_number == login_number;
			} );

	if ( party_member == party->players.end() )
		return nullptr;

	return party_member;
}

auto GW::GetPlayerPartyMember( GW::AgentLiving const& agent )
	-> GW::PlayerPartyMember const*
{
	return GW::GetPlayerPartyMember( agent.login_number );
}

auto GW::GetResignedPlayers() -> ResignedPlayers const&
{
	return g_ResignLog.Get();
}

bool GW::HasResigned( GW::PlayerPartyMember const& player )
{
	if ( not player.connected() )
		return false;

	auto const& resigners = GW::GetResignedPlayers();
	auto const resigned =
		std::find( resigners.begin(), resigners.end(), player.login_number );

	return resigned != resigners.end();
}

auto GW::CountConnected() -> int
{
	auto connected = []( GW::PlayerPartyMember const& player )
	{
		return player.connected();
	};

	auto const* party = GW::GetPartyInfo();
	if ( party == nullptr || not party->players.valid() )
		return 0;

	auto const& players = party->players;
	return std::count_if( players.begin(), players.end(), connected );
}

auto GW::CountConnectedAndResigned() -> int
{
	auto const* party = GW::GetPartyInfo();
	if ( party == nullptr || not party->players.valid() )
		return 0;

	auto const& players = party->players;
	return std::count_if( players.begin(), players.end(), GW::HasResigned );
}

auto GW::GetMessageBuffer() -> GW::MessageBuffer&
{
	return GW::GameContext::instance()->world->message_buff;
}

namespace {
	namespace ResignMessage {

		using namespace GW::Packet::StoC;
		using namespace std::string_view_literals;

		constexpr auto Prefix = L"\x7BFF\xC9C4\xAEAA\x1B9B\x0107"sv;
		constexpr auto Suffix = L"\x1\x0"sv;
		constexpr auto N =
			sizeof( MessageCore::message ) / sizeof( MessageCore::message[0] );

	}
}


bool GW::IsResignMessage( GW::Packet::StoC::MessageCore const& packet )
{
	using namespace ResignMessage;

	return std::wstring_view{ packet.message, Prefix.size() } == Prefix;
}

auto GW::GetResignerName( GW::Packet::StoC::MessageCore const& packet )
	-> std::wstring_view
{
	using namespace ResignMessage;

	if ( not IsResignMessage( packet ) )
		return std::wstring_view{};

	auto const name_begin = packet.message + Prefix.size();
	auto const name_end =
		std::find( name_begin, std::end( packet.message ), Suffix[0] );

	return std::wstring_view( name_begin, name_end - name_begin );
}

auto GW::CreateResignMessage( std::wstring_view const new_name )
	-> GW::Packet::StoC::MessageCore
{
	using namespace ResignMessage;
	constexpr auto max_length = N - Prefix.size() - Suffix.size();

	auto result = GW::Packet::StoC::MessageCore{};
	result.header = GAME_SMSG_CHAT_MESSAGE_CORE;

	auto const name_length = std::min( new_name.size(), max_length );
	auto const name_begin = result.message + Prefix.size();
	auto const name_end = name_begin + name_length;

	std::copy( Prefix.begin(), Prefix.end(), result.message );
	std::copy( new_name.begin(), new_name.end(), name_begin );
	std::copy( Suffix.begin(), Suffix.end(), name_end );

	return result;
}

void GW::SendChat( char const channel, const char* str )
{
	GW::Chat::SendChat( channel, str );
}

void GW::SendChat( char const channel, const wchar_t* str )
{
	GW::Chat::SendChat( channel, str );
}

void GW::SendChat( char const channel, std::string const& str )
{
	GW::Chat::SendChat( channel, str.c_str() );
}

void GW::SendChat( char const channel, std::wstring const& str )
{
	GW::Chat::SendChat( channel, str.c_str() );
}

void GW::WriteChat( GW::ChatChannel const channel, const char* msg )
{
	auto buf = std::array<wchar_t, 128>{};
	for ( auto it = 0; msg[it] != '\0'; ++it )
	{
		buf[it] = static_cast<wchar_t>( msg[it] );
	}

	GW::Chat::WriteChat(
		static_cast<GW::Chat::Channel>( channel.channel ), buf.data() );
}

void GW::WriteChat( GW::ChatChannel const channel, const wchar_t* msg )
{
	GW::Chat::WriteChat(
		static_cast<GW::Chat::Channel>( channel.channel ), msg );
}

void GW::WriteChat( GW::ChatChannel const channel, std::string const& msg )
{
	GW::WriteChat( channel, msg.c_str() );
}

void GW::WriteChat( GW::ChatChannel const channel, std::wstring const& msg )
{
	GW::Chat::WriteChat(
		static_cast<GW::Chat::Channel>( channel.channel ), msg.c_str() );
}

void GW::RegisterLocalMessageCallback(
	GW::HookEntry* entry,
	std::function<void( GW::HookStatus&, GW::LocalMessageInfo )> const& fn )
{
	GW::Chat::RegisterLocalMessageCallback(
		entry,
		[fn]( GW::HookStatus* status, int channel, wchar_t* msg )
		{
			fn( *status, { channel, msg } );
		} );
}

void GW::RegisterSendChatCallback(
	GW::HookEntry* entry,
	std::function<void( GW::HookStatus&, SendChatInfo )> const& fn )
{
	GW::Chat::RegisterSendChatCallback(
		entry,
		[fn]( GW::HookStatus* status, GW::Chat::Channel, wchar_t* msg )
		{
			fn( *status, { msg - 1 } );
		} );
}

void GW::detail::SendPacket( unsigned const size, void* buffer )
{
	GW::CtoS::SendPacket( size, buffer );
}

void GW::SendDialog( DialogID const id )
{
	GW::Agents::SendDialog( id );
}

auto GW::GetLastSentDialog() -> DialogID
{
	return GW::Agents::GetLastDialogId();
}

void GW::detail::RegisterCallback(
	GW::HookEntry* entry,
	uint32_t const header,
	GW::HookCallback<Packet::StoC::PacketBase*> const& fn )
{
	GW::StoC::RegisterPacketCallback( entry, header, fn );
}

void GW::detail::EmulatePacket( GW::Packet::StoC::PacketBase* packet )
{
	GW::StoC::EmulatePacket( packet );
}

auto GW::GetWindowHandle() -> HWND
{
	return GW::MemoryMgr::GetGWWindowHandle();
}

void GW::SetRenderCallback( std::function<void( IDirect3DDevice9* )> fn )
{
	GW::Render::SetRenderCallback( fn );
}

void GW::SetResetCallback( std::function<void( IDirect3DDevice9* )> fn )
{
	GW::Render::SetResetCallback( fn );
}
