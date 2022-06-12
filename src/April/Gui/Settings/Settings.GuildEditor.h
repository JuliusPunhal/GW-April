#pragma once

#include "April/Module/GuildEditor.h"

#include <memory>


namespace April::Gui {

	class Settings_GuildEditor {
	public:
		Settings_GuildEditor(
			std::shared_ptr<Module::GuildEditor>,
			std::shared_ptr<Module::GuildEditor::Config> );

		void Draw();


	private:
		std::shared_ptr<Module::GuildEditor>         guild_editor;
		std::shared_ptr<Module::GuildEditor::Config> config;
	};

}
