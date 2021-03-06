
#include "April/Module/UwTimer.h"

#include "April/Utility/UwReaper.h"

#include "Dependencies/GWCA.hpp"

#include <type_traits>

using namespace GW::Packet::StoC;
using namespace std::chrono_literals;


namespace {

	constexpr auto normalize( int const id ) noexcept
	{
		return id - GW::Objectives::Chamber;
	}

}


void April::UwTimer::Update()
{
	if ( GW::GetMapID() != GW::MapID::The_Underworld ) return;
	if ( GW::PartyMgr::GetIsPartyDefeated() ) return;

	times.instance_time = GW::GetInstanceTime();
}

void April::UwTimer::Update( AgentName const& packet )
{
	auto const reaper = GetReaperByName( packet.name_enc );
	if ( reaper == std::nullopt ) return;

	// Packet is also sent when reaper is added/removed from party-list.
	// Therefore, only change when packet is first recieved.
	auto const it = static_cast<int>(*reaper);
	if ( times.pop[it] > 0ms ) return;

	times.pop[it] = GW::GetInstanceTime();
}

void April::UwTimer::Update( ObjectiveAdd const& packet )
{
	if ( packet.objective_id == GW::Objectives::Dhuum )
		times.nightman_cometh = GW::GetInstanceTime();
}

void April::UwTimer::Update( ObjectiveUpdateName const& packet )
{
	if ( packet.objective_id >= GW::Objectives::Chamber
		&& packet.objective_id <= GW::Objectives::Pools )
	{
		auto const it = normalize( packet.objective_id );
		times.take[it] = GW::GetInstanceTime();
	}
}

void April::UwTimer::Update( ObjectiveDone const& packet )
{
	if ( packet.objective_id >= GW::Objectives::Chamber
		&& packet.objective_id <= GW::Objectives::Pools )
	{
		auto const it = normalize( packet.objective_id );
		times.done[it] = GW::GetInstanceTime();
	}
	else if ( packet.objective_id == GW::Objectives::Dhuum )
	{
		times.dhuum_done = GW::GetInstanceTime();
	}
}

void April::UwTimer::Update( AgentUpdateAllegiance const& packet )
{
	auto const* agent = GW::Agents::GetAgentByID( packet.agent_id );
	if ( agent == nullptr ) return;

	auto const* living = agent->GetAsAgentLiving();
	if ( living == nullptr ) return;
	if ( living->player_number != GW::Constants::ModelID::UW::Dhuum ) return;
	if ( living->hp < 1 ) return; // TODO: find content for turning hostile

	times.dhuum_hostile = GW::GetInstanceTime();
}

void April::UwTimer::Reset()
{
	if ( GW::GetMapID() == GW::MapID::The_Underworld )
		times = UwTimes{};
}
