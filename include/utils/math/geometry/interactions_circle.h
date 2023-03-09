#pragma once


#include "inner_shapes.h"

namespace utils::math::geometry
	{
	template <bool hollow>
	vec2f circle<hollow>::closest_point_to(const point& other) const noexcept
		{
		auto vec{other.vertex - center};
		vec.length = radius;
		return vec;
		}

	template <bool hollow>
	bool circle<hollow>::contains(const point& other) const noexcept
		{
		return point{center}.distance_min(other) <= radius;
		}

	template <bool hollow>
	vec2f circle<hollow>::closest_point_to(const segment& b) const noexcept
		{
		auto closest_to_center{b.closest_point_to(point{center})};
		float dist_to_center{closest_to_center.minimum_distance(point{center)}};
		if (dist_to_center >= radius)
			{
			return closest_point_to(point{closest_to_center});
			}
		else
			{
			return {}; //TODO intersections
			}
		}

	template<bool hollow>
	bool circle<hollow>::intersects(const segment& other) const noexcept
		{
		if ( contains(other.a) && !contains(other.b)) { return true; }
		if ( contains(other.b) && !contains(other.a)) { return true; }
		if (!contains(other.b) && !contains(other.a)) { return other.distance_min(center) <= radius; }
		return false;
		}

	template<bool hollow>
	std::optional<vec2f> circle<hollow>::intersection(const segment& other) const noexcept
		{
		//TODO
		return std::nullopt;
		}

	template<bool hollow>
	bool circle<hollow>::contains(const segment& other) const noexcept
		{
		return contains(other.a) && contains(other.b);
		}

	template<bool hollow> template<bool hollow_b> vec2f                circle<hollow>::closest_point_to(const aabb<hollow_b>& other) const noexcept;
	template<bool hollow> template<bool hollow_b> vec2f                circle<hollow>::distance_min    (const aabb<hollow_b>& other) const noexcept;
	template<bool hollow> template<bool hollow_b> bool                 circle<hollow>::intersects      (const aabb<hollow_b>& other) const noexcept;
	template<bool hollow> template<bool hollow_b> std::optional<vec2f> circle<hollow>::intersection    (const aabb<hollow_b>& other) const noexcept;
	template<bool hollow> template<bool hollow_b> bool                 circle<hollow>::contains        (const aabb<hollow_b>& other) const noexcept;
	
	template<bool hollow> template<bool hollow_b> vec2f                circle<hollow>::closest_point_to(const polygon<hollow_b>& other) const noexcept;
	template<bool hollow> template<bool hollow_b> vec2f                circle<hollow>::distance_min    (const polygon<hollow_b>& other) const noexcept;
	template<bool hollow> template<bool hollow_b> bool                 circle<hollow>::intersects      (const polygon<hollow_b>& other) const noexcept;
	template<bool hollow> template<bool hollow_b> std::optional<vec2f> circle<hollow>::intersection    (const polygon<hollow_b>& other) const noexcept;
	template<bool hollow> template<bool hollow_b> bool                 circle<hollow>::contains        (const polygon<hollow_b>& other) const noexcept;
	
	template<bool hollow> template<bool hollow_b> vec2f                circle<hollow>::closest_point_to(const circle<hollow_b>& other) const noexcept;
	template<bool hollow> template<bool hollow_b> vec2f                circle<hollow>::distance_min    (const circle<hollow_b>& other) const noexcept;
	template<bool hollow> template<bool hollow_b> bool                 circle<hollow>::intersects      (const circle<hollow_b>& other) const noexcept;
	template<bool hollow> template<bool hollow_b> std::optional<vec2f> circle<hollow>::intersection    (const circle<hollow_b>& other) const noexcept;
	template<bool hollow> template<bool hollow_b> bool                 circle<hollow>::contains        (const circle<hollow_b>& other) const noexcept;
	}