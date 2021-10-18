#pragma once
#include <iterator>
#include <compare>

namespace utils
	{
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