#pragma once

#include "segment.h"

namespace utils::math::geometry
	{
	struct continuous_point : segment
		{
		using segment::segment;

		continuous_point(const segment& segment) : segment{segment} {}
		};
	}