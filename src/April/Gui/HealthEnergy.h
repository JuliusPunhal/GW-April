#pragma once

#include "April/Utility/FontAtlas.h"

#include "GWCA/GWCA.hpp"

#include "ImGui/ImGui.hpp"

#include <memory>


namespace April::Gui {

	class Healthbar {
	public:
		struct Config {
			struct Colors {
				ImVec4 standard;
				ImVec4 degen_hexed;
				ImVec4 deep_wounded;
				ImVec4 bleeding;
				ImVec4 poisoned;
			};
			struct PipsAlignment {
				ImVec2 left;
				ImVec2 right;
			};

			ImVec2 size = { 402, 22 };
			ImVec4 border_color = White();
			ImVec4 background_color = { .1f, .1f, .1f, 1 };
			Colors colors = {
				{ .70f, .00f, .00f, 1 },
				{ .55f, .00f, .55f, 1 },
				{ .50f, .50f, .50f, 1 },
				{ .77f, .57f, .38f, 1 },
				{ .50f, .50f, .00f, 1 }
			};
			PipsAlignment alignment = { { 0.1f, 0.5f }, { 0.9f, 0.5f } };

			Font   font = FontInfo{ "ABeeZee-Regular.ttf", 17 };
			Window window {
				"Healthbar",
				false,
				ImGuiWindowFlags_NoTitleBar
				| ImGuiWindowFlags_NoResize
				| ImGuiWindowFlags_NoMove
				| ImGuiWindowFlags_NoScrollbar
				| ImGuiWindowFlags_NoScrollWithMouse
				| ImGuiWindowFlags_NoCollapse
				| ImGuiWindowFlags_AlwaysAutoResize
				| ImGuiWindowFlags_NoBackground
				| ImGuiWindowFlags_NoFocusOnAppearing
				| ImGuiWindowFlags_NoBringToFrontOnFocus
			};
		};


		Healthbar(
			std::shared_ptr<Config>,
			std::shared_ptr<FontAtlas const> );

		void Display();


	private:
		std::shared_ptr<Config>          config;
		std::shared_ptr<FontAtlas const> fonts;
	};

}


namespace April::Gui {

	class Energybar {
	public:
		struct Config {
			struct PipsAlignment {
				ImVec2 left;
				ImVec2 right;
			};

			ImVec2           size = { 402, 22 };
			ImVec4           border_color = White();
			ImVec4           background_color = { .1f, .1f, .1f, 1 };
			ImVec4           color = { 0, 0, .7f, 1 };
			ImVec4           color_overcast = { .5f, .5f, .5f, 1 };
			PipsAlignment    alignment = { { 0.1f, 0.5f }, { 0.9f, 0.5f } };

			Font   font = FontInfo{ "ABeeZee-Regular.ttf", 17 };
			Window window = {
				"Energybar",
				false,
				ImGuiWindowFlags_NoTitleBar
				| ImGuiWindowFlags_NoResize
				| ImGuiWindowFlags_NoMove
				| ImGuiWindowFlags_NoScrollbar
				| ImGuiWindowFlags_NoScrollWithMouse
				| ImGuiWindowFlags_NoCollapse
				| ImGuiWindowFlags_AlwaysAutoResize
				| ImGuiWindowFlags_NoBackground
				| ImGuiWindowFlags_NoFocusOnAppearing
				| ImGuiWindowFlags_NoBringToFrontOnFocus
			};
		};


		Energybar(
			std::shared_ptr<Config>,
			std::shared_ptr<FontAtlas const> );

		void Display();


	private:
		std::shared_ptr<Config>          config;
		std::shared_ptr<FontAtlas const> fonts;
	};

}
