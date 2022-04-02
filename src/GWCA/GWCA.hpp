#pragma once

#include <cstdint>
#include <functional>

struct IDirect3DDevice9;

using DWORD = unsigned long;

#pragma warning( push, 0 )
#include "GWCA/Constants/Constants.h"

#include "GWCA/GameContainers/Array.h"
#include "GWCA/GameContainers/GamePos.h"
#include "GWCA/GameContainers/List.h"

#include "GWCA/GameEntities/Agent.h"
#include "GWCA/GameEntities/Attribute.h"
#include "GWCA/GameEntities/Camera.h"
#include "GWCA/GameEntities/Friendslist.h"
#include "GWCA/GameEntities/Guild.h"
#include "GWCA/GameEntities/Hero.h"
#include "GWCA/GameEntities/Item.h"
#include "GWCA/GameEntities/Map.h"
#include "GWCA/GameEntities/Match.h"
#include "GWCA/GameEntities/NPC.h"
#include "GWCA/GameEntities/Party.h"
#include "GWCA/GameEntities/Pathing.h"
#include "GWCA/GameEntities/Player.h"
#include "GWCA/GameEntities/Quest.h"
#include "GWCA/GameEntities/Skill.h"
#include "GWCA/GameEntities/Title.h"

#include "GWCA/Context/AgentContext.h"
#include "GWCA/Context/CharContext.h"
#include "GWCA/Context/Cinematic.h"
#include "GWCA/Context/GadgetContext.h"
#include "GWCA/Context/GameContext.h"
#include "GWCA/Context/GuildContext.h"
#include "GWCA/Context/ItemContext.h"
#include "GWCA/Context/MapContext.h"
#include "GWCA/Context/PartyContext.h"
#include "GWCA/Context/PreGameContext.h"
#include "GWCA/Context/TextParser.h"
#include "GWCA/Context/TradeContext.h"
#include "GWCA/Context/WorldContext.h"

#include "GWCA/Utilities/Hook.h"
#include "GWCA/Utilities/Hooker.h"

#include "GWCA/Packets/StoC.h"

#include "GWCA/GWCA.h"
#pragma warning( pop )

#include <chrono>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>


namespace GW {

	bool InitializeEx();

}

// Utility
namespace GW {

	using ms32 = std::chrono::duration<long, std::milli>;
	using sec32 = std::chrono::duration<long>;
	using min32 = std::chrono::duration<long, std::ratio<60>>;

	namespace literals {

		constexpr auto operator"" _ms ( unsigned long long const ms )
		{
			return ms32{ ms };
		}

		constexpr auto operator"" _s ( unsigned long long const sec )
		{
			return sec32{ sec };
		}

		constexpr auto operator"" _min ( unsigned long long const min )
		{
			return min32{ min };
		}

	}

}

// Agent
namespace GW {

	using Constants::Profession;
	using AttributeID = Constants::Attribute;


	auto GetAsAgentGadget( GW::Agent const* ) -> GW::AgentGadget const*;
	auto GetAsAgentItem( GW::Agent const* ) -> GW::AgentItem const*;
	auto GetAsAgentLiving( GW::Agent const* ) -> GW::AgentLiving const*;

	auto GetAgentByID( GW::AgentID ) -> GW::Agent const*;
	auto GetAgentLivingByID( GW::AgentID ) -> GW::AgentLiving const*;

	auto GetCharacter() -> GW::AgentLiving const*;

	auto GetPlayerAttribute( GW::AttributeID ) -> GW::Attribute const*;

}

// Map
namespace GW {

	using InstanceTime = ms32;
	using Constants::InstanceType;
	using Constants::MapID;
	using ObjectiveID = uint32_t;


	auto GetInstanceTime() -> InstanceTime;
	auto GetInstanceType() -> InstanceType;
	auto GetMapID() -> MapID;

}

// Skills
namespace GW {

	using Constants::SkillID;
	using Constants::SkillType;

	using SkillDataArray =
		std::array<
			GW::Skill,
			static_cast<int>( GW::SkillID::Heroic_Refrain ) + 1>;


	auto GetSkillConstantData() -> SkillDataArray const&;
	auto GetSkillConstantData( GW::SkillID ) -> GW::Skill const&;

	auto GetSkillType( GW::SkillID ) -> GW::SkillType;


