#pragma once

#include "April/Gui/ChainedSouls.h"
#include "April/Gui/DhuumInfo.h"
#include "April/Gui/HealthEnergy.h"
#include "April/Gui/InstanceTimer.h"
#include "April/Gui/Inventory.h"
#include "April/Gui/Settings.h"
#include "April/Gui/Skillbar.h"
#include "April/Gui/UwTimer.h"
#include "April/Module/ChatCommands.h"
#include "April/Module/ChatFilter.h"
#include "April/Module/ConsumablesMgr.h"
#include "April/Module/DhuumBot.h"
#include "April/Module/GuildEditor.h"
#include "April/Module/ItemFilter.h"
#include "April/Module/NotifyOnEffect.h"
#include "April/Module/ResignMessage.h"
#include "April/Module/ResignReminder.h"
#include "April/Module/ReturnToOutpost.h"
#include "April/Module/ShowKitUses.h"
#include "April/Module/SuppressSpeechBubbles.h"
#include "April/Module/UwTimer.h"
#include "April/Utility/ChainedSouls.h"
#include "April/Utility/DefaultTheme.h"
#include "April/Utility/DhuumsJudgement.h"
#include "April/Utility/FontAtlas.h"
#include "April/Utility/Mouse.h"
#include "April/Utility/ReducedSkillRecharge.h"
#include "April/Utility/Exit.h"

#include <memory>
#include <tuple>


namespace April {

	using Features =
		std::tuple<
			std::unique_ptr<Gui::ChainedSouls>,
			std::unique_ptr<Gui::DhuumInfo>,
			std::unique_ptr<Gui::Energybar>,
			std::unique_ptr<Gui::Healthbar>,
			std::unique_ptr<Gui::InstanceTimer>,
			std::unique_ptr<Gui::Inventory>,
			std::unique_ptr<Gui::Settings>,
			std::unique_ptr<Gui::Skillbar>,
			std::unique_ptr<Gui::UwTimer>,
			std::unique_ptr<Module::ChatCommands>,
			std::unique_ptr<Module::ChatFilter>,
			std::shared_ptr<Module::ConsumablesMgr>,
			std::shared_ptr<Module::DhuumBot>,
			std::shared_ptr<Module::GuildEditor>,
			std::shared_ptr<Module::ItemFilter>,
			std::unique_ptr<Module::NotifyOnEffect>,
			std::unique_ptr<Module::ResignMessage>,
			std::unique_ptr<Module::ResignReminder>,
			std::unique_ptr<Module::ReturnToOutpost>,
			std::shared_ptr<Module::ShowKitUses>,
			std::unique_ptr<Module::SuppressSpeechBubbles>,
			std::unique_ptr<Module::UwTimer>,
			std::shared_ptr<ChainedSouls>,
			std::shared_ptr<DefaultTheme>,
			std::shared_ptr<DhuumsJudgement>,
			std::shared_ptr<FontAtlas>,
			std::shared_ptr<Mouse>,
			std::shared_ptr<ReducedSkillRecharge>,
			std::shared_ptr<Exit>>;


	auto make_Features() -> Features;
	void Shutdown( Features& );

	void Update( Features& );
	void Update( Features&, GW::HookStatus&, GW::LocalMessageInfo );
	void Update( Features&, GW::HookStatus&, GW::SendChatInfo );
	void Display( Features& );


	namespace detail {

		template<typename T, typename Packet_t, typename = void>
		struct has_PacketUpdate : std::false_type {};

		template<typename T, typename Packet_t>
		struct has_PacketUpdate<
			T,
			Packet_t,
			std::void_t<decltype(
				std::declval<T>().Update( std::declval<Packet_t&>() ) )>>
			: std::true_type
		{
		};


		template<typename Feature_t, typename Packet_t>
		void update( Feature_t& ft, Packet_t& packet )
		{
			using T = typename Feature_t::element_type;
			if constexpr ( has_PacketUpdate<T, Packet_t>::value )
			{
				ft->Update( packet );
			}
		}

	}


	template<typename Packet_t>
	void Update( Features& features, Packet_t& packet )
	{
		std::apply(
			[&packet]( auto&... ft ) { (..., detail::update( ft, packet )); },
			features );
	}


	namespace detail {

		template<typename T, typename Packet_t, typename = void>
		struct has_BlockablePacketUpdate : std::false_type {};

		template<typename T, typename Packet_t>
		struct has_BlockablePacketUpdate<
			T,
			Packet_t,
			std::void_t<decltype(
				std::declval<T>().Update(
					std::declval<GW::HookStatus&>(),
					std::declval<Packet_t&>() ) )>>
			: std::true_type
		{
		};


		template<typename Feature_t, typename Packet_t>
		void update(
			Feature_t& ft, GW::HookStatus& status, Packet_t& packet )
		{
			using T = typename Feature_t::element_type;
			if constexpr ( has_BlockablePacketUpdate<T, Packet_t>::value )
			{
				ft->Update( status, packet );
			}
		}

	}


	template<typename Packet_t>
	void Update(
		Features& features, GW::HookStatus& status, Packet_t& packet )
	{
		std::apply(
			[&status, &packet]( auto&... ft )
			{
				(..., detail::update( ft, status, packet ));
			},
			features );
	}

}
