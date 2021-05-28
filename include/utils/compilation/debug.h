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