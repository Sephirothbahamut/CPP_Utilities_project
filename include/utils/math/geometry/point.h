#pragma once

#include "aabb.h"

namespace utils::math::geometry
	{
	using point = vec2f;

	aabb bounding_box(const point& from) { return {from.x, from.y, from.x, from.y}; }
	}