#pragma once

#include "April/Module/DhuumsJudgement.h"

#include "Dependencies/ImGui.hpp"

#include <memory>
#include <string>


namespace April::Gui {

	class DhuumInfo {
	public:
		struct Config {
			std::string			window_name;
			ImGuiWindowFlags	window_flags;

			static auto LoadDefault() -> Config;
		};


		DhuumInfo( std::shared_ptr<DhuumsJudgement const>, Config const& );

		void Display() const;


	private:
		std::shared_ptr<DhuumsJudgement const> judgement;
		Config const& config;
	};

}
