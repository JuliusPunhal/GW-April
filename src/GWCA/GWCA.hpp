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

#include <array>
#include <chrono>
#include <optional>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>


namespace GW {

	bool InitializeEx();


	namespace detail {
		void Enqueue( std::function<void()> const& );
	}

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
	using Morale = int;                       // -60 ... +10

	using Constants::SkillID;


	auto GetAsAgentGadget( GW::Agent const* ) -> GW::AgentGadget const*;
	auto GetAsAgentItem( GW::Agent const* ) -> GW::AgentItem const*;
	auto GetAsAgentLiving( GW::Agent const* ) -> GW::AgentLiving const*;

	auto GetAgentArray() -> GW::AgentArray;
	auto GetAgentByID( GW::AgentID ) -> GW::Agent const*;
	auto GetAgentLivingByID( GW::AgentID ) -> GW::AgentLiving const*;

	auto GetCharacter() -> GW::AgentLiving const*;

	auto GetPlayerAttribute( GW::AttributeID ) -> GW::Attribute const*;

	int GetHealthPoints( GW::AgentLiving const& );
	int GetEnergyPoints( GW::AgentLiving const& );

	// returns 0...1 percentage how much energy is _not_ affected by overcast
	float GetMaxAvailableEnergy( GW::AgentLiving const& );

	auto GetSkill( GW::AgentLiving const& ) -> GW::SkillID;

	auto GetMorale( GW::AgentID ) -> Morale;
	auto GetMorale() -> Morale;

}

// Map
namespace GW {

	using InstanceTime = ms32;
	using Constants::InstanceType;
	using Constants::MapID;
	using MissionProgress = float; // 0 ... 1
	using ObjectiveID = uint32_t;


	auto GetInstanceTime() -> InstanceTime;
	auto GetInstanceType() -> InstanceType;
	auto GetMapID() -> MapID;

	auto GetCurrentMapInfo() -> GW::AreaInfo const&;

	auto GetMissionProgress() -> GW::MissionProgress;

	bool InCinematic();

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

// Items
namespace GW {

	namespace Constants {
		namespace ItemModelID = ItemID;
	}

	using InventoryBags = std::array<GW::Bag const*, 4>;
	using ItemModelID = decltype( GW::Item::model_id );
	enum class Rarity : char {
		White, Blue, Purple, Gold, Green, Unknown
	};


	bool IsIdentified( GW::Item const& );

	auto GetItemArray() -> GW::ItemArray const&;
	auto GetInventoryBags() -> InventoryBags const*;

	auto GetItemOwner( GW::ItemID ) -> std::optional<GW::AgentID>;
	auto GetItemOwner( GW::Item const& ) -> std::optional<GW::AgentID>;
	auto GetRarity( GW::Item const& ) -> GW::Rarity;

	auto GetIdentKitUses( GW::ItemModelID, uint32_t item_value )
		-> std::optional<uint32_t>;
	auto GetSalvageKitUses( GW::ItemModelID, uint32_t item_value )
		-> std::optional<uint32_t>;

	auto GetIdentKitUses( GW::Item const& ) -> std::optional<uint32_t>;
	auto GetSalvageKitUses( GW::Item const& ) -> std::optional<uint32_t>;

	auto SearchInventory( GW::ItemModelID ) -> GW::Item const*;

	void UseItem( GW::Item const& );
	void IdentifyItem( GW::Item const&, GW::Item const& ident_kit );

	void OpenXunlaiWindow();

}

// Party
namespace GW {

	using LoginNumber = decltype( AgentLiving::login_number );
	using ResignedPlayers = std::vector<GW::LoginNumber>;


	auto GetPartyContext() -> GW::PartyContext const&;
	auto GetPartyInfo() -> GW::PartyInfo const*;

	auto GetPlayerArray() -> GW::PlayerArray const&;

	bool GetIsPlayerLeader();
	bool GetIsPlayerLoaded();
	bool GetIsPartyLoaded();

	auto GetResignedPlayers() -> ResignedPlayers const&;

}

// Chat
namespace GW {

