#pragma once

#include "April/Module/ShowKitUses.h"

#include <memory>


namespace April::Gui {

	class Settings_ShowKitUses {
	public:
		Settings_ShowKitUses(
			std::shared_ptr<Module::ShowKitUses>,
			std::shared_ptr<Module::ShowKitUses::Config> );

		void Draw();


	private:
		std::shared_ptr<Module::ShowKitUses>         show_kit_uses;
		std::shared_ptr<Module::ShowKitUses::Config> config;
	};

}
