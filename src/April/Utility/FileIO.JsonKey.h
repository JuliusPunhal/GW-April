#pragma once

#include "April/Gui/ChainedSouls.h"
#include "April/Gui/DhuumInfo.h"
#include "April/Gui/HealthEnergy.h"
#include "April/Gui/InstanceTimer.h"
#include "April/Gui/Inventory.h"
#include "April/Gui/Settings.h"
#include "April/Gui/Settings/Settings.Dialogs.h"
#include "April/Gui/Skillbar.h"
#include "April/Gui/UwTimer.h"
#include "April/Module/ChatCommands.h"
#include "April/Module/ChatFilter.h"
#include "April/Module/GuildEditor.h"
#include "April/Module/ItemFilter.h"
#include "April/Module/NotifyOnEffect.h"
#include "April/Module/ResignMessage.h"
#include "April/Module/ReturnToOutpost.h"
#include "April/Module/ShowKitUses.h"
#include "April/Module/SuppressSpeechBubbles.h"
#include "April/Utility/DefaultTheme.h"


namespace April {

	template<typename T>
	struct JsonKey{};


	template<>
	struct JsonKey<DefaultTheme::Config> {
		static constexpr auto key = "Gui/DefaultTheme";
	};

	template<>
	struct JsonKey<Gui::ChainedSouls::Config> {
		static constexpr auto key = "Gui/ChainedSouls";
	};

	template<>
	struct JsonKey<Gui::DhuumInfo::Config> {
		static constexpr auto key = "Gui/DhuumInfo";
	};

	template<>
	struct JsonKey<Gui::Energybar::Config> {
		static constexpr auto key = "Gui/Energybar";
	};

	template<>
	struct JsonKey<Gui::Healthbar::Config> {
		static constexpr auto key = "Gui/Healthbar";
	};

	template<>
	struct JsonKey<Gui::InstanceTimer::Config> {
		static constexpr auto key = "Gui/InstanceTimer";
	};

	template<>
	struct JsonKey<Gui::Inventory::Config> {
		static constexpr auto key = "Gui/Inventory";
	};

	template<>
	struct JsonKey<Gui::UwTimer::Config> {
		static constexpr auto key = "Gui/UwTimer";
	};

	template<>
	struct JsonKey<Gui::Settings::Config> {
		static constexpr auto key = "Gui/Settings";
	};

	template<>
	struct JsonKey<Gui::Settings_Dialogs::Config> {
		static constexpr auto key = "Gui/Settings/Dialogs";
	};

	template<>
	struct JsonKey<Gui::Skillbar::Config> {
		static constexpr auto key = "Gui/Skillbar";
	};

	template<>
	struct JsonKey<Module::ChatCommands::Config> {
		static constexpr auto key = "Module/ChatCommands";
	};

	template<>
	struct JsonKey<Module::ChatFilter::Config> {
		static constexpr auto key = "Module/ChatFilter";
	};

	template<>
	struct JsonKey<Module::GuildEditor::Config> {
		static constexpr auto key = "Module/GuildEditor";
	};

	template<>
	struct JsonKey<Module::ItemFilter::Config> {
		static constexpr auto key = "Module/ItemFilter";
	};

	template<>
	struct JsonKey<Module::NotifyOnEffect::Config> {
		static constexpr auto key = "Module/NotifyOnEffect";
	};

	template<>
	struct JsonKey<Module::ResignMessage::Config> {
		static constexpr auto key = "Module/ResignMessage";
	};

	template<>
	struct JsonKey<Module::ReturnToOutpost::Config> {
		static constexpr auto key = "Module/ReturnToOutpost";
	};

	template<>
	struct JsonKey<Module::ShowKitUses::Config> {
		static constexpr auto key = "Module/ShowKitUses";
	};

	template<>
	struct JsonKey<Module::SuppressSpeechBubbles::Config> {
		static constexpr auto key = "Module/SuppressSpeechBubbles";
	};

}
