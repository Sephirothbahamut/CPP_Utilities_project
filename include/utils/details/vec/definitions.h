#pragma once

#include "base.h"

namespace utils::details::vector
	{
	template<typename T, size_t size, template <typename, size_t> class unspecialized_derived_T>
	struct utils_oop_empty_bases definitions
		{
		inline static constexpr const size_t static_size{size};
		inline static constexpr const bool   static_value_is_reference{utils::concepts::reference<T>};
				
		using self_t          = unspecialized_derived_T<T, static_size>;
		using array_t         = std::array<T, static_size>;
		using value_type      = typename array_t::value_type     ;
		using size_type       = typename array_t::size_type      ;
		using difference_type = typename array_t::difference_type;
		using reference       = typename array_t::reference      ;
		using const_reference = typename array_t::const_reference;
		using pointer         = typename array_t::pointer        ;
		using const_pointer   = typename array_t::const_pointer  ;

		template <typename T, size_t size>
		using unspecialized_derived_t = unspecialized_derived_T<T, size>;

		using nonref_value_type = typename utils::remove_reference_t<value_type>;
		using nonref_self_t     = unspecialized_derived_t<nonref_value_type, static_size>;

		template <typename inner_iterator_t, typename T>
		struct base_iterator
			{
			using self_t            = base_iterator<inner_iterator_t, T>;
			using iterator_category = std::random_access_iterator_tag;
			using difference_type   = std::ptrdiff_t;
			using value_type        = T;
			using pointer           = T*;
			using reference         = T&;

			inner_iterator_t inner_iterator;

			self_t  operator+ (difference_type value) const noexcept { return {inner_iterator + value}; }
			self_t  operator- (difference_type value) const noexcept { return {inner_iterator - value}; }
			self_t& operator+=(difference_type value) const noexcept { inner_iterator += value; return *this; }
			self_t& operator-=(difference_type value) const noexcept { inner_iterator += value; return *this; }

			self_t& operator++(   ) noexcept { inner_iterator++; return *this; }
			self_t& operator--(   ) noexcept { inner_iterator--; return *this; }
			self_t  operator++(int) noexcept { self_t tmp{*this}; ++(*this); return tmp; }
			self_t  operator--(int) noexcept { self_t tmp{*this}; --(*this); return tmp; }

			auto operator<=>(const self_t& other) const noexcept { return inner_iterator <=> other.inner_iterator; }
			auto operator== (const self_t& other) const noexcept { return inner_iterator ==  other.inner_iterator; }

			reference operator* () noexcept { return static_cast<reference>(inner_iterator.operator* ()); }
			pointer   operator->() noexcept { return static_cast<pointer  >(inner_iterator.operator->()); }
			};

		using iterator               = typename base_iterator<typename array_t::iterator              ,       nonref_value_type>;
		using const_iterator         = typename base_iterator<typename array_t::const_iterator        , const nonref_value_type>;
		using reverse_iterator       = typename base_iterator<typename array_t::reverse_iterator      ,       nonref_value_type>;
		using const_reverse_iterator = typename base_iterator<typename array_t::const_reverse_iterator, const nonref_value_type>;
		};
	}