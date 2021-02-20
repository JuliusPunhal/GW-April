
#include "April/Module/ChainedSoul.h"

#include "Dependencies/GWCA.hpp"

using namespace std::chrono;


namespace {

	auto find_chained_soul() -> GW::AgentLiving const*
	{
		using namespace GW::Constants::ModelID::UW;

		for ( auto const* agent : GW::Agents::GetAgentArray() )
		{
			if ( agent == nullptr ) continue;

			auto const* const living = agent->GetAsAgentLiving();
			if ( living == nullptr ) continue;

			if ( living->player_number == ChainedSoul
				&& living->pos == GW::GamePos{ 11427, 5079, 0 } )
			{
				return living;
			}
		}

		return nullptr;
	}

}


void April::ChainedSoul::Update()
{
	if ( GW::Map::GetMapID() != GW::MapID::The_Underworld )
	{
		state = OutOfRange{};
		return;
	}

	auto const* chained_soul = find_chained_soul();
	if ( chained_soul == nullptr )
	{
		state = OutOfRange{ time_of_death() };
		return;
	}

	if ( chained_soul->GetIsDead() )
	{
		if ( std::holds_alternative<AliveSoul>( state ) )
			state = DeadSoul{ GW::GetInstanceTime() };
		else
			state = DeadSoul{ time_of_death() };

		return;
	}

	state = AliveSoul{ chained_soul->hp, chained_soul->hp_pips };
}
