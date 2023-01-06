#pragma once

#include <type_traits>
#include <concepts>

namespace utils::math
	{
	template <typename T>
	inline bool will_overflow_mult(T a, T b)
		{
		T x = a * b;
		return (a != 0 && x / a != b);
		}

	template <typename T, typename oth_t>
	inline T lerp(T a, oth_t b, float t) { return (a * (1.f - t)) + (b * t); }

	template <typename T>
	inline T inverse_lerp(T a, T b, float t) { return (t - a) / (b - a); }

	template <typename T>
	inline T map(T from_min, T from_max, T to_min, T to_max, T value)
		{
		return lerp(to_min, to_max, inverse_lerp(from_min, from_max, value));
		}

	template <typename T, char iterations = 2>
	inline T inv_sqrt(T x)
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
	constexpr void swap(T& a, T& b) { a ^= b ^= a ^= b; }

	template <typename T>
	constexpr T clamp(const T& in, const T& min, const T& max) { return std::clamp(in, min, max); }

	template <typename ...Ts>
	constexpr auto min(const Ts& ...values) { return std::min({values...}); }

	template <typename ...Ts>
	constexpr auto max(const Ts& ...values) { return std::max({values...}); }

	template <std::integral to_t, typename from_t>
	requires (std::integral<from_t> || std::floating_point<from_t>) 
	to_t cast_clamp(from_t f)
		{
		if constexpr (std::numeric_limits<from_t>::max() > std::numeric_limits<to_t>::max())
			{
			if (f > static_cast<from_t>(std::numeric_limits<to_t>::max())) { return static_cast<from_t>(std::numeric_limits<to_t>::max()); }
			}

		if constexpr (!std::unsigned_integral<from_t>)
			{
			if constexpr (!std::unsigned_integral<to_t>)
				{
				if constexpr (std::numeric_limits<from_t>::min() < std::numeric_limits<to_t>::min())
					{
					if (f < static_cast<from_t>(std::numeric_limits<to_t>::min())) { return static_cast<from_t>(std::numeric_limits<to_t>::min()); }
					}
				}
			else
				{
				if (f < from_t{0}) { return to_t{0}; }
				}
			}

		return static_cast<to_t>(f);
		}
	}