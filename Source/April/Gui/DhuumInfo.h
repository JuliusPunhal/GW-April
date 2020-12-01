#pragma once

#include "April/Module/DhuumsJudgement.h"

#include <memory>


namespace April::Gui {

	class DhuumInfo {
	public:
		DhuumInfo( std::shared_ptr<DhuumsJudgement const> );

		void Display() const;


	private:
		std::shared_ptr<DhuumsJudgement const> judgement;
	};

}
