#pragma once

#include "GWCA/GWCA.hpp"

#include <bitset>


namespace April {

	class ReducedSkillRecharge {
	public:
		ReducedSkillRecharge() = default;

		void Update( GW::Packet::StoC::MapLoaded const& );
		void Update( GW::Packet::StoC::SkillRecharge const& );
		void Update( GW::Packet::StoC::SkillRecharged const& );

		bool operator[]( int const idx ) const { return skills[idx]; }


	private:
		std::bitset<8> skills;
	};

}
