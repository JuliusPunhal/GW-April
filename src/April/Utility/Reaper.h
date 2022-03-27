#pragma once

#include <optional>


namespace April {

	enum class Reaper : char {
		Lab, Vale, Wastes, Pits, Planes, Mnts, Pools
	};


	auto GetReaperByName( wchar_t const* name ) -> std::optional<Reaper>;

}
