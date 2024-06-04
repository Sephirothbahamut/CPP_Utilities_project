#pragma once

#include "base.h"
#include "definitions.h"

namespace utils::details::vector
	{
	template<typename definitions_t>
	struct utils_oop_empty_bases storage
		{
		private:
			inline static constexpr auto static_size              {definitions_t::static_size              };
			inline static constexpr auto static_value_is_reference{definitions_t::static_value_is_reference};
			using self_t                 = typename definitions_t::self_t                ;
			using array_t                = typename definitions_t::array_t               ;
			using value_type             = typename definitions_t::value_type            ;
			using nonref_value_type      = typename definitions_t::nonref_value_type     ;
			using iterator               = typename definitions_t::iterator              ;
			using const_iterator         = typename definitions_t::const_iterator        ;
			using reverse_iterator       = typename definitions_t::reverse_iterator      ;
			using const_reverse_iterator = typename definitions_t::const_reverse_iterator;

		public:
			array_t array;

			utils_gpu_available consteval size_t size() const noexcept { return array.size(); }

			utils_gpu_available constexpr const auto data() const noexcept { return array.data(); }
			utils_gpu_available constexpr       auto data()       noexcept { return array.data(); }
			 
			utils_gpu_available constexpr const nonref_value_type& operator[](size_t index) const noexcept { return static_cast<const nonref_value_type&>(array   [index]); }
			utils_gpu_available constexpr       nonref_value_type& operator[](size_t index)       noexcept { return static_cast<      nonref_value_type&>(array   [index]); }
			utils_gpu_available constexpr const nonref_value_type& at        (size_t index) const          { return static_cast<const nonref_value_type&>(array.at(index)); }
			utils_gpu_available constexpr       nonref_value_type& at        (size_t index)                { return static_cast<      nonref_value_type&>(array.at(index)); }

			utils_gpu_available constexpr       iterator  begin()       noexcept { return {array. begin()}; }
			utils_gpu_available constexpr const_iterator  begin() const noexcept { return {array. begin()}; }
			utils_gpu_available constexpr const_iterator cbegin()       noexcept { return {array.cbegin()}; }
			utils_gpu_available constexpr       iterator  end  ()       noexcept { return {array. end  ()}; }
			utils_gpu_available constexpr const_iterator  end  () const noexcept { return {array. end  ()}; }
			utils_gpu_available constexpr const_iterator cend  ()       noexcept { return {array.cend  ()}; }
			 
			utils_gpu_available constexpr       reverse_iterator  rbegin()       noexcept { return {array. begin()}; }
			utils_gpu_available constexpr const_reverse_iterator  rbegin() const noexcept { return {array. begin()}; }
			utils_gpu_available constexpr const_reverse_iterator crbegin()       noexcept { return {array.cbegin()}; }
			utils_gpu_available constexpr       reverse_iterator  rend  ()       noexcept { return {array. end  ()}; }
			utils_gpu_available constexpr const_reverse_iterator  rend  () const noexcept { return {array. end  ()}; }
			utils_gpu_available constexpr const_reverse_iterator crend  ()       noexcept { return {array.cend  ()}; }

			utils_gpu_available constexpr storage() noexcept requires(!static_value_is_reference) = default;

			template <std::convertible_to<value_type>... Args>
				requires(!static_value_is_reference && static_size >= sizeof...(Args))
			utils_gpu_available constexpr storage(const Args&... args) noexcept : array{std::forward<Args>(args)...} {}

			template <std::same_as<nonref_value_type>... Args>
				requires(static_value_is_reference && static_size == sizeof...(Args))
			utils_gpu_available constexpr storage(const Args&... args) noexcept : array{std::forward<Args>(args)...} {}

			utils_gpu_available constexpr storage(const concepts::compatible_vector<self_t> auto& other) noexcept
				requires(!static_value_is_reference)
				{
				size_t i{0};
				for (; i < utils::math::min(static_size, decltype(other)::static_size); i++)
					{
					operator[](i) = static_cast<value_type>(other[i]);
					}
				for (size_t i = other.size(); i < static_size; i++)
					{
					if constexpr (decltype(other)::static_size) { operator[](i) = value_type{}; }
					else { operator[](i) = value_type{}; }
					}
				}

			utils_gpu_available constexpr storage(const concepts::compatible_vector auto& other) noexcept
				requires
				(
				static_value_is_reference &&
				static_size == decltype(other)::static_size &&
				std::same_as<typename decltype(other)::value_type, nonref_value_type>
				) :
					array{std::apply([](auto&... values) -> array_t { return {values ...}; }, other)}
				{}

			utils_gpu_available constexpr self_t& operator=(const concepts::compatible_vector auto& other) noexcept
				{
				for (size_t i{0}; i < utils::math::min(static_size, decltype(other)::static_size); i++)
					{
					operator[](i) = static_cast<value_type>(other[i]);
					}
				}
		};
	}