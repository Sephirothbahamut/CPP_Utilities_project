#pragma once

#include <numbers>

#include <limits>

namespace utils::math::constants
	{
	inline constexpr float  PIf = static_cast<float>(std::numbers::pi);
	inline constexpr double PId = std::numbers::pi;
	inline constexpr float  PI  = PIf;

	inline constexpr double epsilond = 0.0000000000001;//std::numeric_limits<double>::epsilon();
	inline constexpr float  epsilonf = static_cast<float>(epsilond);//std::numeric_limits<float>::epsilon();
	inline constexpr float  epsilon  = epsilonf;

	inline constexpr double dinf = std::numeric_limits<double>::infinity();
	inline constexpr float  finf = std::numeric_limits<float >::infinity();
	inline constexpr float  inf  = finf;
	inline constexpr float  inff = finf;
	inline constexpr float  infd = dinf;
	}