#pragma once

#include <type_traits>
#include <concepts>

namespace utils
	{
	template <typename T>
	inline bool will_overflow_mult(T a, T b)
		{
		T x = a * b;
		return (a != 0 && x / a != b);
		}

	template <typename T>
	inline T lerp(T a, T b, float t) { return (a * (1.f - t)) + (b * t); }

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

	template <typename T>
	inline void swap(T& a, T& b) { std::swap(a, b); }
	template <std::integral T>
	inline void swap(T& a, T& b) { a ^= b ^= a ^= b; }

	template <typename T>
	inline T clamp(const T& in, const T& min, const T& max) { return std::clamp(in, min, max); }
	}