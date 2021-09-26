#pragma once

namespace utils::compilation
	{
	inline constexpr bool debug
#ifdef NDEBUG
		{false};
#else
		{true};
#endif
	inline constexpr bool release{!debug};
	}

#ifdef NDEBUG
	#define utils_is_release
	#define utils_if_debug(x)
	#define utils_if_release(x) x
#else
	#define utils_is_debug
	#define utils_if_debug(x) x
	#define utils_if_release(x)
#endif