#pragma once

#include "April/Gui/Settings.Config.h"

// Module Settings Widgets
#include "April/Gui/Settings/Settings.ChatCommands.h"
#include "April/Gui/Settings/Settings.ChatFilter.h"
#include "April/Gui/Settings/Settings.DhuumBot.h"
#include "April/Gui/Settings/Settings.GuildEditor.h"
#include "April/Gui/Settings/Settings.ItemFilter.h"
#include "April/Gui/Settings/Settings.NotifyOnEffect.h"
#include "April/Gui/Settings/Settings.ShowKitUses.h"

// Gui Settings Widgets
#include "April/Gui/Settings/Settings.HealthEnergy.h"
#include "April/Gui/Settings/Settings.InstanceTimer.h"
#include "April/Gui/Settings/Settings.Inventory.h"
#include "April/Gui/Settings/Settings.Settings.h"
#include "April/Gui/Settings/Settings.Skillbar.h"
#include "April/Gui/Settings/Settings.DefaultTheme.h"

// Info Widgets
#include "April/Gui/Settings/Settings.Dialogs.h"
#include "April/Gui/Settings/Settings.ResignLog.h"

// Generic Settings Widgets
#include "April/Gui/Settings/Settings.Generic.h"
#include "April/Gui/ChainedSouls.h"
#include "April/Gui/DhuumInfo.h"
#include "April/Gui/UwTimer.h"
#include "April/Module/ResignMessage.h"
#include "April/Module/ReturnToOutpost.h"
#include "April/Module/SuppressSpeechBubbles.h"

#include "April/Utility/FontAtlas.h"
#include "April/Utility/Mouse.h"

#include "GWCA/GWCA.hpp"

#include <memory>
#include <string>
#include <tuple>
#include <vector>


namespace April::Gui {

	class Settings {
	public:
		using Config = Settings_Config;

		using ModuleSettings =
			std::tuple<
				Settings_ChatCommands,
				Settings_ChatFilter,
				Settings_GuildEditor,
				Settings_ItemFilter,
				Settings_NotifyOnEffect,
				Settings_Generic<Module::ResignMessage::Config>,
				Settings_Generic<Module::ReturnToOutpost::Config>,
				Settings_ShowKitUses,
				Settings_Generic<
					Module::SuppressSpeechBubbles::Config>>;

		using GuiSettings =
			std::tuple<
				Settings_Generic<Gui::ChainedSouls::Config>,
				Settings_Generic<Gui::DhuumInfo::Config>,
				Settings_Energybar,
				Settings_Healthbar,
				Settings_InstanceTimer,
				Settings_Inventory,
				Settings_Settings,
				Settings_Skillbar,
				Settings_Generic<Gui::UwTimer::Config>,
				Settings_DefaultTheme>;

		using InfoWidgets =
			std::tuple<
				Settings_DhuumBot,
				Settings_Dialogs,
				Settings_ResignLog>;

		struct Sections {
			ModuleSettings modules;
			GuiSettings    guis;
			InfoWidgets    info;
		};

		using SectionsCombined =
			decltype(
				std::tuple_cat(
					std::declval<ModuleSettings>(),
					std::declval<GuiSettings>(),
					std::declval<InfoWidgets>() ) );

		using Configurations =
			std::tuple<
				std::shared_ptr<Module::ChatCommands::Config>,
				std::shared_ptr<Module::ChatFilter::Config>,
				std::shared_ptr<Module::GuildEditor::Config>,
				std::shared_ptr<Module::ItemFilter::Config>,
				std::shared_ptr<Module::NotifyOnEffect::Config>,
				std::shared_ptr<Module::ResignMessage::Config>,
				std::shared_ptr<Module::ReturnToOutpost::Config>,
				std::shared_ptr<Module::ShowKitUses::Config>,
				std::shared_ptr<Module::SuppressSpeechBubbles::Config>,
				std::shared_ptr<Gui::ChainedSouls::Config>,
				std::shared_ptr<Gui::DhuumInfo::Config>,
				std::shared_ptr<Gui::Energybar::Config>,
				std::shared_ptr<Gui::Healthbar::Config>,
				std::shared_ptr<Gui::InstanceTimer::Config>,
				std::shared_ptr<Gui::Inventory::Config>,
				std::shared_ptr<Gui::Settings::Config>,
				std::shared_ptr<Gui::Skillbar::Config>,
				std::shared_ptr<Gui::UwTimer::Config>,
				std::shared_ptr<DefaultTheme::Config>,
				std::shared_ptr<Exit::Config>,
				std::shared_ptr<Gui::Settings_Dialogs::Config>>;

		using Features =
			std::tuple<
				std::shared_ptr<FontAtlas>,
				std::shared_ptr<Module::DhuumBot>,
				std::shared_ptr<Module::GuildEditor>,
				std::shared_ptr<Module::ItemFilter>,
				std::shared_ptr<Module::ShowKitUses>>;


		Settings(
			std::shared_ptr<Config>,
			std::shared_ptr<FontAtlas const>,
			std::shared_ptr<Mouse>,
			Configurations const&,
			Features const& );

		void Display();


	private:
		std::shared_ptr<Config>          config;
		std::shared_ptr<FontAtlas const> fonts;
		std::shared_ptr<Mouse>           mouse;
		Sections                         sections;
		SectionsCombined                 drawables;
		Configurations                   configs;

		int         selected_section = 0;
		std::string selected_feature = "";
	};

}
