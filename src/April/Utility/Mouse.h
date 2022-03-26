#pragma once


namespace April {

	class Mouse {
	public:
		Mouse() = default;

		void suppress() { suppress_ = true; }
		void restore()  { suppress_ = false; }

		bool is_suppressed() const { return suppress_; }


	private:
		bool suppress_ = false;
	};

}
