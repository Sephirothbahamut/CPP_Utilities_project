#pragma once

namespace utils
	{
	namespace definitions
		{
		//size_t literal
		constexpr size_t operator "" _size(unsigned long long n) { return n; }

		//compile time strings
		using const_str = const char*;
		}
	}