#pragma once

#include "aabb.h"
#include "common.h"

namespace utils::math::geometry
	{
	class point
		{
		public:
			vec2f vertex;
			
			vec2f closest_point(const point          & b) const noexcept;
			vec2f closest_point(const segment        & b) const noexcept;
			vec2f closest_point(const aabb           & b) const noexcept;
			vec2f closest_point(const polygon        & b) const noexcept;
			vec2f closest_point(const convex_polygon & b) const noexcept;
			vec2f closest_point(const circle         & b) const noexcept;
			vec2f closest_point(const capsule        & b) const noexcept;
			
			vec2f vector_to(const point          & b) const noexcept;
			vec2f vector_to(const segment        & b) const noexcept;
			vec2f vector_to(const aabb           & b) const noexcept;
			vec2f vector_to(const polygon        & b) const noexcept;
			vec2f vector_to(const convex_polygon & b) const noexcept;
			vec2f vector_to(const circle         & b) const noexcept;
			vec2f vector_to(const capsule        & b) const noexcept;

			float distance_min(const point          & b) const noexcept;
			float distance_min(const segment        & b) const noexcept;
			float distance_min(const aabb           & b) const noexcept;
			float distance_min(const polygon        & b) const noexcept;
			float distance_min(const convex_polygon & b) const noexcept;
			float distance_min(const circle         & b) const noexcept;
			float distance_min(const capsule        & b) const noexcept;
			
			std::optional<vec2f> intersection(const point          & b) const noexcept;
			std::optional<vec2f> intersection(const segment        & b) const noexcept;
			std::optional<vec2f> intersection(const aabb           & b) const noexcept;
			std::optional<vec2f> intersection(const polygon        & b) const noexcept;
			std::optional<vec2f> intersection(const convex_polygon & b) const noexcept;
			std::optional<vec2f> intersection(const circle         & b) const noexcept;
			std::optional<vec2f> intersection(const capsule        & b) const noexcept;
			
			bool contains(const point          & b) const noexcept;
			bool contains(const segment        & b) const noexcept;
			bool contains(const aabb           & b) const noexcept;
			bool contains(const polygon        & b) const noexcept;
			bool contains(const convex_polygon & b) const noexcept;
			bool contains(const circle         & b) const noexcept;
			bool contains(const capsule        & b) const noexcept;
		};

	aabb bounding_box(const point& from) { return {from.vertex.x, from.vertex.y, from.vertex.x, from.vertex.y}; }
	}