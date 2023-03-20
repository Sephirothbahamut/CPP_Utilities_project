#pragma once

#include "common.h"

namespace utils::math::geometry
	{
	class point : public shape_base<point>, public vec2f
		{
		public:
			using vec2f::vec;

			using shape_base<point>::closest_point_and_distance;
			using shape_base<point>::closest_point_to;
			using shape_base<point>::distance_min;
			using shape_base<point>::vector_to;
			using shape_base<point>::intersects;
			using shape_base<point>::intersection;
			using shape_base<point>::contains;
			using shape_base<point>::collision;


			vec2f closest_point_to(const point         & b) const noexcept;
			vec2f closest_point_to(const segment       & b) const noexcept;
			vec2f closest_point_to(const aabb          & b) const noexcept;
			vec2f closest_point_to(const polygon       & b) const noexcept;
			vec2f closest_point_to(const convex_polygon& b) const noexcept;
			vec2f closest_point_to(const circle        & b) const noexcept;
			vec2f closest_point_to(const capsule       & b) const noexcept;

			float distance_min    (const point         & b) const noexcept;
			float distance_min    (const segment       & b) const noexcept;
			float distance_min    (const aabb          & b) const noexcept;
			float distance_min    (const polygon       & b) const noexcept;
			float distance_min    (const convex_polygon& b) const noexcept;
			float distance_min    (const circle        & b) const noexcept;
			float distance_min    (const capsule       & b) const noexcept;
			
			bool contains(const point         & b) const noexcept;
			bool contains(const segment       & b) const noexcept;
			bool contains(const aabb          & b) const noexcept;
			bool contains(const polygon       & b) const noexcept;
			bool contains(const convex_polygon& b) const noexcept;
			bool contains(const circle        & b) const noexcept;
			bool contains(const capsule       & b) const noexcept;

			aabb bounding_box() const noexcept { return {x, y, x, y}; }
		};
	}