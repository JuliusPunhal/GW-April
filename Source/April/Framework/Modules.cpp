
#include "April/Framework/Modules.h"

#include <memory>
#include <tuple>

using namespace April;
using namespace std::chrono;


namespace {

	template<auto N = 0, typename... Args>
	#pragma warning( suppress: 4100 )
	void update( std::tuple<Args...>& modules )
	{
		if constexpr ( N < sizeof...( Args ) )
		{
			std::get<N>( modules )->Update();
			update<N + 1>( modules );
		}
	}

	template<auto N = 0, typename... Args>
	#pragma warning( suppress: 4100 )
	void display( std::tuple<Args...>& modules )
	{
		if constexpr ( N < sizeof...( Args ) )
		{
			std::get<N>( modules )->Display();
			display<N + 1>( modules );
		}
	}

}


April::Modules::Modules(
	Active&& active,
	Passive&& passive,
	Guis&& gui,
	std::unique_ptr<ModuleConfigurations> config )
	:
	active{ std::move( active ) },
	passive{ std::move( passive ) },
	gui{ std::move( gui ) },
	config{ std::move( config ) }
{
}

void April::Modules::Update()
{
	update( active );
}

void April::Modules::Display()
{
	display( gui );
}

void April::Modules::Shutdown()
{
	std::get<std::unique_ptr<AgentFilter>>( passive )->DisplaySuppressedItems();
}
