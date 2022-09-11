#pragma once

#include <limits>

namespace utils::math::identity
	{
	template <typename T>
	consteval T sum() noexcept { return static_cast<T>(0); }
	template <typename T>
	consteval T mul() noexcept { return static_cast<T>(1); }
	}