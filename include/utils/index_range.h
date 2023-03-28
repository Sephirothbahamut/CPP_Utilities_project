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
	auto indices(const T& container, size_t min = 0) { return std::ranges::iota_view {decltype(std::ranges::size(container)){min}, std::ranges::size(container)}; }

	template <typename callback_t>
	class index_range
		{
		public:
			template <typename T>
			class iterator
				{
				friend class index_range<callback_t>;
				public:
					using iterator_category = std::forward_iterator_tag;
					using difference_type = std::ptrdiff_t;
					using value_type = T;
					using pointer    = value_type*;
					using reference  = value_type&;

					size_t size() const noexcept { return max - min; }
					size_t index;
					auto operator* ()       noexcept { return callback(index); }
					auto operator* () const noexcept { return callback(index); }
					auto operator->()       noexcept { return callback(index); }
					auto operator->() const noexcept { return callback(index); }

					iterator& operator++() { index++; return *this; }
					iterator operator++(int) { iterator retval{*this}; ++(*this); return retval; }

					auto operator<=>(const iterator& other) { return index <=> other.index; }

					bool operator==(const iterator& other) { return index == other.index; }
					bool operator!=(const iterator& other) { return index != other.index; }
					bool operator> (const iterator& other) { return index >  other.index; }
					bool operator< (const iterator& other) { return index <  other.index; }
					bool operator>=(const iterator& other) { return index >= other.index; }
					bool operator<=(const iterator& other) { return index <= other.index; }

				private:
					iterator(size_t index, callback_t callback) : index{index}, callback{callback} {}
					callback_t callback;
				};

			auto begin() const noexcept { return iterator<decltype(callback(size_t{0}))>{min, callback}; }
			auto end()   const noexcept { return iterator<decltype(callback(size_t{0}))>{max, callback}; }

			index_range(size_t min, size_t max, callback_t callback) noexcept : min{min}, max{max}, callback{callback} {}

		private:
			size_t min;
			size_t max;
			callback_t callback;
		};
	}