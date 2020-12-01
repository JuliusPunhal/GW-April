
#include "Dependencies/GWCA.hpp"

#include <array>
#include <cstdint>

using namespace std::chrono;


auto GW::GetInstanceTime() -> milliseconds
{
	return milliseconds{ GW::Map::GetInstanceTime() };
}

auto GW::GetRecharge( GW::SkillbarSkill const& skill )  -> milliseconds
{
	// cast to signed to allow for negative times
	auto const recharge = static_cast<long>( skill.GetRecharge() );
	return milliseconds{ recharge };
}

auto GW::GetTimeRemaining( GW::Effect const& effect ) -> milliseconds
{
	// cast to signed to allow for negative times
	auto const uptime = static_cast<long>( effect.GetTimeRemaining() );
	return milliseconds{ uptime };
}

auto GW::GetMissionProgress() -> float
{
	struct ProgressBar {
		int						pips;
		std::array<uint8_t, 4>	color;		// ARGB
		std::array<uint8_t, 4>	background; // ARGB
		int						unk[7];
		float					progress;
		int						unk2[4];	// possibly fewer
	};
	
	auto* bar = 
		reinterpret_cast<ProgressBar*>(
			GW::GameContext::instance()->character->progress_bar);

	return bar->progress; // becomes invalid but not nullptr
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

	// world->h62c does not seems to ever be nullptr, 
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

auto GW::GetHealthPoints( GW::AgentLiving const& agent ) -> int
{
	return static_cast<int>( agent.hp * agent.max_hp );
}

auto GW::GetEnergyPoints( GW::AgentLiving const& agent ) -> int
{
	return static_cast<int>( agent.energy * agent.max_energy );
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

void GW::IdentifyItem(GW::Item const& item, GW::Item const& kit)
{
	GW::CtoS::SendPacket(
		0xC, GAME_CMSG_ITEM_IDENTIFY, kit.item_id, item.item_id );
}

void GW::UseItem( GW::Item const& item )
{
	GW::Items::UseItem( &item );
}

void GW::SendChat( char const channel, const char* str )
{
	return GW::Chat::SendChat( channel, str );
}

void GW::SendChat( char const channel, const wchar_t* str )
{
	return GW::Chat::SendChat( channel, str );
}

void GW::SendChat( char const channel, std::string const& str )
{
	return GW::Chat::SendChat( channel, str.c_str() );
}

void GW::SendChat( char const channel, std::wstring const& str )
{
	return GW::Chat::SendChat( channel, str.c_str() );
}

void GW::WriteChat( GW::Chat::Channel const channel, const char* msg )
{
	return GW::Chat::WriteChat( channel, msg );
}

void GW::WriteChat( GW::Chat::Channel const channel, const wchar_t* msg )
{
	return GW::Chat::WriteChat( channel, msg );
}

void GW::WriteChat( GW::Chat::Channel const channel, std::string const& msg )
{
	return GW::Chat::WriteChat( channel, msg.c_str() );
}

void GW::WriteChat( GW::Chat::Channel const channel, std::wstring const& msg )
{
	return GW::Chat::WriteChat( channel, msg.c_str() );
}

void GW::UseSkill(uint32_t slot_0, uint32_t target_id, bool ping)
{
	GW::GameThread::Enqueue(
		[=]() { GW::SkillbarMgr::UseSkill( slot_0, target_id, ping ); } );
}

void GW::UseSkill(uint32_t slot_0, GW::AgentLiving const& target, bool ping)
{
	GW::GameThread::Enqueue(
		[=, target_id = target.agent_id]()
		{
			GW::SkillbarMgr::UseSkill( slot_0, target_id, ping );
		} );
}

auto GW::FindUnidentGold() -> GW::Item const*
{
	auto const is_unident_gold = []( GW::Item const* item )
	{
		if ( item == nullptr ) return false;
		if ( (item->interaction & 1) != 0 ) return false;
		if ( GW::GetRarity( *item ) != GW::Rarity::Gold ) return false;

		return true;
	};

	return SearchBags( 1, 4, is_unident_gold );
}