	struct ChatChannel {
		int channel;
	};

	inline constexpr auto EMOTE = ChatChannel{  6 };
	inline constexpr auto PARTY = ChatChannel{ 11 };


	using MessageBuffer = decltype( GW::WorldContext::message_buff );

	auto GetMessageBuffer() -> MessageBuffer&;


	bool IsResignMessage( GW::Packet::StoC::MessageCore const& );

	auto GetResignerName( GW::Packet::StoC::MessageCore const& )
		-> std::wstring_view;

	// Wraps new_resigner_name into the correct identifiers so GW recognizes
	// the message as a Resign-Message.
	auto CreateResignMessage( std::wstring_view const new_resigner_name )
		-> GW::Packet::StoC::MessageCore;

	void SendChat( char channel, const char* msg );
	void SendChat( char channel, const wchar_t* msg );
	void SendChat( char channel, std::string const& msg );
	void SendChat( char channel, std::wstring const& msg );

	void WriteChat( ChatChannel, const char* msg );
	void WriteChat( ChatChannel, const wchar_t* msg );
	void WriteChat( ChatChannel, std::string const& msg );
	void WriteChat( ChatChannel, std::wstring const& msg );


	struct LocalMessageInfo {
		int      channel;
		wchar_t* msg;     // encoded
	};

	void RegisterLocalMessageCallback(
		GW::HookEntry*,
		std::function<void( GW::HookStatus&, LocalMessageInfo )> const& );


	struct SendChatInfo {
		wchar_t* msg; // msg[0] == channel, e.g. !, ", /, ...
	};

	void RegisterSendChatCallback(
		GW::HookEntry*,
		std::function<void( GW::HookStatus&, SendChatInfo )> const& );

}

// CtoS
namespace GW {

	namespace detail {
		void SendPacket( unsigned size, void* buffer );
	}

	template<typename T>
	void SendPacket( T const& packet )
	{
		auto cpy = packet;
		detail::SendPacket( sizeof( T ), &cpy );
	}

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


	namespace detail {
		void EmulatePacket( GW::Packet::StoC::PacketBase* );
	}

