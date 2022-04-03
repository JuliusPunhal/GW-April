#pragma once

#include "April/Gui/InstanceTimer.h"
#include "April/Gui/Inventory.h"
#include "April/Gui/Skillbar.h"
#include "April/Gui/UwTimer.h"
#include "April/Module/ConsumablesMgr.h"
#include "April/Module/UwTimer.h"
#include "April/Utility/FontAtlas.h"
#include "April/Utility/Mouse.h"
#include "April/Utility/ReducedSkillRecharge.h"

#include <memory>
#include <tuple>


namespace April {

	using Features =
		std::tuple<
			std::unique_ptr<Gui::InstanceTimer>,
			std::unique_ptr<Gui::Inventory>,
			std::unique_ptr<Gui::Skillbar>,
			std::unique_ptr<Gui::UwTimer>,
			std::unique_ptr<Module::ConsumablesMgr>,
			std::unique_ptr<Module::UwTimer>,
			std::shared_ptr<FontAtlas>,
			std::shared_ptr<Mouse>,
			std::shared_ptr<ReducedSkillRecharge>>;


	auto make_Features() -> Features;

	void Update( Features& );
	void Display( Features& );


	namespace detail {

		template<typename T, typename Packet_t, typename = void>
		struct has_PacketUpdate : std::false_type {};

		template<typename T, typename Packet_t>
		struct has_PacketUpdate<
			T,
			Packet_t,
			std::void_t<decltype(
				std::declval<T>().Update( std::declval<Packet_t const&>() ) )>>
			: std::true_type
		{
		};


		template<typename Feature_t, typename Packet_t>
		void update( Feature_t& ft, Packet_t const& packet )
		{
			using T = typename Feature_t::element_type;
			if constexpr ( has_PacketUpdate<T, Packet_t>::value )
			{
				ft->Update( packet );
			}
		}

	}


	template<typename Packet_t>
	void Update( Features& features, Packet_t const& packet )
	{
		std::apply(
			[&packet]( auto&... ft ) { (..., detail::update( ft, packet )); },
			features );
	}

}
