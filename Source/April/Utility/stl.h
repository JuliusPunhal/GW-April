#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <optional>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>


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


namespace stl {

	template<typename Iter_t>
	constexpr auto make_sv( Iter_t const begin, Iter_t const end )
	{
		auto const length = std::distance( begin, end );
		return std::string_view( &*begin, length );
	}


	template<class Iter_t>
	auto stoi_can_parse( Iter_t const begin, Iter_t const end )
	{
		auto const first_char = std::find_if_not( begin, end, isspace );
		return
			first_char != end
			&& (std::isdigit( *first_char ) || *first_char == '-' );
	}

	template<class Container_t>
	auto stoi_can_parse( Container_t const& container )
	{
		return stoi_can_parse( std::begin( container ), std::end( container ) );
	}


	template<class T>
	struct remove_cvref {
		using type = std::remove_cv_t<std::remove_reference_t<T>>;
	};

	template<class T>
	using remove_cvref_t = typename remove_cvref<T>::type;


	template<typename T>
	struct is_std_vector : std::false_type {};

	template<typename T, typename A>
	struct is_std_vector<std::vector<T, A>> : std::true_type {};

	template<typename T>
	inline constexpr auto is_std_vector_v = is_std_vector<T>::value;


	template<typename T>
	struct is_std_array : std::false_type {};

	template<typename T, auto N>
	struct is_std_array<std::array<T, N>> : std::true_type {};

	template<typename T>
	inline constexpr auto is_std_array_v = is_std_array<T>::value;


	template<typename T>
	struct is_std_tuple : std::false_type {};

	template<typename... T>
	struct is_std_tuple<std::tuple<T...>> : std::true_type {};

	template<typename T>
	inline constexpr auto is_std_tuple_v = is_std_tuple<T>::value;


	template<typename T>
	struct is_std_optional : std::false_type {};

	template<typename T>
	struct is_std_optional<std::optional<T>> : std::true_type {};

	template<typename T>
	inline constexpr auto is_std_optional_v = is_std_optional<T>::value;


	template<typename T, typename = void>
	struct is_iterable : std::false_type {};

	template<typename T>
	struct is_iterable<T, std::void_t<
		decltype( std::declval<T>().begin() ),
		decltype( std::declval<T>().end() )>> : std::true_type {};

	template<typename T>
	inline constexpr auto is_iterable_v = is_iterable<T>::value;


	template<auto N = 0, typename Tuple, typename Pred>
	#pragma warning( suppress: 4100 )
	constexpr bool any_of_tuple( Tuple&& tup, Pred pred )
	{
		if constexpr ( N < std::tuple_size_v<std::remove_reference_t<Tuple>> )
		{
			if ( pred( std::get<N>( tup ) ) == true )
				return true;

			return any_of_tuple<N + 1>( std::forward<Tuple>( tup ), pred );
		}
		else return false;
	}


	namespace detail {

		template <class T, class Tuple, std::size_t... N>
		constexpr auto init_from_tuple_impl(
			Tuple&& tup, std::index_sequence<N...> ) -> T
		{
			return T{ std::get<N>( std::forward<Tuple>( tup ) )... };
		}

	}


	template <class T, class Tuple>
	constexpr auto init_from_tuple( Tuple&& tup ) -> T
	{
		// std::make_from_tuple() calls ctor, this uses list initialization

		return
			detail::init_from_tuple_impl<T>(
				std::forward<Tuple>( tup ),
				std::make_index_sequence<
					std::tuple_size_v<std::remove_reference_t<Tuple>>>{} );
	}

}
