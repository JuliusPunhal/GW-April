#pragma once

#include <algorithm>


namespace stl {

	template<typename Container_t, typename Element_t>
	inline auto find( Container_t& container, Element_t const& element )
	{
		return
			std::find(
				std::begin( container ),
				std::end( container ),
				element );
	}

	template<typename Container_t, typename Predicate>
	inline auto find_if( Container_t& container, Predicate predicate )
	{
		return
			std::find_if(
				std::begin( container ),
				std::end( container ),
				predicate );
	}

}
