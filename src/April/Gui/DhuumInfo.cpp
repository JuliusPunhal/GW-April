
#include "April/Gui/DhuumInfo.h"

#include "April/Utility/TimeFormatting.h"

#include "GWCA/GWCA.hpp"

using namespace April;
using namespace GW::Packet::StoC;


namespace {

	auto dhuums_rest_str() -> std::string
	{
		auto const percent = GW::GetMissionProgress() * 100;
		if ( percent < 0 || percent > 100 )
			return "0.00%";

		char buf[32] = {};
		snprintf( buf, sizeof( buf ), "%.2f%%", percent );
		return buf;
	}

	auto last_judgement_string( DhuumsJudgement const& judgement )
		-> std::string
	{
		if ( judgement.Get() == std::nullopt )
			return "Time since last Judgement: --:--";

		auto const time_since_last = GW::GetInstanceTime() - *judgement.Get();
		auto const str = to_string_mmss( time_since_last );
		return "Time since last Judgement: " + str;
	}

}


April::Gui::DhuumInfo::DhuumInfo(
	std::shared_ptr<Config>                config,
	std::shared_ptr<DhuumsJudgement const> judgement,
	std::shared_ptr<Mouse>                 mouse )
	: config{ config }, judgement{ judgement }, mouse{ mouse }
{
}

void April::Gui::DhuumInfo::Display()
{
	ImGui::SetNextWindowPosCentered( ImGuiCond_FirstUseEver );
	ImGui::SetNextWindowSize( { 300.f, 0 }, ImGuiCond_FirstUseEver );

	if ( ImGui::Begin( config->window, *mouse ) )
	{
		ImGui::Text( "Dhuum's Rest" );

		auto const cursor = ImGui::GetCursorPos();
		ImGui::ProgressBar( GW::GetMissionProgress(), { -1, 0 }, "" );

		auto const rest_str = dhuums_rest_str();
		ImGui::SetCursorPos( cursor );
		ImGui::PushStyleColor( ImGuiCol_Button, Invisible() );
		if ( ImGui::Button( rest_str, { -1, 0 } ) )
		{
			GW::SendChat( '#', "Dhuum's Rest: " + rest_str );
		}
		ImGui::PopStyleColor();

		auto const judgement_str = last_judgement_string( *judgement );
		if ( ImGui::Button( judgement_str, { -1, 0 } ) )
		{
			GW::SendChat( '#', judgement_str );
		}
	}
	ImGui::End();
}

void April::Gui::DhuumInfo::Update( AgentUpdateAllegiance const& packet )
{
	if ( not config->auto_set_visibility )
		return;

	if (
		auto const* living = GW::GetAgentLivingByID( packet.agent_id );
		living
		&& living->player_number == GW::Constants::ModelID::UW::Dhuum
		&& living->hp == 1 ) // TODO: find content for turning hostile
	{
		config->window.visible = true;
	}
}

void April::Gui::DhuumInfo::Update( ObjectiveDone const& packet )
{
	if ( not config->auto_set_visibility )
		return;

	if ( packet.objective_id == GW::Constants::ObjectiveID::Dhuum )
		config->window.visible = false;
}

void April::Gui::DhuumInfo::Update( MapLoaded const& )
{
	if ( config->auto_set_visibility )
		config->window.visible = false;
}
