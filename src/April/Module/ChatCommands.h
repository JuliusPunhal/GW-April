#pragma once

#include "April/Gui/ChainedSouls.h"
#include "April/Gui/DhuumInfo.h"
#include "April/Gui/HealthEnergy.h"
#include "April/Gui/InstanceTimer.h"
#include "April/Gui/Inventory.h"
#include "April/Gui/Settings.Config.h"
#include "April/Gui/Skillbar.h"
#include "April/Gui/UwTimer.h"
#include "April/Module/ConsumablesMgr.h"
#include "April/Module/ItemFilter.h"
#include "April/Utility/Exit.h"

#include "GWCA/GWCA.hpp"

#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>


namespace April::Module {

	class ChatCommands {
	public:
		class Abbreviation {
			Abbreviation( std::string const&, std::string const& );

			friend auto make_Abbreviation(
				std::string const&, std::string const& )
				-> std::optional<Abbreviation>;


		public:
			auto abbreviated() const -> auto const& { return abbreviated_; }

			auto expanded() -> auto& { return expanded_; }
			auto expanded() const -> auto const& { return expanded_; }


			bool operator==( std::string const& ) const;


		private:
			std::string abbreviated_;
			std::string expanded_;
		};
		using Abbreviations = std::vector<Abbreviation>;

		struct Config {
			Abbreviations abbreviations = default_Abbreviations();

		private:
			static auto default_Abbreviations() -> Abbreviations;
		};

		using GuiConfigs =
			std::tuple<
				std::shared_ptr<Gui::ChainedSouls::Config>,
				std::shared_ptr<Gui::DhuumInfo::Config>,
				std::shared_ptr<Gui::Energybar::Config>,
				std::shared_ptr<Gui::Healthbar::Config>,
				std::shared_ptr<Gui::InstanceTimer::Config>,
				std::shared_ptr<Gui::Inventory::Config>,
				std::shared_ptr<Gui::Settings_Config>,
				std::shared_ptr<Gui::Skillbar::Config>,
				std::shared_ptr<Gui::UwTimer::Config>>;


		static constexpr auto cmd_exit =             "/april_exit";
		static constexpr auto cmd_openxunlai =       "/april_open_xunlai";
		static constexpr auto cmd_pcons_perm_on =    "/april_pcons_perm_on";
		static constexpr auto cmd_pcons_perm_off =   "/april_pcons_perm_off";
		static constexpr auto cmd_pcons_temp_on =    "/april_pcons_temp_on";
		static constexpr auto cmd_pcons_temp_off =   "/april_pcons_temp_off";
		static constexpr auto cmd_pcons_objective =  "/april_pcons_objective";
		static constexpr auto cmd_reset_dx9 =        "/april_reset_dx9";
		static constexpr auto cmd_settings_panel =   "/april_settings_panel";
		static constexpr auto cmd_toggle_window =    "/april_toggle_window";
		static constexpr auto cmd_suppressed_items =
			"/april_show_suppressed_items";


		ChatCommands(
			std::shared_ptr<Config const>,
			std::shared_ptr<ConsumablesMgr>,
			std::shared_ptr<ItemFilter>,
			std::shared_ptr<Exit>,
			GuiConfigs );

		void Update( GW::HookStatus&, GW::SendChatInfo );


	private:
		std::shared_ptr<Config const>   config;
		std::shared_ptr<ConsumablesMgr> consumables_mgr;
		std::shared_ptr<ItemFilter>     item_filter;
		std::shared_ptr<Exit>           exit;
		GuiConfigs                      gui_configs;
	};


	auto make_Abbreviation( std::string const&, std::string const& )
		-> std::optional<ChatCommands::Abbreviation>;

}
