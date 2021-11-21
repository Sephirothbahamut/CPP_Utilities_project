#pragma once
#include <iterator>
#include <compare>
#include <ranges>
#include <numeric>

namespace utils
	{
	
	template <typename T>
	concept random_accessible_sized_range = std::ranges::random_access_range<T> && std::ranges::sized_range<T>;

	/// <summary>
	/// Note: assumes whatever crap you put inside to start from 0
	/// </summary>
	template <random_accessible_sized_range T>
	auto indices(const T& container) { return std::ranges::iota_view{decltype(std::ranges::size(container)){0}, std::ranges::size(container)}; }

	class Index_range
		{
		public:
			struct iterator
				{
				using iterator_category = std::forward_iterator_tag;
				using difference_type = std::ptrdiff_t;
				using value_type = size_t;
				using pointer = size_t*;  // or also value_type*
				using reference = size_t&;  // or also value_type&

				size_t index;
				      size_t& operator* ()       noexcept { return  index; }
				const size_t& operator* () const noexcept { return  index; }
				      size_t* operator->()       noexcept { return &index; }
				const size_t* operator->() const noexcept { return &index; }

				iterator& operator++() { index++; return *this; }
				iterator operator++(int) { iterator retval = *this; ++(*this); return retval; }

				auto operator<=>(const iterator& other) { return index <=> other.index; }

				bool operator==(const iterator& other) { return index == other.index; }
				bool operator!=(const iterator& other) { return index != other.index; }
				bool operator> (const iterator& other) { return index >  other.index; }
				bool operator< (const iterator& other) { return index <  other.index; }
				bool operator>=(const iterator& other) { return index >= other.index; }
				bool operator<=(const iterator& other) { return index <= other.index; }
				};

			iterator begin() const noexcept { return {0}; }
			iterator end()   const noexcept { return {max};   }

			Index_range(size_t max) noexcept : max{max} {}

		private:
			size_t max;
		};
	}