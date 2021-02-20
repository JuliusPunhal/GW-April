
#include "April/Gui/DhuumBotGui.h"

#include "April/Framework/WndProc.h"

#include "Dependencies/ImGui.hpp"


April::Gui::DhuumBotGui::DhuumBotGui(
	std::shared_ptr<DhuumBot> bot, Config const& config )
	:
	bot{ std::move( bot ) }, config{ config }
{
}

void April::Gui::DhuumBotGui::Display() const
{
	if ( ImGui::Begin( config.window ) )
	{
		if ( ImGui::IsWindowHovered() )
		{
			WndProc::BlockMouseInput();
		}

		auto checked = bot->is_active();
		if ( ImGui::Checkbox( "Activate Dhuum Bot", &checked ) )
		{
			bot->activate( checked );
		}
	}
	ImGui::End();
}

auto April::Gui::DhuumBotGui::Config::LoadDefault() -> Config
{
	return Config{
		{ "Dhuum Bot", false, ImGuiWindowFlags_None }
	};
}
