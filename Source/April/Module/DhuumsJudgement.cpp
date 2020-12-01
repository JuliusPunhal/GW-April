
#include "April/Module/DhuumsJudgement.h"

#include "Dependencies/GWCA.hpp"

using namespace GW::Packet::StoC;
using namespace std::chrono;


namespace {

	auto entry = GW::HookEntry{};


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

	void update( SkillActivate const& packet, milliseconds& last_judgement )
	{
		auto const skill_id = static_cast<GW::SkillID>( packet.skill_id );
		if ( skill_id == GW::SkillID::Judgment_of_Dhuum )
		{
			last_judgement = GW::GetInstanceTime();
		}
	}

}


April::DhuumsJudgement::DhuumsJudgement()
{
	// Callbacks will only be cleaned up during GWCA shutdown.
	GW::StoC::RegisterPacketCallback<SkillActivate>(
		&entry, [this]( auto*, auto* packet ) { update( *packet, last); } );

	GW::StoC::RegisterPacketCallback<MapLoaded>( 
		&entry, [this]( auto*, auto* ) { last = -1ms; } );
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
