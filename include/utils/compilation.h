#pragma once

namespace utils::compilation
	{
#ifdef NDEBUG
	constexpr const bool debug = false;
#else
	constexpr const bool debug = true;
#endif
	constexpr const bool release = !debug;
	}