#pragma once

#include "April/Module/AgentFilter.h"
#include "April/Module/ChatCommands.h"
#include "April/Module/ChatFilter.h"
#include "April/Module/ConsumablesMgr.h"
#include "April/Module/NotifyEffectLoss.h"
#include "April/Module/ReturnToOutpost.h"
#include "April/Module/ShowKitUses.h"
#include "April/Module/SuppressSpeechBubbles.h"

#include "April/Gui/ChainedSoulInfo.h"
#include "April/Gui/DhuumBotGui.h"
#include "April/Gui/DhuumInfo.h"
#include "April/Gui/Dialogs.h"
#include "April/Gui/HealthEnergy.h"
#include "April/Gui/InstanceTimer.h"
#include "April/Gui/Inventory.h"
#include "April/Gui/Settings.h"
#include "April/Gui/Skillbar.h"
#include "April/Gui/TargetInfo.h"
#include "April/Gui/UwTimes.h"

#include <fstream>
#include <optional>
#include <string>
#include <string_view>


namespace April::IO {

	auto to_string( AgentFilter::Config const& ) -> std::string;
	auto to_string( ChatCommands::Config const& ) -> std::string;
	auto to_string( ChatFilter::Config const& ) -> std::string;
	auto to_string( ConsumablesMgr::Config const& ) -> std::string;
	auto to_string( NotifyEffectLoss::Config const& ) -> std::string;
	auto to_string( ReturnToOutpost::Config const& ) -> std::string;
	auto to_string( ShowKitUses::Config const& ) -> std::string;
	auto to_string( SuppressSpeechBubbles::Config const& ) -> std::string;

	auto to_string( Gui::ChainedSoulGui::Config const& ) -> std::string;
	auto to_string( Gui::DhuumBotGui::Config const& ) -> std::string;
	auto to_string( Gui::DhuumInfo::Config const& ) -> std::string;
	auto to_string( Gui::Dialogs::Config const& ) -> std::string;
	auto to_string( Gui::Energybar::Config const& ) -> std::string;
	auto to_string( Gui::Healthbar::Config const& ) -> std::string;
	auto to_string( Gui::InstanceTimer::Config const& ) -> std::string;
	auto to_string( Gui::Inventory::Config const& ) -> std::string;
	auto to_string( Gui::Settings::Config const& ) -> std::string;
	auto to_string( Gui::Skillbar::Config const& ) -> std::string;
	auto to_string( Gui::TargetInfo::Config const& ) -> std::string;
	auto to_string( Gui::UwTimesGui::Config const& ) -> std::string;

	template<typename T>
	auto to_file( T&& config, const char* path )
	{
		if ( auto os = std::ofstream{ path }; os )
		{
			os << to_string( std::forward<T>( config ) );
		}
	}


	template<typename T>
	auto from_string( std::string_view ) -> std::optional<T>;


	template<> auto from_string( std::string_view )
		-> std::optional<AgentFilter::Config>;

	template<> auto from_string( std::string_view )
		-> std::optional<ChatCommands::Config>;

	template<> auto from_string( std::string_view )
		-> std::optional<ChatFilter::Config>;

	template<> auto from_string( std::string_view )
		-> std::optional<ConsumablesMgr::Config>;

	template<> auto from_string( std::string_view )
		-> std::optional<NotifyEffectLoss::Config>;

	template<> auto from_string( std::string_view )
		-> std::optional<ReturnToOutpost::Config>;

	template<> auto from_string( std::string_view )
		-> std::optional<ShowKitUses::Config>;

	template<> auto from_string( std::string_view )
		-> std::optional<SuppressSpeechBubbles::Config>;


	template<> auto from_string( std::string_view )
		-> std::optional<Gui::ChainedSoulGui::Config>;

	template<> auto from_string( std::string_view )
		-> std::optional<Gui::DhuumBotGui::Config>;

	template<> auto from_string( std::string_view ) 
		-> std::optional<Gui::DhuumInfo::Config>;

	template<> auto from_string( std::string_view )
		-> std::optional<Gui::Dialogs::Config>;

	template<> auto from_string( std::string_view )
		-> std::optional<Gui::Healthbar::Config>;

	template<> auto from_string( std::string_view )
		-> std::optional<Gui::Energybar::Config>;

	template<> auto from_string( std::string_view )
		-> std::optional<Gui::InstanceTimer::Config>;

	template<> auto from_string( std::string_view )
		-> std::optional<Gui::Inventory::Config>;

	template<> auto from_string( std::string_view )
		-> std::optional<Gui::Settings::Config>;

	template<> auto from_string( std::string_view )
		-> std::optional<Gui::Skillbar::Config>;

	template<> auto from_string( std::string_view )
		-> std::optional<Gui::TargetInfo::Config>;

	template<> auto from_string( std::string_view )
		-> std::optional<Gui::UwTimesGui::Config>;


	auto read_file( const char* path ) -> std::string;

	template<typename T>
	auto from_file( const char* path ) -> std::optional<T>
	{
		return from_string<T>( read_file( path ) );
	}

}
