#pragma once
//inspired by vulkan.hpp, actually blatantly copy pasted

#include <concepts>
#include <algorithm>
#include <type_traits>
#include <initializer_list>

#include <concepts>

//Skip for intellisense cause magic enum is magically good at making intellisense slow
#ifndef __INTELLISENSE__
#include "enum.h"
#endif

namespace utils
	{
	template <typename bit_t>
	class flags
		{
		public:
			using mask_t = typename std::underlying_type<bit_t>::type;

			// constructors
			constexpr flags() noexcept : mask{0} {}
			constexpr flags(                      bit_t  bit ) noexcept : mask{static_cast<mask_t>(bit)} {}
			constexpr flags(std::initializer_list<bit_t> bits) noexcept : mask{0}
				{
				for (bit_t bit : bits)
					{
					mask |= static_cast<mask_t>(bit);
					}
				}

			static consteval flags<bit_t> all() noexcept
				{
				flags<bit_t> ret;

				for (decltype(utils::enums::enum_count<bit_t>()) i{0}; i < utils::enums::enum_count<bit_t>(); i++)
					{
					auto value{utils::enums::enum_value<bit_t>(i)};
					ret |= value;
					}

				return ret;
				}

			constexpr flags(flags<bit_t> const& rhs) noexcept = default;

			explicit constexpr flags(mask_t flags) noexcept : mask(flags) {}

			// relational operators
			constexpr bool has(const bit_t& bit) const noexcept { return (mask & static_cast<mask_t>(bit)) == static_cast<mask_t>(bit); }
			constexpr auto operator<=>(flags<bit_t> const&    ) const noexcept = default;
			constexpr bool operator== (flags<bit_t> const& rhs) const noexcept { return mask == rhs.mask; }
			constexpr bool operator== (      bit_t  const& bit) const noexcept { return mask == static_cast<mask_t>(bit); };

			// logical operator
			constexpr bool operator!() const noexcept
				{
				return !mask;
				}

			// bitwise operators
			constexpr flags<bit_t> operator&(flags<bit_t> const& rhs) const noexcept { return flags<bit_t>(mask & rhs.mask); }
			constexpr flags<bit_t> operator|(flags<bit_t> const& rhs) const noexcept { return flags<bit_t>(mask | rhs.mask); }
			constexpr flags<bit_t> operator^(flags<bit_t> const& rhs) const noexcept { return flags<bit_t>(mask ^ rhs.mask); }
		
			// bitwise operators with bit_t
			constexpr flags<bit_t> operator&(      bit_t  const& rhs) const noexcept { return flags<bit_t>(mask & static_cast<mask_t>(rhs)); }
			constexpr flags<bit_t> operator|(      bit_t  const& rhs) const noexcept { return flags<bit_t>(mask | static_cast<mask_t>(rhs)); }
			constexpr flags<bit_t> operator^(      bit_t  const& rhs) const noexcept { return flags<bit_t>(mask ^ static_cast<mask_t>(rhs)); }

			constexpr flags<bit_t> operator~() const noexcept
				{
				flags<bit_t> ret{*this};
				constexpr auto all_flags{utils::enums::template enum_values<bit_t>()};
				for (auto bit : all_flags)
					{
					ret ^= bit;
					}
				return ret;
				}

			// assignment operators
			constexpr flags<bit_t>& operator=(flags<bit_t> const& rhs) noexcept = default;

			constexpr flags<bit_t>& operator|=(flags<bit_t> const& rhs) noexcept { *this = *this | rhs; return *this; }
			constexpr flags<bit_t>& operator&=(flags<bit_t> const& rhs) noexcept { *this = *this & rhs; return *this; }
			constexpr flags<bit_t>& operator^=(flags<bit_t> const& rhs) noexcept { *this = *this ^ rhs; return *this; }
			// assignment operators with bits
			constexpr flags<bit_t>& operator|=(      bit_t  const& rhs) noexcept { *this = *this | rhs; return *this; }
			constexpr flags<bit_t>& operator&=(      bit_t  const& rhs) noexcept { *this = *this & rhs; return *this; }
			constexpr flags<bit_t>& operator^=(      bit_t  const& rhs) noexcept { *this = *this ^ rhs; return *this; }

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
	template <typename bit_t>
	constexpr flags<bit_t> operator&(bit_t bit, flags<bit_t> const& flags) noexcept
		{
		return flags.operator&(bit);
		}

	template <typename bit_t>
	constexpr flags<bit_t> operator|(bit_t bit, flags<bit_t> const& flags) noexcept
		{
		return flags.operator|(bit);
		}

	template <typename bit_t>
	constexpr flags<bit_t> operator^(bit_t bit, flags<bit_t> const& flags) noexcept
		{
		return flags.operator^(bit);
		}

	}