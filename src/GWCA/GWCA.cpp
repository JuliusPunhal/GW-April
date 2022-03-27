
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

#include <array>


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

auto GW::GetInstanceTime() -> InstanceTime
{
	return InstanceTime{ GW::Map::GetInstanceTime() };
}

auto GW::GetMapID() -> MapID
{
	return GW::Map::GetMapID();
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
