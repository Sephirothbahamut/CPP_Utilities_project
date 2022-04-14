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
		vec2f impact_point;
		float t;
		};

	using collision = std::optional<collision_data>;

	namespace details
		{
		void check_continuous_point_to_edge(const continuous_point& point, const segment& edge, std::optional<vec2f>& intersection, segment& hit_edge, float& distance)
			{
			auto tmp_intersection{point.intersection(edge)};
			if (!tmp_intersection) { return; }

			float tmp_distance{vec2f::distance2(tmp_intersection.value(), point.a)};

			if (tmp_distance < distance)
				{
				intersection = tmp_intersection;
				distance = tmp_distance;
				hit_edge = edge;
				}
			}
		}

	inline collision continuous_collides(const continuous_point& point, const aabb& aabb) noexcept
		{
		std::optional<vec2f> intersection{std::nullopt};
		segment hit_edge;
		float distance{std::numeric_limits<float>::infinity()};

		details::check_continuous_point_to_edge(point, segment{aabb.ul, aabb.ur}, intersection, hit_edge, distance);
		details::check_continuous_point_to_edge(point, segment{aabb.ur, aabb.dr}, intersection, hit_edge, distance);
		details::check_continuous_point_to_edge(point, segment{aabb.dr, aabb.dl}, intersection, hit_edge, distance);
		details::check_continuous_point_to_edge(point, segment{aabb.dl, aabb.ul}, intersection, hit_edge, distance);

		if (!intersection) { return std::nullopt; }

		return collision_data
			{
			.normal{hit_edge.perpendicular_left()},
			.impact_point{intersection.value()},
			.t{distance}
			};
		}
	inline collision continuous_collides(const continuous_point& point, const polygon& polygon) noexcept
		{
		std::optional<vec2f> intersection{std::nullopt};
		segment hit_edge;
		float distance{std::numeric_limits<float>::infinity()};

		for (const auto& edge : polygon.get_edges()) { details::check_continuous_point_to_edge(point, edge, intersection, hit_edge, distance); }

		if (!intersection) { return std::nullopt; }

		return collision_data
			{
			.normal{hit_edge.perpendicular_left()},
			.impact_point{intersection.value()},
			.t{distance}
			};
		}
	inline collision continuous_collides(const continuous_point& point, const circle& circle) noexcept
		{//https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm
		//TODO
		}

	}