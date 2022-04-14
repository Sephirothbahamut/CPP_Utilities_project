#pragma once

#include <optional>
#include <limits>

#include "../vec2.h"
#include "../angle.h"
#include "segment.h"
#include "polygon.h"
#include "circle.h"
#include "aabb.h"

#include "continuous_point.h"

#include "../constants.h"

namespace utils::math::geometry
	{
	struct collision_data
		{
		vec2f normal;
		vec2f impat_point;
		float t;
		};

	using collision = std::optional<collision_data>;

	inline collision continuous_collides(const continuous_point& point, const aabb& aabb) noexcept
		{
		vec2f intersection;
		float distance{std::numeric_limits<float>::infinity()};

		auto check = [&](vec2f a, vec2f b)
			{
			vec2f tmp_intersection{point.intersects_line(segment{aabb.ul, aabb.ur})};
			float tmp_distance{vec2f::distance2(tmp_intersection, point.a)};

			if (tmp_distance < distance)
				{
				intersection = tmp_intersection;
				distance = tmp_distance;
				}
			};

		check(aabb.ul, aabb.ur);
		check(aabb.ur, aabb.dr);
		check(aabb.dr, aabb.dl);
		check(aabb.dl, aabb.ul);
		}

	}