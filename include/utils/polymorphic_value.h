#pragma once

#include "third_party/polymorphic_value.h"

namespace utils
	{
	template <class T>
	using polymorphic_value = typename utils::polymorphic_value<T>;
	}