#pragma once

#include "point.h"
#include "aabb.h"
#include "ab.h"
#include "polyline.h"

namespace utils::math::geometry::interactions
	{
	return_types::gradient_signed_distance gradient_signed_distance(const shape::concepts::any auto& shape, const shape::concepts::point auto& point) noexcept
		{
		const auto closest_with_signed_distance{interactions::closest_with_signed_distance(shape, point)};
		return return_types::gradient_signed_distance::create(closest_with_signed_distance, point);
		}
	}