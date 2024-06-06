#pragma once

#include <utility>

namespace utils::oop
	{
	struct empty {};

	template <auto condition, typename type>
	using type_or_nothing = std::conditional_t<condition, type, empty>;
	}