	auto GetPlayerSkillbar() -> GW::Skillbar const*;

	auto GetRecharge( GW::SkillbarSkill const& ) -> GW::ms32;
	auto GetSkillID( GW::SkillbarSkill const& ) -> GW::SkillID;

}

// Effects
namespace GW {

	auto GetPartyEffects() -> GW::AgentEffectsArray const&;
	auto GetPlayerEffects() -> GW::EffectArray const*;
	auto GetPlayerEffect( GW::SkillID ) -> GW::Effect const*;

	auto GetSkillID( GW::Effect const& ) -> GW::SkillID;

	auto GetTimeRemaining( GW::Effect const& ) -> GW::ms32;
	auto GetTimeRemaining( GW::SkillID ) -> GW::ms32;

	auto GetDrunkTime() -> GW::ms32;

}

// Chat
namespace GW {

	struct ChatChannel {
		int channel;
	};

	inline constexpr auto EMOTE = ChatChannel{  6 };
	inline constexpr auto PARTY = ChatChannel{ 11 };


	void SendChat( char channel, const char* msg );
	void SendChat( char channel, const wchar_t* msg );
	void SendChat( char channel, std::string const& msg );
	void SendChat( char channel, std::wstring const& msg );

	void WriteChat( ChatChannel, const char* msg );
	void WriteChat( ChatChannel, const wchar_t* msg );
	void WriteChat( ChatChannel, std::string const& msg );
	void WriteChat( ChatChannel, std::wstring const& msg );

}

// StoC
namespace GW {

	namespace detail {
		void RegisterCallback(
			GW::HookEntry*,
			uint32_t header,
			GW::HookCallback<Packet::StoC::PacketBase*> const& );
	}

	template <typename T>
	void RegisterCallback(
		GW::HookEntry* entry,
		std::function<void( GW::HookStatus&, T& )> handler )
	{
		detail::RegisterCallback(
			entry,
			GW::Packet::StoC::Packet<T>::STATIC_HEADER,
			[handler](
				GW::HookStatus* status, GW::Packet::StoC::PacketBase* packet )
			{
				return handler( *status, *static_cast<T*>( packet ) );
			} );
	}

	template <typename T>
	void RegisterCallback(
		GW::HookEntry* entry, std::function<void( T& )> handler )
	{
		detail::RegisterCallback(
			entry,
			GW::Packet::StoC::Packet<T>::STATIC_HEADER,
			[handler](
				GW::HookStatus*, GW::Packet::StoC::PacketBase* packet )
			{
				return handler( *static_cast<T*>( packet ) );
			} );
	}

}

// Render
namespace GW {

	auto GetWindowHandle() -> HWND;
	void SetRenderCallback( std::function<void( IDirect3DDevice9* )> );
	void SetResetCallback( std::function<void( IDirect3DDevice9* )> );

}

// Other
namespace GW::Constants::ObjectiveID {

	inline constexpr GW::ObjectiveID Chamber = 146;
	inline constexpr GW::ObjectiveID Restore = 147;
	inline constexpr GW::ObjectiveID Escort = 148;
	inline constexpr GW::ObjectiveID UWG = 149;
	inline constexpr GW::ObjectiveID Vale = 150;
	inline constexpr GW::ObjectiveID Wastes = 151;
	inline constexpr GW::ObjectiveID Pits = 152;
	inline constexpr GW::ObjectiveID Planes = 153;
	inline constexpr GW::ObjectiveID Mnts = 154;
	inline constexpr GW::ObjectiveID Pools = 155;
	inline constexpr GW::ObjectiveID Completed_X_of_10_quests = 156;
	inline constexpr GW::ObjectiveID Dhuum = 157;

}

namespace GW::Packet::StoC {

	struct SkillRecharged : Packet<SkillRecharged> {
		uint32_t agent_id;
		uint32_t skill_id;
		uint32_t skill_instance;
	};
	unsigned const Packet<SkillRecharged>::STATIC_HEADER =
		GAME_SMSG_SKILL_RECHARGED;

	struct UpdateTitle : Packet<UpdateTitle> {
		uint32_t title_id;
		uint32_t new_value;
	};
	unsigned const Packet<UpdateTitle>::STATIC_HEADER = 244;

}
