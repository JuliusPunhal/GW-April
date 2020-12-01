#pragma once

#include <algorithm>


namespace std {

	template<typename Container_t, typename Element_t>
	inline auto find( Container_t const& container, Element_t const& element )
	{
		return
			std::find(
				std::begin( container ),
				std::end( container ),
				element );
	}

	template<typename Container_t, typename Predicate>
	inline auto find_if( Container_t const& container, Predicate predicate )
	{
		return
			std::find_if(
				std::begin( container ),
				std::end( container ),
				predicate );
	}

	template<typename Container_t, typename Predicate>
	inline auto find_if_not( Container_t const& container, Predicate predicate )
	{
		return
			std::find_if_not(
				std::begin( container ),
				std::end( container ),
				predicate );
	}


	template<class... Ts>
	struct __declspec(empty_bases) overloaded : Ts... {
		using Ts::operator()...;
	};

	template<class... Ts>
	overloaded(Ts...) -> overloaded<Ts...>;

}
