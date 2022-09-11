#pragma once
#include "../../concepts.h"

#include "segment.h"
#include "aabb.h"
#include "circle.h"
#include "point.h"
#include "polygon.h"

namespace utils::math::geometry
	{
	template <typename T>
	concept shape_discrete = ::utils::concepts::any_of<T, point, segment, aabb, circle, polygon, convex_polygon>;
	}