#pragma once
#include "../../concepts.h"

#include "shapes/segment.h"
#include "shapes/aabb.h"
#include "shapes/circle.h"
#include "shapes/point.h"
#include "shapes/polygon.h"

namespace utils::math::geometry
	{
	template <typename T>
	concept shape_discrete = ::utils::concepts::any_of<T, point, segment, aabb, circle, polygon, convex_polygon>;
	}