#pragma once

#include "April/Utility/FontAtlas.h"

#include "ImGui/ImGui.hpp"

#include <memory>


namespace April {

	class DefaultTheme {
	public:
		struct Config {
			Font       font = FontInfo{ "ABeeZee-Regular.ttf", 16 };
			bool       move_from_titlebar = true;
			ImGuiStyle style = default_style();

		private:
			static auto default_style() -> ImGuiStyle;
		};


		DefaultTheme(
			std::shared_ptr<Config const>, std::shared_ptr<FontAtlas const> );

		void ApplyThemeToImGui();


	private:
		std::shared_ptr<Config const>    config;
		std::shared_ptr<FontAtlas const> fonts;
	};

}
