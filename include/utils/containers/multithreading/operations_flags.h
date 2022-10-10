#pragma once

#include <utility>

#include "../../flags.h"

namespace utils::containers::multithreading
	{
	enum class operation_flag_bits : uint32_t { none = 0b00000000, pre = 0b00000001, post = 0b00000010 };

	inline constexpr flags<operation_flag_bits> operator|(operation_flag_bits bit0, operation_flag_bits bit1) noexcept
		{
		return flags<operation_flag_bits>(bit0) | bit1;
		}

	inline constexpr  flags<operation_flag_bits> operator&(operation_flag_bits bit0,
		operation_flag_bits bit1) noexcept
		{
		return flags<operation_flag_bits>(bit0) & bit1;
		}

	inline constexpr  flags<operation_flag_bits> operator^(operation_flag_bits bit0,
		operation_flag_bits bit1) noexcept
		{
		return flags<operation_flag_bits>(bit0) ^ bit1;
		}

	inline constexpr flags<operation_flag_bits> operator~(operation_flag_bits bits) noexcept
		{
		return ~(flags<operation_flag_bits>(bits));
		}
	}