
#include "April/Module/UwTimer.h"

#include "April/Utility/Reaper.h"

using namespace GW::literals;
using namespace GW::Constants::ObjectiveID;
using namespace GW::Packet::StoC;
using namespace std::chrono;


namespace {

	auto obj_to_idx( int const id )
	{
		return id - Chamber;
	}

	auto reaper_to_idx( April::Reaper const& reaper )
	{
		return static_cast<int>( reaper );
	}

}


April::Module::UwTimer::UwTimer( std::shared_ptr<UwTimesHistory> times )
	: times{ times }
{
	if ( GW::GetMapID() == GW::MapID::The_Underworld )
	{
		auto const start = system_clock::now() - GW::GetInstanceTime();
		times->push_back( UwTimes{ start } );
	}
}

void April::Module::UwTimer::Update()
{
	if ( times->size() == 0 ) return;
	if ( GW::GetMapID() != GW::MapID::The_Underworld ) return;

	times->back().instance_time = GW::GetInstanceTime();
}

void April::Module::UwTimer::Update( AgentName const& packet )
{
	auto const reaper = GetReaperByName( packet.name_enc );
	if ( reaper == std::nullopt )
		return;

	// Packet is also sent when reaper is added/removed from party-list.
	// Therefore, only change when packet is first recieved.
	auto const it = reaper_to_idx( *reaper );
	if ( times->back().pop[it] > 0_ms )
		return;

	times->back().pop[it] = GW::GetInstanceTime();
}

void April::Module::UwTimer::Update( AgentUpdateAllegiance const& packet )
{
	auto const* living = GW::GetAgentLivingByID( packet.agent_id );
	if ( living == nullptr ) return;
	if ( living->player_number != GW::Constants::ModelID::UW::Dhuum ) return;
	if ( living->hp < 1 ) return; // TODO: find content for turning hostile

	times->back().dhuum_hostile = GW::GetInstanceTime();
}

void April::Module::UwTimer::Update( MapLoaded const& )
{
	if ( times->size() == 0 && GW::GetMapID() == GW::MapID::The_Underworld )
		times->push_back( { system_clock::now() } );

	if ( prev_map_id == GW::MapID::The_Underworld )
		times->push_back( { system_clock::now() } );

	prev_map_id = GW::GetMapID();
}

void April::Module::UwTimer::Update( ObjectiveAdd const& packet )
{
	if ( packet.objective_id == Dhuum )
		times->back().nightman_cometh = GW::GetInstanceTime();
}

void April::Module::UwTimer::Update( ObjectiveDone const& packet )
{
	if ( packet.objective_id >= Chamber && packet.objective_id <= Pools )
	{
		auto const it = obj_to_idx( packet.objective_id );
		times->back().done[it] = GW::GetInstanceTime();
	}
	else if ( packet.objective_id == Dhuum )
	{
		times->back().dhuum_done = GW::GetInstanceTime();
	}
}

void April::Module::UwTimer::Update( ObjectiveUpdateName const& packet )
{
	if ( packet.objective_id >= Chamber && packet.objective_id <= Pools )
	{
		auto const it = obj_to_idx( packet.objective_id );
		times->back().take[it] = GW::GetInstanceTime();
	}
}
