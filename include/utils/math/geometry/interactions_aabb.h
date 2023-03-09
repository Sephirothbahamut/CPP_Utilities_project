#pragma once

#include "../rect.h"

namespace utils::math::geometry
	{
	using aabb = rect<float>;

	aabb bounding_box(const aabb& from) { return from; }
	}