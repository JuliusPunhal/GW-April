#pragma once

#include <algorithm>
#include <vector>


namespace April {

	template<typename T, typename Comparator_t>
	class sorted_vector {
	public:
		using underlying_t = std::vector<T>;
		using size_type = typename underlying_t::size_type;
		using const_iterator = typename underlying_t::const_iterator;


		sorted_vector() = default;

		sorted_vector( underlying_t const& vec ) : vec{ vec }
		{
			sort();
		}
		sorted_vector( underlying_t&& vec ) : vec{ std::move( vec ) }
		{
			sort();
		}
		sorted_vector( std::initializer_list<T> il )
			: vec{ il.begin(), il.end() }
		{
			sort();
		}

		auto size() const noexcept { return vec.size(); }
		auto empty() const noexcept { return vec.empty(); }

		auto begin() const noexcept { return vec.begin(); }
		auto end() const noexcept { return vec.end(); }

		auto front() const { return vec.front(); }
		auto back() const { return vec.back(); }

		auto operator[]( size_type const it ) const { return vec[it]; }

		void push_back( T const& el )
		{
			vec.push_back( el );
			sort();
		}
		void push_back( T&& el )
		{
			vec.push_back( std::move( el ) );
			sort();
		}

		void replace( const_iterator const it, T const& el )
		{
			auto const pos = it - vec.begin();
			vec[pos] = el;
			sort();
		}
		void replace( const_iterator const it, T&& el )
		{
			auto const pos = it - vec.begin();
			vec[pos] = std::move( el );
			sort();
		}

		auto clear() noexcept { return vec.clear(); }
		auto erase( const_iterator const it ) { return vec.erase( it ); }


	private:
		std::vector<T> vec;

		void sort() { std::sort( vec.begin(), vec.end(), Comparator_t{} ); }
	};

}
