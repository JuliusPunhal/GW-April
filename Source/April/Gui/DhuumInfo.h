#pragma once

#include "April/Module/DhuumsJudgement.h"
#include "April/Utility/DrawStructures.h"

#include <memory>


namespace April::Gui {

	class DhuumInfo {
	public:
		struct Config {
			Window window;

			static auto LoadDefault() -> Config;
			static constexpr auto path = "April\\Gui_DhuumInfo.ini";
		};


		DhuumInfo( std::shared_ptr<DhuumsJudgement const>, Config const& );

		void Display() const;


	private:
		std::shared_ptr<DhuumsJudgement const> judgement;
		Config const& config;
	};

}
