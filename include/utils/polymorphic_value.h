#pragma once

#include "third_party/polymorphic_value.h"

namespace utils
	{
	template <class T>
	using polymorphic_value = isocpp_p0201::polymorphic_value<T>;
	}