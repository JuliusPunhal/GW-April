#pragma once

#include "April/Module/NotifyOnEffect.h"

#include <memory>


namespace April::Gui {

	class Settings_NotifyOnEffect {
	public:
		Settings_NotifyOnEffect(
			std::shared_ptr<Module::NotifyOnEffect::Config> );

		void Draw();


	private:
		std::shared_ptr<Module::NotifyOnEffect::Config> config;

		Module::NotifyOnEffect::Notification new_notification{};
	};

}
