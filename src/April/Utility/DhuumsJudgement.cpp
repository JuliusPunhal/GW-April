
#include "April/Utility/DhuumsJudgement.h"

using namespace GW::Constants::ModelID::UW;
using namespace GW::Packet::StoC;


namespace {

	auto get_dhuum( std::optional<GW::AgentID>& cache )
		-> GW::AgentLiving const*
	{
		if ( cache.has_value() )
		{
			auto const* dhuum = GW::GetAgentLivingByID( *cache );
			if ( dhuum && dhuum->player_number == Dhuum )
				return dhuum;
		}

		for ( auto const* agent : GW::GetAgentArray() )
		{
			auto const* living = GW::GetAsAgentLiving( agent );
			if ( living && living->player_number == Dhuum )
			{
				cache = living->agent_id;
				return living;
			}
		}

		return nullptr;
	}

}


void April::DhuumsJudgement::Update()
{
	auto const* dhuum = get_dhuum( dhuum_id );
	if ( dhuum == nullptr )
		return;

	if ( GW::GetSkill( *dhuum ) == GW::SkillID::Judgment_of_Dhuum )
	{
		last_judgement = GW::GetInstanceTime();
	}
}

void April::DhuumsJudgement::Update( MapLoaded const& )
{
	last_judgement = std::nullopt;
	dhuum_id = std::nullopt;
}
