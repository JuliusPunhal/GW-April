#pragma once

#include "April/Module/ItemFilter.h"

#include <memory>


namespace April::Gui {

	class Settings_ItemFilter {
	public:
		Settings_ItemFilter(
			std::shared_ptr<Module::ItemFilter>,
			std::shared_ptr<Module::ItemFilter::Config> );

		void Draw();


	private:
		std::shared_ptr<Module::ItemFilter>         item_filter;
		std::shared_ptr<Module::ItemFilter::Config> config;

		int new_model_id = 0;
	};

}
