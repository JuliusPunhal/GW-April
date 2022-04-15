#pragma once

#include "April/Gui/HealthEnergy.h"
#include "April/Gui/InstanceTimer.h"
#include "April/Gui/Inventory.h"
#include "April/Gui/Skillbar.h"
#include "April/Gui/UwTimer.h"
#include "April/Module/ConsumablesMgr.h"
#include "April/Module/ItemFilter.h"

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
				std::shared_ptr<Gui::Energybar::Config>,
				std::shared_ptr<Gui::Healthbar::Config>,
				std::shared_ptr<Gui::InstanceTimer::Config>,
				std::shared_ptr<Gui::Inventory::Config>,
				std::shared_ptr<Gui::Skillbar::Config>,
				std::shared_ptr<Gui::UwTimer::Config>>;


		ChatCommands(
			std::shared_ptr<Config const>,
			std::shared_ptr<ConsumablesMgr>,
			std::shared_ptr<ItemFilter>,
			GuiConfigs );

		void Update( GW::HookStatus&, GW::SendChatInfo );


	private:
		std::shared_ptr<Config const>   config;
		std::shared_ptr<ConsumablesMgr> consumables_mgr;
		std::shared_ptr<ItemFilter>     item_filter;
		GuiConfigs                      gui_configs;
	};


	auto make_Abbreviation( std::string const&, std::string const& )
		-> std::optional<ChatCommands::Abbreviation>;

}
