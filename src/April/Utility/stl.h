#pragma once

#include <algorithm>
#include <type_traits>


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


	#if defined(_MSC_VER) && _MSC_VER >= 1900 && _MSC_FULL_VER >= 190023918 && _MSC_VER < 2000
		#define EMPTY_BASES __declspec(empty_bases)
	#else // defined(_MSC_VER) && _MSC_VER >= 1900 && _MSC_FULL_VER >= 190023918 && _MSC_VER < 2000
		#define EMPTY_BASES
	#endif

	template<class... Ts>
	struct EMPTY_BASES overloaded : Ts... {
		using Ts::operator()...;
	};

	template<class... Ts>
	overloaded(Ts...) -> overloaded<Ts...>;


	template<typename T>
	struct remove_cvref {
		using type = std::remove_cv_t<std::remove_reference_t<T>>;
	};

	template<typename T>
	using remove_cvref_t = typename remove_cvref<T>::type;

}
