#pragma once

#include "April/Framework/Command.h"
#include "April/Framework/Instance.h"


namespace April {

	class OnCommand {
	public:
		OnCommand( Instance& );

		void operator()( SendChat const& ) const;
		void operator()( WriteChat const& ) const;
		void operator()( SendDialog const& ) const;
		void operator()( UseConsumable const& ) const;
		void operator()( UseSkill const& ) const;
		void operator()( Return_to_Outpost const& ) const;
		void operator()( EnableConsumables const& ) const;
		void operator()( DisableConsumables const& ) const;
		void operator()( DisableAllConsumables const& ) const;
		void operator()( SetConsumablesObjective const& ) const;
		void operator()( OpenXunlai const& ) const;
		void operator()( ToggleGui const& ) const;
		void operator()( SetUwGuiVisibility const& ) const;
		void operator()( ShowSuppresedAgents const& ) const;
		void operator()( NoCommand_t const& ) const;
		void operator()( Exit const& ) const;


	private:
		Instance& instance;
	};

}
