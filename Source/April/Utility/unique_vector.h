#pragma once

#include <vector>


namespace April {

	template<typename Element_t>
	class unique_vector {
	public:
		using size_type = typename std::vector<Element_t>::size_type;
		using const_iterator = typename std::vector<Element_t>::const_iterator;


		unique_vector() = default;

		unique_vector( std::vector<Element_t> const& vec )
			: elements{ vec }
		{
			remove_duplicates();
		}

		unique_vector( std::vector<Element_t>&& vec )
			: elements{ std::move( vec ) }
		{
			remove_duplicates();
		}

		unique_vector( std::initializer_list<Element_t> il )
			: elements{ il.begin(), il.end() }
		{
			remove_duplicates();
		}


		auto size() const noexcept { return elements.size(); }
		auto empty() const noexcept { return elements.empty(); }

		auto begin() const noexcept { return elements.begin(); }
		auto end() const noexcept { return elements.end(); }

		auto cbegin() const noexcept { return elements.begin(); }
		auto cend() const noexcept { return elements.end(); }

		auto rbegin() const noexcept { return elements.rbegin(); }
		auto rend() const noexcept { return elements.rend(); }

		auto crbegin() const noexcept { return elements.crbegin(); }
		auto crend() const noexcept { return elements.crend(); }

		auto operator[]( size_type const it ) const -> Element_t const&
		{
			return elements[it];
		}

		void push_back( Element_t const& el )
		{
			if ( not is_present( el ) )
				elements.push_back( el );
		}
		void push_back( Element_t&& el )
		{
			if ( not is_present( el ) )
				elements.push_back( std::move( el ) );
		}

		auto erase( const_iterator where )
		{
			return elements.erase( where );
		}
		auto erase( const_iterator first, const_iterator last )
		{
			return elements.erase( first, last );
		}

		auto clear() noexcept { return elements.clear(); }


	private:
		std::vector<Element_t> elements;


		bool is_present( Element_t const& el ) const noexcept
		{
			return std::find( elements.begin(), elements.end(), el )
				!= elements.end();
		}
		void remove_duplicates() noexcept
		{
			auto const remove_duplicates = []( auto first, auto last )
			{
				while ( first + 1 < last )
				{
					last = std::remove( first + 1, last, *first );
					++first;
				}
				return last;
			};

			elements.erase(
				remove_duplicates( elements.begin(), elements.end() ) );
		}

	};

}
