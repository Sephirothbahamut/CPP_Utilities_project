#pragma once
//inspired by vulkan.hpp, actually blatantly copy pasted

#include <bitset>
#include <concepts>
#include <algorithm>
#include <type_traits>
#include <initializer_list>

#include <concepts>

// Skip for intellisense cause magic enum is 
// magically good at making intellisense slow
#ifndef __INTELLISENSE__
#include "enum.h"
#endif

namespace utils
	{
	template <typename enum_T>
	class flags
		{
		public:
			using enum_t = enum_T;
			using underlying_t = typename std::underlying_type<enum_t>::type;
			using mask_t = underlying_t; //std::bitset<sizeof(enum_T) * CHAR_BIT>;

			// constructors
			constexpr flags() noexcept : mask{0} {}
			constexpr flags(                      enum_t  bit ) noexcept : mask{static_cast<mask_t>(bit)} {}
			constexpr flags(std::initializer_list<enum_t> bits) noexcept : mask{0}
				{
				for (enum_t bit : bits)
					{
					mask |= static_cast<mask_t>(bit);
					}
				}

			static consteval flags<enum_t> full() noexcept
				{
				flags<enum_t> ret;

#ifndef __INTELLISENSE__
				for (decltype(utils::enums::enum_count<enum_t>()) i{0}; i < utils::enums::enum_count<enum_t>(); i++)
					{
					auto value{utils::enums::enum_value<enum_t>(i)};
					ret |= flags<enum_t>{value};
					}
#endif

				return ret;
				}

			constexpr flags(mask_t flags) noexcept : mask(flags) {}

			// bits access
			constexpr bool           test (const flags<enum_t>& bit) const noexcept { return (mask & static_cast<mask_t>(bit)) == static_cast<mask_t>(bit); }
			constexpr flags<enum_t>& set  (const flags<enum_t>& bit)       noexcept { mask |=  static_cast<mask_t>(bit); return *this; }
			constexpr flags<enum_t>& reset(const flags<enum_t>& bit)       noexcept { mask &= ~static_cast<mask_t>(bit); return *this; }
			constexpr flags<enum_t>& flip (const flags<enum_t>& bit)       noexcept { mask ^=  static_cast<mask_t>(bit); return *this; }
			constexpr bool all () const noexcept { return mask == ~mask_t{0}; }
			constexpr bool any () const noexcept { return mask !=  mask_t{0}; }
			constexpr bool none() const noexcept { return mask ==  mask_t{0}; }

			constexpr bool test(std::initializer_list<enum_t> bits) const noexcept 
				{
				for (const auto& bit : bits)
					{
					if (!operator[](bit)) { return false; }
					}
				return true;
				}
			
			constexpr bool operator[]     (const enum_t& bit) const noexcept { return (mask & static_cast<mask_t>(bit)) == static_cast<mask_t>(bit); }
			// relational operators

			constexpr auto operator<=>(const flags<enum_t>&    ) const noexcept = default;
			constexpr bool operator== (const flags<enum_t>& rhs) const noexcept { return mask == rhs.mask; }

			// logical operator
			constexpr bool operator!() const noexcept { return !mask; }

			// bitwise operators
			constexpr flags<enum_t> operator&(const flags<enum_t>& rhs) const noexcept { return flags<enum_t>{static_cast<mask_t>(mask & rhs.mask)}; }
			constexpr flags<enum_t> operator|(const flags<enum_t>& rhs) const noexcept { return flags<enum_t>{static_cast<mask_t>(mask | rhs.mask)}; }
			constexpr flags<enum_t> operator^(const flags<enum_t>& rhs) const noexcept { return flags<enum_t>{static_cast<mask_t>(mask ^ rhs.mask)}; }

			constexpr flags<enum_t> operator~(                        ) const noexcept { return flags<enum_t>{static_cast<mask_t>(~mask)}; }

			// assignment operators
			constexpr flags<enum_t>& operator=(const flags<enum_t>& rhs) noexcept = default;

			constexpr flags<enum_t>& operator|=(const flags<enum_t>& rhs) noexcept { *this = *this | rhs; return *this; }
			constexpr flags<enum_t>& operator&=(const flags<enum_t>& rhs) noexcept { *this = *this & rhs; return *this; }
			constexpr flags<enum_t>& operator^=(const flags<enum_t>& rhs) noexcept { *this = *this ^ rhs; return *this; }

			// cast operators
			explicit constexpr operator bool() const noexcept
				{
				return !!mask;
				}

			explicit constexpr operator mask_t() const noexcept
				{
				return mask;
				}

			mask_t mask;
		};

	// bitwise operators
	template <typename enum_t>
	constexpr flags<enum_t> operator&(enum_t bit, flags<enum_t> const& flags) noexcept
		{
		return flags.operator&(bit);
		}

	template <typename enum_t>
	constexpr flags<enum_t> operator|(enum_t bit, flags<enum_t> const& flags) noexcept
		{
		return flags.operator|(bit);
		}

	template <typename enum_t>
	constexpr flags<enum_t> operator^(enum_t bit, flags<enum_t> const& flags) noexcept
		{
		return flags.operator^(bit);
		}

	}