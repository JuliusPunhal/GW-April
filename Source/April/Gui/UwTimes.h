#pragma once

#include "April/Module/UwTimer.h"

#include <memory>


namespace April::Gui {
	
	class UwTimesGui {
	public:
		UwTimesGui( std::shared_ptr<UwTimes const> );

		void Display() const;


	private:
		std::shared_ptr<UwTimes const> times;
	};

}
