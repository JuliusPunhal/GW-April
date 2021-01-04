#pragma once

#include <cstdint>
#include <ctime>
#include <functional>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>


#pragma warning( push, 0 )
#include "GWCA/GWCA.h"

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
#include "GWCA/Context/TextParser.h"
#include "GWCA/Context/TradeContext.h"
#include "GWCA/Context/WorldContext.h"

#include "GWCA/Packets/StoC.h"

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

#include <chrono>
#include <string>


namespace GW {

	using Constants::InstanceType;
	using Constants::MapID;
	using Constants::Profession;
	using Constants::SkillID;
	using Constants::SkillType;

	using Map::GetMapID;
	using Map::GetInstanceType;

	auto GetInstanceTime() -> std::chrono::milliseconds;
	auto GetRecharge( GW::SkillbarSkill const& ) -> std::chrono::milliseconds;
	auto GetTimeRemaining( GW::Effect const& ) -> std::chrono::milliseconds;

	// Only works for one active MissionProgress-Bar, either first or last.
	// In areas where there is only one, this should be the correct one.
	auto GetMissionProgress() -> float; // 0 <= progress <= 1

	using Morale = int; // -60 <= morale <= +10

	// Only works for player and heros
	auto GetMorale( GW::AgentID ) -> Morale;
	auto GetMorale() -> Morale; // player morale

	auto GetHealthPoints( GW::AgentLiving const& ) -> int;
	auto GetEnergyPoints( GW::AgentLiving const& ) -> int;

	enum class Rarity : char { White, Blue, Purple, Gold, Green, Unknown };

	auto GetRarity( GW::Item const& item ) -> Rarity;
	void IdentifyItem( GW::Item const& item, GW::Item const& kit );
	void UseItem( GW::Item const& );

	void SendChat( char channel, const char* msg );
	void SendChat( char channel, const wchar_t* msg );
	void SendChat( char channel, std::string const& msg );
	void SendChat( char channel, std::wstring const& msg );
	void WriteChat( GW::Chat::Channel, const char* msg );
	void WriteChat( GW::Chat::Channel, const wchar_t* msg );
	void WriteChat( GW::Chat::Channel, std::string const& msg );
	void WriteChat( GW::Chat::Channel, std::wstring const& msg );

	void UseSkill( uint32_t slot_0, uint32_t target_id = 0, bool ping = false );
	void UseSkill(
		uint32_t slot_0, GW::AgentLiving const& target, bool ping = false );

	template<typename Pred>
	auto SearchBags( int const first, int const last, Pred predicate )
		-> GW::Item*
	{
		auto const bags = GW::Items::GetBagArray();
		if ( bags == nullptr || bags[0] == nullptr ) return nullptr;

		for ( auto bag_iter = first; bag_iter <= last; ++bag_iter )
		{
			auto const* bag = bags[bag_iter];
			if ( bag == nullptr ) continue;

			for ( auto* item : bag->items )
			{
				if ( predicate( item ) )
					return item;
			}
		}

		return nullptr;
	}

	auto FindUnidentGold() -> GW::Item const*;


	using ObjectiveID = int;

	namespace Objectives {

		inline constexpr ObjectiveID Chamber = 146;
		inline constexpr ObjectiveID Restore = 147;
		inline constexpr ObjectiveID Escort = 148;
		inline constexpr ObjectiveID UWG = 149;
		inline constexpr ObjectiveID Vale = 150;
		inline constexpr ObjectiveID Wastes = 151;
		inline constexpr ObjectiveID Pits = 152;
		inline constexpr ObjectiveID Plains = 153;
		inline constexpr ObjectiveID Mnts = 154;
		inline constexpr ObjectiveID Pools = 155;
		inline constexpr ObjectiveID Completed_X_of_10_quests = 156;
		inline constexpr ObjectiveID Dhuum = 157;

	}

}


namespace GW::Constants::ItemID {

	// Weapons
	inline constexpr int DSR = 32823;
	inline constexpr int EternalBlade = 1045;
	inline constexpr int VoltaicSpear = 2071;

	// Consumables
	inline constexpr int IdentKit = 2989;
	inline constexpr int IdentKit_Superior = 5899;
	inline constexpr int Lockpick = 22751;
	inline constexpr int MiniDhuum = 32822;
	inline constexpr int PhantomKey = 5882;
	inline constexpr int PumpkinCookie = 28433;
	inline constexpr int ResScroll = 26501;

	// Summons
	inline constexpr int GakiSummon = 30960;
	inline constexpr int ImperialGuardSummon = 30210;
	inline constexpr int TurtleSummon = 30966;

	// Tonics
	inline constexpr int ELMiku = 36451;

}


namespace GW::Packet::StoC {

	struct RemoveEffect : Packet<RemoveEffect> {
		// TODO: Confirm layout
		GW::AgentID agent_id;
		unsigned	effect_id;
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

	struct UpdateItemOwner : Packet<UpdateItemOwner> {
		GW::ItemID  item_id;
		GW::AgentID owner_agent_id;
		float seconds_reserved;
	};
	unsigned const Packet<UpdateItemOwner>::STATIC_HEADER = 312;

	struct ItemGeneral_ReuseID : ItemGeneral {};
	unsigned const Packet<ItemGeneral_ReuseID>::STATIC_HEADER =
		GAME_SMSG_ITEM_GENERAL_INFO + 1;

	struct CreateMissionProgress : Packet<CreateMissionProgress> {
		uint8_t		id;
		wchar_t		unk1[122];
		wchar_t		unk2[122];
		uint8_t		unk3;
		uint32_t	pips;			// probably only signed char used
		float		regeneration;	// 0 <= x <= 1
		float		filled;			// 0 <= x <= 1
		uint32_t	color;			// not including border
	};
	unsigned const Packet<CreateMissionProgress>::STATIC_HEADER = 422;

	struct UpdateMissionProgress : Packet<UpdateMissionProgress> {
		uint8_t		id;
		uint8_t		unk1;
		uint32_t	pips;			// only signed char used
		float		regeneration;	// 0 <= x <= 1
		float		filled;			// 0 <= x <= 1
		uint32_t	color;			// not including border
	};
	unsigned const Packet<UpdateMissionProgress>::STATIC_HEADER = 424;

}
