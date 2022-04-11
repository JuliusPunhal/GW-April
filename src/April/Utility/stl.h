#pragma once

#include <algorithm>
#include <string>
#include <string_view>
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


	template<typename Iter_t>
	constexpr auto make_sv( Iter_t const begin, Iter_t const end )
	{
		if ( begin == end )
		{
			// prevents error when begin points to past-the-end-element which
			// can't be dereferenced below.
			return std::string_view{};
		}

		auto const length = std::distance( begin, end );
		return std::string_view( &*begin, length );
	}

	auto utf8_to_wstr( std::string const& ) -> std::wstring;
	auto wstr_to_utf8( std::wstring const& ) -> std::string;


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