	template<typename T>
	void EmulatePacket( T const& packet )
	{
		using namespace GW::Packet::StoC;

		GW::detail::Enqueue(
			[cpy = packet]() mutable
			{
				cpy.header = T::STATIC_HEADER;
				detail::EmulatePacket( reinterpret_cast<PacketBase*>( &cpy ) );
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

namespace GW::Constants::ItemID {

	// Consumables
	inline constexpr int IdentificationKit = 2989;
	inline constexpr int IdentificationKit_Superior = 5899;
	inline constexpr int SalvageKit = 2992;
	inline constexpr int SalvageKit_Expert = 2991;
	inline constexpr int SalvageKit_Superior = 5900;

	// Alcohol
	inline constexpr int BattleIsleIcedTea = 36682;
	inline constexpr int BottleOfJuniberryGin = 19172;
	inline constexpr int BottleOfVabbianWine = 19173;
	inline constexpr int ZehtukasJug = 19171;

	// DP
	inline constexpr int FourLeafClover = 22191; // party-wide
	inline constexpr int OathOfPurity = 30206;   // party-wide
	inline constexpr int PeppermintCandyCane = 6370;
	inline constexpr int RefinedJelly = 19039;
	inline constexpr int ShiningBladeRations = 35127;
	inline constexpr int WintergreenCandyCane = 21488;

	// Morale
	inline constexpr int ElixirOfValor = 21227;         // party-wide
	inline constexpr int Honeycomb = 26784;             // party-wide
	inline constexpr int PumpkinCookie = 28433;
	inline constexpr int RainbowCandyCane = 21489;      // party-wide
	inline constexpr int SealOfTheDragonEmpire = 30211; // party-wide

	// Summons
	inline constexpr int GakiSummon = 30960;
	inline constexpr int TurtleSummon = 30966;

	// Summons x3
	inline constexpr int TenguSummon = 30209;
	inline constexpr int ImperialGuardSummon = 30210;
	inline constexpr int WarhornSummon = 35126;

	// Tonics
	inline constexpr int ELGwen = 36442;
	inline constexpr int ELMiku = 36451;
	inline constexpr int ELMargo = 36456;
	inline constexpr int ELZenmai = 36493;

	// Other Consumables
	inline constexpr int ArmbraceOfTruth = 21127;
	inline constexpr int Lockpick = 22751;
	inline constexpr int PhantomKey = 5882;
	inline constexpr int ResScroll = 26501;

	// Weapons
	inline constexpr int DSR = 32823;
	inline constexpr int EternalBlade = 1045;
	inline constexpr int VoltaicSpear = 2071;

	// Minis
	inline constexpr int MiniDhuum = 32822;

	// Bundles
	inline constexpr int UnholyText = 2619;

}

namespace GW::Packet::CtoS {

	template<auto I>
	struct PacketBase {
		unsigned const header = I;
	};


	struct ReturnToOutpost : PacketBase<GAME_CMSG_PARTY_RETURN_TO_OUTPOST>{};

}

namespace GW::Packet::StoC {

	struct ItemGeneral_ReuseID : ItemGeneral {
	};
	unsigned const Packet<ItemGeneral_ReuseID>::STATIC_HEADER =
		GAME_SMSG_ITEM_GENERAL_INFO + 1;

	struct PlayerIsPartyLeader : Packet<PlayerIsPartyLeader> {
		uint32_t is_leader; // bool
	};
	unsigned const Packet<PlayerIsPartyLeader>::STATIC_HEADER =
		GAME_SMSG_PARTY_YOU_ARE_LEADER;

	struct RemoveEffect : Packet<RemoveEffect> {
		GW::AgentID agent_id;
		unsigned    effect_id; // not GW::SkillID
	};
	unsigned const Packet<RemoveEffect>::STATIC_HEADER =
		GAME_SMSG_EFFECT_REMOVED;

	struct SkillRecharged : Packet<SkillRecharged> {
		uint32_t agent_id;
		uint32_t skill_id;
		uint32_t skill_instance;
	};
	unsigned const Packet<SkillRecharged>::STATIC_HEADER =
		GAME_SMSG_SKILL_RECHARGED;

	// Corrected version of GuildGeneral
	struct UpdateGuildInfo : Packet<UpdateGuildInfo> {
		uint32_t guild_id;
		std::array<uint32_t, 4> gh_key;
		std::array<wchar_t, 32> guild_name;
		std::array<wchar_t, 6>  guild_tag;  // client accepts only 4 + \0
		uint32_t guild_features;
		uint32_t territory;
		uint32_t cape_background_color; // & 0xF0 = hue, & 0x0F = brightness
		uint32_t cape_detail_color;     // & 0xF0 = hue, & 0x0F = brightness
		uint32_t cape_emblem_color;     // & 0xF0 = hue, & 0x0F = brightness
		uint32_t cape_shape;            // 0 -   8
		uint32_t cape_detail;           // 0 -  31
		uint32_t cape_emblem;           // 0 - 173
		uint32_t cape_trim;             // 0 -  13
		uint32_t faction;
		uint32_t factions_count;
		uint32_t qualifier_points;
		uint32_t rating;
		uint32_t rank;
		uint32_t unk;
	};
	unsigned const Packet<UpdateGuildInfo>::STATIC_HEADER =
		GAME_SMSG_GUILD_GENERAL_INFO;

	struct UpdateItemOwner : Packet<UpdateItemOwner> {
		GW::ItemID  item_id;
		GW::AgentID owner_agent_id;
		float       seconds_reserved;
	};
	unsigned const Packet<UpdateItemOwner>::STATIC_HEADER = 308;

	struct UpdateTitle : Packet<UpdateTitle> {
		uint32_t title_id;
		uint32_t new_value;
	};
	unsigned const Packet<UpdateTitle>::STATIC_HEADER = 244;

}
