
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
			return "Dhuum's Rest: invalid";
		}

		char buf[32] = {};
		snprintf( buf, sizeof( buf ), "Dhuum's Rest: %.2f%%", percent );
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


April::Gui::DhuumInfo::DhuumInfo( 
	std::shared_ptr<DhuumsJudgement const> judgement, Config const& config )
	: 
	judgement{ std::move( judgement ) }, config{ config }
{
}

void April::Gui::DhuumInfo::Display() const
{
	auto const rest = GW::GetMissionProgress() * 100;
	auto const label_rest = rest_to_string( rest );
	auto const label_judgement = judgement_to_string( *judgement );

	ImGui::Begin( config.window_name, config.window_flags );
	{
		if ( ImGui::IsWindowHovered() )
		{
			WndProc::BlockMouseInput();
		}

		if ( ImGui::Button( label_rest ) )
		{
			GW::SendChat( '#', label_rest );
		}

		if ( ImGui::Button( label_judgement ) )
		{
			GW::SendChat( '#', label_judgement );
		}
	}
	ImGui::End();
}

auto April::Gui::DhuumInfo::Config::LoadDefault() -> Config
{
	auto const config = Config{
		"Dhuum Info",
		ImGuiWindowFlags_None
	};

	return config;
}
