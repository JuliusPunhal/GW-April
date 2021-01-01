#pragma once

namespace April {
	struct ModuleConfigurations; // avoid cyclic include
}


namespace April {

	class WindowMgr {
	public:
		WindowMgr( ModuleConfigurations& );

		void Update();


	private:
		ModuleConfigurations& configs;
		bool in_pits = false;
	};

}
