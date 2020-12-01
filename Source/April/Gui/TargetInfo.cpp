
#include "April/Gui/TargetInfo.h"

#include "April/Config/Gui/TargetInfo.config.hpp"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"

namespace config = April::Gui::TargetInfoConfig;

	
void April::Gui::TargetInfo::Display() const
{
	auto const* target = GW::Agents::GetTarget();
	if ( target == nullptr )
	{
		ImGui::Begin( config::window_name, config::window_flags );
		ImGui::Text( "No target found!" );
		ImGui::End();
		return;
	}
	
	auto const* player = GW::Agents::GetPlayer();
	ImGui::Begin( config::window_name, config::window_flags );
	{
		auto const x = static_cast<int>( target->pos.x );
		auto const y = static_cast<int>( target->pos.y );
		auto const distance = 
			player ? GW::GetDistance( target->pos, player->pos ) : 0;
		auto const temp = std::sqrt( 
			target->move_x * target->move_x + target->move_y * target->move_y );
		auto const velocity = temp / 288.f * 100;

		ImGui::Text( "AgentID: %d", target->agent_id );
		ImGui::Text( "Position: %d, %d", x, y );
		ImGui::Text( "Distance: %.0f", distance );
		ImGui::Text( "Velocity: %.0f%%", velocity );

		if ( auto const* living = target->GetAsAgentLiving(); living )
		{
			ImGui::Text( "ModelID: %d", living->player_number );
			ImGui::Text( "LoginNumber: %d", living->login_number );
			ImGui::Text( 
				"Health: %d/%d (%.0f%%)", 
				GW::GetHealthPoints( *living ), 
				living->max_hp, 
				living->hp * 100 );
			ImGui::Text(
				"Energy: %d/%d (%.0f%%)", 
				GW::GetEnergyPoints( *living ), 
				living->max_energy, 
				living->energy * 100 );
			ImGui::Text( "Effects: 0x%X", living->effects );
		}
	}
	ImGui::End();
}
