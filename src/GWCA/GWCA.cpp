
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

using namespace GW::literals;


namespace {

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

}


bool GW::InitializeEx()
{
	using namespace GW::Packet::StoC;

	auto const success = Initialize();
	if ( not success )
		return false;

	static auto entry = GW::HookEntry{};
	auto on_packet = []( auto const& packet ) { g_Drunk.Update( packet ); };

	GW::RegisterCallback<MapLoaded>( &entry, on_packet );
	GW::RegisterCallback<UpdateTitle>( &entry, on_packet );

	return success;
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

auto GW::GetInventoryBags() -> InventoryBags const*
{
	auto const* inventory = GameContext::instance()->items->inventory;
	if ( inventory == nullptr )
		return nullptr;

	return reinterpret_cast<InventoryBags const*>( &inventory->backpack );
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

bool GW::GetIsPartyLoaded()
{
	return GW::PartyMgr::GetIsPartyLoaded();
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

void GW::detail::RegisterCallback(
	GW::HookEntry* entry,
	uint32_t const header,
	GW::HookCallback<Packet::StoC::PacketBase*> const& fn )
{
	GW::StoC::RegisterPacketCallback( entry, header, fn );
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
