#pragma once

#include "common.h"

namespace utils::math::geometry
	{
	class circle : shape_base<circle>
		{
		public:
			vec2f center;
			float radius;

			
			aabb bounding_box() const noexcept
				{
				aabb ret
					{{
					.ll{center.x - radius},
					.up{center.y - radius},
					.rr{center.x + radius},
					.dw{center.y + radius},
					}};

				return ret;
				}
			
			vec2f closest_point_to(const point& other) const noexcept;
			bool  contains        (const point& other) const noexcept;
			
			vec2f                closest_point_to(const segment& other) const noexcept;
			bool                 intersects      (const segment& other) const noexcept;
			std::optional<vec2f> intersection    (const segment& other) const noexcept;
			bool                 contains        (const segment& other) const noexcept;

			vec2f                closest_point_to(const aabb& other) const noexcept;
			vec2f                distance_min    (const aabb& other) const noexcept;
			bool                 intersects      (const aabb& other) const noexcept;
			std::optional<vec2f> intersection    (const aabb& other) const noexcept;
			bool                 contains        (const aabb& other) const noexcept;
			
			vec2f                closest_point_to(const polygon& other) const noexcept;
			vec2f                distance_min    (const polygon& other) const noexcept;
			bool                 intersects      (const polygon& other) const noexcept;
			std::optional<vec2f> intersection    (const polygon& other) const noexcept;
			bool                 contains        (const polygon& other) const noexcept;
			
			vec2f                closest_point_to(const circle& other) const noexcept;
			vec2f                distance_min    (const circle& other) const noexcept;
			bool                 intersects      (const circle& other) const noexcept;
			std::optional<vec2f> intersection    (const circle& other) const noexcept;
			bool                 contains        (const circle& other) const noexcept;
		};
	}