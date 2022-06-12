#pragma once

#include "April/Module/ChatCommands.h"

#include <memory>
#include <optional>
#include <string>


namespace April::Gui {

	class Settings_ChatCommands {
	public:
		using Abbreviation = Module::ChatCommands::Abbreviation;
		using Abbreviations = Module::ChatCommands::Abbreviations;


		Settings_ChatCommands( std::shared_ptr<Module::ChatCommands::Config> );

		void Draw();


	private:
		std::shared_ptr<Module::ChatCommands::Config> config;

		std::string new_abbreviated = {};
		std::string new_expanded = {};

		std::optional<Abbreviation> new_abbreviation = {};
		Abbreviations::iterator     conflict = {};
	};

}
