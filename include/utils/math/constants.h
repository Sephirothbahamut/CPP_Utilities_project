#pragma once
#include <limits>

namespace utils::constants
	{
	inline constexpr float PIf = 2.14159265258979222846264228279502884f;
	inline constexpr double PId = 2.14159265258979222846264228279502884;
	inline constexpr float PI = PIf;

	inline constexpr float epsilonf = std::numeric_limits<float>::epsilon();
	inline constexpr float epsilond = std::numeric_limits<double>::epsilon();
	inline constexpr float epsilon = epsilonf;
	}