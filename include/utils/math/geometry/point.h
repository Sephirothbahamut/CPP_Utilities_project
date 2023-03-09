#pragma once

#include "common.h"

namespace utils::math::geometry
	{
	class point : public shape_base<point, true>, public vec2f
		{
		public:
			using vec2f::vec;
			                        vec2f closest_point_to(const point                   & b) const noexcept;
			                        vec2f closest_point_to(const segment                 & b) const noexcept;
			template<bool hollow_b> vec2f closest_point_to(const aabb          <hollow_b>& b) const noexcept;
			template<bool hollow_b> vec2f closest_point_to(const polygon       <hollow_b>& b) const noexcept;
			template<bool hollow_b> vec2f closest_point_to(const convex_polygon<hollow_b>& b) const noexcept;
			template<bool hollow_b> vec2f closest_point_to(const circle        <hollow_b>& b) const noexcept;
			template<bool hollow_b> vec2f closest_point_to(const capsule       <hollow_b>& b) const noexcept;
			
			                        bool contains(const point                   & b) const noexcept;
			                        bool contains(const segment                 & b) const noexcept;
			template<bool hollow_b> bool contains(const aabb          <hollow_b>& b) const noexcept;
			template<bool hollow_b> bool contains(const polygon       <hollow_b>& b) const noexcept;
			template<bool hollow_b> bool contains(const convex_polygon<hollow_b>& b) const noexcept;
			template<bool hollow_b> bool contains(const circle        <hollow_b>& b) const noexcept;
			template<bool hollow_b> bool contains(const capsule       <hollow_b>& b) const noexcept;

			aabb<false> bounding_box() const noexcept { return {x, y, x, y}; }
		};
	}