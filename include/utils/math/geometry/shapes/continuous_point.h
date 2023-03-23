#pragma once

#include "../common/begin.h"
#include "aabb.h"

namespace utils::math::geometry
	{
	template <bool view>
	struct continuous_point : segment<view>
		{
		using segment<view>::segment;
		};
	}