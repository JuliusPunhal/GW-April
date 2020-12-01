
#include "April/Gui/DhuumBotGui.h"

#include "April/Config/Gui/DhuumBotGui.config.hpp"
#include "April/Framework/WndProc.h"

#include "Dependencies/ImGui.hpp"

namespace config = April::Gui::DhuumBotGuiConfig;


April::Gui::DhuumBotGui::DhuumBotGui( std::shared_ptr<DhuumBot> bot )
	: bot{ std::move( bot ) }
{
}

void April::Gui::DhuumBotGui::Display() const
{
	ImGui::Begin( config::window_name, config::window_flags );
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
