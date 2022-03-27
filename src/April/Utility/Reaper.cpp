
#include "April/Utility/Reaper.h"

#include <array>


namespace {

	struct ReaperName {
		std::array<wchar_t, 4> name;
	};

	constexpr std::array<ReaperName, 7> ReaperNames{
		// Content of GW::Packet::StoC::UpdateAgentName::name[32]
		0x3621, 0xC3FD, 0xD128, 0x1BFB, // Lab
		0x362D, 0xAA21, 0xBC09, 0x7D87, // Vale
		0x361F, 0x8C5D, 0xF18C, 0x67C7, // Wastes
		0x3625, 0x8444, 0xE09B, 0x5660, // Pits
		0x3627, 0x9440, 0xB651, 0x6C09, // Planes
		0x3623, 0xEC03, 0xC5E5, 0x5BF9, // Mnts
		0x3629, 0xDD53, 0xE104, 0x4B27, // Pools
	};


	bool operator==( wchar_t const* name, ReaperName const& reaper )
	{
		return name[0] == reaper.name[0]
			&& name[1] == reaper.name[1]
			&& name[2] == reaper.name[2]
			&& name[3] == reaper.name[3];
	}

}


auto April::GetReaperByName( wchar_t const* name ) -> std::optional<Reaper>
{
	for ( auto it = 0u; it < ReaperNames.size(); ++it )
	{
		if ( name == ReaperNames[it] )
			return static_cast<Reaper>( it );
	}
	return std::nullopt;
}
