#pragma once

#include <concepts>
#include <algorithm>
#include <type_traits>

#include "../compilation/gpu.h"
#include "../memory.h"

namespace utils::math
	{
	namespace concepts
		{
		template <typename T>
		concept number = std::integral<T> || std::floating_point<T>;

		template <typename T>
		concept undecorated_floating_point = std::floating_point<std::remove_cvref_t<T>>;
		template <typename T>
		concept undecorated_integral = std::integral<std::remove_cvref_t<T>>;

		template <typename T>
		concept undecorated_number = undecorated_floating_point<T> || undecorated_integral<T>;
		};

	template <typename T>
	utils_gpu_available constexpr T sign(T value) noexcept
		{
		return value < T{0} ? T{-1} : value > T{0} ? T{1} : T{0};
		}

	//TODO replace with std::abs when std::abs becomes constexpr conforming to C++23 standard
	template <typename T>
	utils_gpu_available constexpr T abs(T value) noexcept
		{
		return value < T{0} ? -value : value;
		}

	template <typename T>
	utils_gpu_available constexpr bool will_overflow_mult(T a, T b) noexcept
		{
		T x = a * b;
		return (a != 0 && x / a != b);
		}

	template <typename T, typename oth_t>
	utils_gpu_available constexpr T lerp(T a, oth_t b, float t) noexcept { return (a * (1.f - t)) + (b * t); }

	template <typename T>
	utils_gpu_available constexpr T inverse_lerp(T a, T b, float t) noexcept { return (t - a) / (b - a); }

	template <typename T>
	utils_gpu_available constexpr T map(T from_min, T from_max, T to_min, T to_max, T value) noexcept
		{
		return lerp(to_min, to_max, inverse_lerp(from_min, from_max, value));
		}

	template <typename T, char iterations = 2>
	utils_gpu_available constexpr T inv_sqrt(T x) noexcept
		{//https://stackoverflow.com/questions/11644441/fast-inverse-square-root-on-x64/11644533
		static_assert(std::is_floating_point<T>::value, "T must be floating point");
		static_assert(iterations == 1 || iterations == 2, "itarations must equal 1 or 2");
		typedef typename std::conditional<sizeof(T) == 8, std::int64_t, std::int32_t>::type Tint;
		T y = x;
		T x2 = y * 0.5;
		Tint i = *(Tint*)&y;
		i = (sizeof(T) == 8 ? 0x5fe6eb50c7b537a9 : 0x5f3759df) - (i >> 1);
		y = *(T*)&i;
		y = y * (1.5 - (x2 * y * y));
		if (iterations == 2)
			y = y * (1.5 - (x2 * y * y));
		return y;
		}

	//TODO understand why it doesn't compile because it tries to instantiate the template with "utils::message**" ???
	//template <typename T>
	//inline void swap(T& a, T& b) { std::swap(a, b); }

	template <std::integral T>
	utils_gpu_available constexpr void swap(T& a, T& b) noexcept { a ^= b ^= a ^= b; }

	template <typename T>
	utils_gpu_available constexpr T clamp(const T& in, const T& min, const T& max) noexcept { return std::clamp(in, min, max); }

	//template <typename ...Ts>
	//utils_gpu_available constexpr auto min(const Ts& ...values) noexcept { return std::min({values...}); }

	template <typename T>
	utils_gpu_available constexpr T min(const T& a, const T& b) noexcept { return a < b ? a : b; }

	//template <typename ...Ts>
	//utils_gpu_available constexpr auto max(const Ts& ...values) noexcept { return std::max({values...}); }

	template <typename T>
	utils_gpu_available constexpr T max(const T& a, const T& b) noexcept { return a > b ? a : b; }

	template <std::integral to_t, typename from_t>
	requires (concepts::number<from_t>) 
	utils_gpu_available constexpr to_t cast_clamp(from_t f) noexcept
		{
		if constexpr (std::numeric_limits<from_t>::max() > std::numeric_limits<to_t>::max())
			{
			if (f > static_cast<from_t>(std::numeric_limits<to_t>::max())) { return std::numeric_limits<to_t>::max(); }
			}

		if constexpr (!std::unsigned_integral<from_t>)
			{
			if constexpr (!std::unsigned_integral<to_t>)
				{
				if constexpr (std::numeric_limits<from_t>::min() < std::numeric_limits<to_t>::min())
					{
					if (f < static_cast<from_t>(std::numeric_limits<to_t>::min())) { return std::numeric_limits<to_t>::min(); }
					}
				}
			else
				{
				if (f < from_t{0}) { return to_t{0}; }
				}
			}

		return static_cast<to_t>(f);
		}
	
	template <typename T, T FULL_VALUE>
	struct type_based_numeric_range;

	namespace concepts
		{
		template <typename T>
		concept type_based_numeric_range = std::same_as<T, utils::math::type_based_numeric_range<typename T::value_type, T::full_value>>;
		}

	template <typename T, T FULL_VALUE = std::floating_point<T> ? static_cast<T>(1.) : std::numeric_limits<T>::max() >
	struct type_based_numeric_range
		{
		using value_type = T;
		inline static constexpr const T full_value{FULL_VALUE};

		template <concepts::type_based_numeric_range other>
		utils_gpu_available static other::value_type cast_to(value_type value) noexcept
			{
			if constexpr (std::same_as<value_type, typename other::value_type>)
				{
				if constexpr (full_value == other::full_value) { return value; }
				}

			using tmp_t = std::conditional_t<(std::numeric_limits<value_type>::max() > std::numeric_limits<typename other::value_type>::max()), value_type, typename other::value_type>;
			tmp_t tmp{(static_cast<tmp_t>(value) / static_cast<tmp_t>(full_value)) * static_cast<tmp_t>(other::full_value)};
			return static_cast<other::value_type>(tmp);
			}
		};
	}