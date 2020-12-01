#pragma once

#include "Dependencies/GWCA.hpp"

#include <unordered_map>
#include <vector>


namespace April {

	class AgentFilter {
	public:
		AgentFilter();

		void DisplaySuppressedItems();

		
	private:
		std::vector<GW::Packet::StoC::AgentAdd> suppressed_packets;
		std::unordered_map<GW::ItemID, GW::AgentID> item_owners;
				
		void OnSpawn( GW::HookStatus*, GW::Packet::StoC::AgentAdd const& );
		void OnDespawn( GW::HookStatus*, GW::Packet::StoC::AgentRemove const& );
		void UpdateOwner( GW::Packet::StoC::UpdateItemOwner const& );
		void DeleteOwner( GW::Packet::StoC::ItemGeneral_ReuseID const& );
		void Reset();
	};

}
