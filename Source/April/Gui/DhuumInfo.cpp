
#include "April/Gui/DhuumInfo.h"

#include "April/Framework/WndProc.h"
#include "April/Utility/TimeFormatting.h"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"

#include <chrono>
#include <string>
#include <tuple>

using namespace std::chrono;


namespace {

	inline constexpr auto fmt = std::tuple{
		April::TimeFormat<std::chrono::minutes>{ "%1d:" },
		April::TimeFormat<std::chrono::seconds>{ "%02d" }
	};


	auto rest_to_string( float const percent ) -> std::string
	{
		if ( percent < 0 or percent > 100 )
		{
			return "";
		}

		char buf[32] = {};
		snprintf( buf, sizeof( buf ), "%.2f%%", percent );
		return buf;
	}

	auto judgement_to_string( April::DhuumsJudgement const& judgement )
		-> std::string
	{
		auto const last = judgement.last_judgement();
		if ( last >= 0ms )
		{
			auto const time_since_last = GW::GetInstanceTime() - last;
			auto const str = to_string( time_since_last, fmt );

			return "Time since last Judgement: " + str;
		}
		else return "Time since last Judgement: --:--";
	}

}


void April::Gui::DhuumInfo::Display(
	DhuumsJudgement const& judgement, Config const& config ) const
{
	if ( ImGui::Begin( config.window ) )
	{
		if ( ImGui::IsWindowHovered() )
		{
			WndProc::BlockMouseInput();
		}

		ImGui::Text( "Dhuum's Rest" );

		auto const rest = GW::GetMissionProgress();
		auto const label_rest = rest_to_string( rest * 100 );
		auto const label_judgement = judgement_to_string( judgement );

		auto const cursor = ImGui::GetCursorPos();
		ImGui::ProgressBar( rest, { -1, 0 }, "" );

		ImGui::SetCursorPos( cursor );
		ImGui::PushStyleColor( ImGuiCol_Button, Invisible() );
		if ( ImGui::Button( label_rest, { -1, 0 } ) )
		{
			GW::SendChat( '#', "Dhuum's Rest: " + label_rest );
		}
		ImGui::PopStyleColor();

		if ( ImGui::Button( label_judgement, { -1, 0 } ) )
		{
			GW::SendChat( '#', label_judgement );
		}
	}
	ImGui::End();
}

auto April::Gui::DhuumInfo::Config::LoadDefault() -> Config
{
	return Config{
		{ "Dhuum Info", false, ImGuiWindowFlags_None }
	};
}
