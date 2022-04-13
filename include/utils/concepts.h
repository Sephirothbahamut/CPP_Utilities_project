#pragma once

#include <concepts>

namespace utils::concepts
	{
	template<typename T, typename ... U>
	concept any_of = (std::same_as<T, U> || ...);
	}