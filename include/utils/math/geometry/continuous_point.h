#pragma once

#include "point.h"
#include "segment.h"

namespace utils::math::geometry
	{
	struct continuous_point : segment
		{
		using segment::segment;

		continuous_point(const point& a, const point& b) : segment{a, b} {}
		};
	}