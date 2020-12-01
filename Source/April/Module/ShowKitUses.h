#pragma once

#include "Dependencies/GWCA.hpp"


namespace April {

	/*	Warning!
		This changes the quantity the client assumes this item has. Therefore, 
		it will also show the incorrect quantity (and value) at e.g. a merchant.
		If you want to sell the kit sure to turn this off and reset the client 
		by reloading the map.
		When you sell stacked items, a different packet is sent to the server,
		therefore you'd be sending packets a client could not normally send.
		Also, the server does not accept the packet in the first place, so this
		cannot be abused as a money-hack.
	*/

	
	class ShowKitUses {
	public:
		ShowKitUses();

	};

}
