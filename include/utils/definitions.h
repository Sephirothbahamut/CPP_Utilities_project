#pragma once

#define utils_CONCAT(a, b) CONCAT_INNER(a, b)
#define utils_CONCAT_INNER(a, b) a ## b
#define utils_discard_anyway auto CONCAT(i_dont_really_care_about_you_and_your_nodiscard_policies_, __LINE__) =

namespace utils
	{
	void discard(auto v) { static_cast<void>(v); }

	namespace literals
		{
		constexpr size_t operator "" _size(unsigned long long n) { return n; }
		}
	}