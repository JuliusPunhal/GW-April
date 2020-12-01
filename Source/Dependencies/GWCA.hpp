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
	
	using Constants::MapID;
	using Constants::SkillID;
	using Constants::SkillType;

	using Map::GetMapID;

	auto GetInstanceTime() -> std::chrono::milliseconds;
	auto GetRecharge( GW::SkillbarSkill const& ) -> std::chrono::milliseconds;
	auto GetTimeRemaining( GW::Effect const& ) -> std::chrono::milliseconds;

	void SendChat( char channel, const char* msg );
	void SendChat( char channel, const wchar_t* msg );
	void SendChat( char channel, std::string const& msg );
	void SendChat( char channel, std::wstring const& msg );


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
		inline constexpr ObjectiveID NightmanCometh = 156;
		inline constexpr ObjectiveID Dhuum = 157;
	
	}

}
