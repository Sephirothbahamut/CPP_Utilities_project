#pragma once

#include <utility>

namespace utils::oop
	{
	struct empty {};

	template <auto condition, typename type>
	using conditional_inheritance = std::conditional_t<condition, type, empty>;
	}