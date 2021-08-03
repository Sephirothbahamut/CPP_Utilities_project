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
	#define utils_ifdebug(x)
	#define utils_ifrelease(x) x
#else
	#define utils_ifdebug(x) x
	#define utils_ifrelease(x)
#endif