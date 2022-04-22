
#include "April/Utility/ChainedSouls.h"

#include "April/Utility/stl.h"


namespace {

	auto find_chained_soul( GW::GamePos const& pos ) -> GW::AgentLiving const*
	{
		auto is_soul = [&pos]( GW::Agent const* agent )
		{
			using namespace GW::Constants::ModelID::UW;

			auto const* living = GW::GetAsAgentLiving( agent );
			return
				living
				&& living->player_number == ChainedSoul
				&& living->pos == pos;
		};

		auto const& arr = GW::GetAgentArray();
		auto const found = std::find_if( arr.begin(), arr.end(), is_soul );
		if ( found == arr.end() )
			return nullptr;

		return (*found)->GetAsAgentLiving();
	}

}


April::ChainedSoul::ChainedSoul( float const x, float const y )
	: pos{ GW::GamePos{ x, y, 0u } }
{
}

auto April::ChainedSoul::time_of_death() const
	-> std::optional<GW::InstanceTime>
{
	constexpr auto visitor = stl::overloaded{
		[]( Alive ) { return std::optional<GW::InstanceTime>{}; },
		[]( Dead const dead ) { return dead.time_of_death; },
		[]( OutOfRange const oor ) { return oor.time_of_death; }
	};
	return std::visit( visitor, state_ );
}

void April::ChainedSoul::Update()
{
	if ( GW::GetMapID() != GW::MapID::The_Underworld )
	{
		state_ = OutOfRange{};
	}
	else if (
		auto const* chained_soul = find_chained_soul( pos );
		chained_soul == nullptr )
	{
		state_ = OutOfRange{ time_of_death() };
	}
	else if ( chained_soul->GetIsDead() )
	{
		if ( std::holds_alternative<Alive>( state_ ) )
			state_ = Dead{ GW::GetInstanceTime() };
		else
			state_ = Dead{ time_of_death() };
	}
	else state_ = Alive{ chained_soul->hp, chained_soul->hp_pips };
}

void April::ChainedSouls::Update()
{
	for ( auto& soul : souls )
		soul.Update();
}
