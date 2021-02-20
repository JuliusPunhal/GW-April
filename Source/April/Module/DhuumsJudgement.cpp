
#include "April/Module/DhuumsJudgement.h"

#include "Dependencies/GWCA.hpp"

using namespace GW::Packet::StoC;
using namespace std::chrono;


namespace {

	auto get_dhuum() -> GW::AgentLiving const*
	{
		for ( auto const* agent : GW::Agents::GetAgentArray() )
		{
			if ( agent == nullptr ) continue;

			auto const* living = agent->GetAsAgentLiving();
			if ( living == nullptr ) continue;

			if ( living->player_number == GW::Constants::ModelID::UW::Dhuum )
				return living;
		}
		return nullptr;
	}

}


void April::DhuumsJudgement::Update()
{
	auto const* p_dhuum = get_dhuum();
	if ( p_dhuum == nullptr ) return;

	auto const skill = static_cast<GW::SkillID>(p_dhuum->skill);
	if ( skill == GW::SkillID::Judgment_of_Dhuum )
	{
		last = GW::GetInstanceTime();
	}
}

void April::DhuumsJudgement::Update( MapLoaded const& )
{
	last = -1ms;
}
