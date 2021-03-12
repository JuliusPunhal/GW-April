#pragma once

#include "April/Utility/Consumable.h"

#include "Dependencies/GWCA.hpp"

#include <optional>
#include <string>
#include <variant>
#include <vector>


namespace April {

	struct SendChat {
		char        channel;
		std::string message;
	};

	struct WriteChat {
		GW::Chat::Channel channel;
		std::string       message;
	};

	struct SendDialog {
		int dialog;
	};

	struct UseConsumable {
		Consumable      consumable;
		GW::Item const* item;
	};

	struct UseSkill {
		unsigned         slot;   // 0 ... 7
		GW::Agent const* target;
	};

	struct Return_to_Outpost {};

	struct EnableConsumables {
		std::vector<GW::ItemID> consumables;
		bool                    persistent;
	};

	struct DisableConsumables {
		std::vector<GW::ItemID> consumables;
		bool                    persistent;
	};

	struct DisableAllConsumables {
		bool persistent;
	};

	struct SetConsumablesObjective {
		GW::ObjectiveID objective;
	};

	struct OpenXunlai {};

	struct ToggleGui {
		std::string window_name;
	};

	struct SetUwGuiVisibility {
		enum class Type { General, Pits, Dhuum };
		using Visibility = std::optional<bool>; // std::nullopt == toggle

		Type       gui;
		Visibility visiblity;
	};

	struct ShowSuppresedAgents {};

	struct NoCommand_t {};

	struct Exit {};


	using Command =
		std::variant<
			SendChat, WriteChat,
			SendDialog,
			UseConsumable,
			UseSkill,
			Return_to_Outpost,
			EnableConsumables, DisableConsumables, DisableAllConsumables,
			SetConsumablesObjective,
			OpenXunlai,
			ToggleGui, SetUwGuiVisibility,
			ShowSuppresedAgents,
			NoCommand_t,
			Exit>;


	inline auto NoCommand = Command{ NoCommand_t{} };

}
