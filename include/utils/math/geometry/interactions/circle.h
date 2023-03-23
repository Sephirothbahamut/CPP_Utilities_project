#pragma once
#include "../shapes.h"

namespace utils::math::geometry
	{
	template <bool view_a> template <bool view_b>
	vec2f circle<view_a>::closest_point_to(const point<view_b>& other) const noexcept
		{
		vec2f vec{vec2f{other} - vec2f{center}};
		vec.length = radius;
		return vec;
		}

	template <bool view_a> template <bool view_b>
	bool circle<view_a>::contains(const point<view_b>& other) const noexcept
		{
		return point<false>{utils::math::vec2f{center}}.distance_min(other) <= radius;
		}

	template <bool view_a> template <bool view_b>
	vec2f circle<view_a>::closest_point_to(const segment<view_b>& b) const noexcept
		{
		point closest_to_center{b.closest_point_to(point{center})};
		float dist_to_center{closest_to_center.distance_min(point{center})};
		if (dist_to_center >= radius)
			{
			return closest_point_to(point{closest_to_center});
			}
		else
			{
			return {}; //TODO intersections
			}
		}

	template <bool view_a> template <bool view_b>
	bool circle<view_a>::intersects(const segment<view_b>& other) const noexcept
		{
		if ( contains(other.a) && !contains(other.b)) { return true; }
		if ( contains(other.b) && !contains(other.a)) { return true; }
		if (!contains(other.b) && !contains(other.a)) { return other.distance_min(center) <= radius; }
		return false;
		}

	template <bool view_a> template <bool view_b>
	std::optional<vec2f> circle<view_a>::intersection(const segment<view_b>& other) const noexcept
		{
		//TODO
		return std::nullopt;
		}

	template <bool view_a> template <bool view_b>
	bool circle<view_a>::contains(const segment<view_b>& other) const noexcept
		{
		return contains(other.a) && contains(other.b);
		}

	template <bool view_a> template <bool view_b> vec2f                circle<view_a>::closest_point_to(const aabb<view_b>& other) const noexcept { return {}; } //TODO
	template <bool view_a> template <bool view_b> vec2f                circle<view_a>::distance_min    (const aabb<view_b>& other) const noexcept { return {}; } //TODO
	template <bool view_a> template <bool view_b> bool                 circle<view_a>::intersects      (const aabb<view_b>& other) const noexcept { return {}; } //TODO
	template <bool view_a> template <bool view_b> std::optional<vec2f> circle<view_a>::intersection    (const aabb<view_b>& other) const noexcept { return {}; } //TODO
	template <bool view_a> template <bool view_b> bool                 circle<view_a>::contains        (const aabb<view_b>& other) const noexcept { return {}; } //TODO

	template <bool view_a> template <bool view_b> vec2f                circle<view_a>::closest_point_to(const polygon<view_b>& other) const noexcept { return {}; } //TODO
	template <bool view_a> template <bool view_b> vec2f                circle<view_a>::distance_min    (const polygon<view_b>& other) const noexcept { return {}; } //TODO
	template <bool view_a> template <bool view_b> bool                 circle<view_a>::intersects      (const polygon<view_b>& other) const noexcept { return {}; } //TODO
	template <bool view_a> template <bool view_b> std::optional<vec2f> circle<view_a>::intersection    (const polygon<view_b>& other) const noexcept { return {}; } //TODO
	template <bool view_a> template <bool view_b> bool                 circle<view_a>::contains        (const polygon<view_b>& other) const noexcept { return {}; } //TODO

	template <bool view_a> template <bool view_b> vec2f                circle<view_a>::closest_point_to(const circle<view_b>& other) const noexcept { return {}; } //TODO
	template <bool view_a> template <bool view_b> vec2f                circle<view_a>::distance_min    (const circle<view_b>& other) const noexcept { return {}; } //TODO
	template <bool view_a> template <bool view_b> bool                 circle<view_a>::intersects      (const circle<view_b>& other) const noexcept { return {}; } //TODO
	template <bool view_a> template <bool view_b> std::optional<vec2f> circle<view_a>::intersection    (const circle<view_b>& other) const noexcept { return {}; } //TODO
	template <bool view_a> template <bool view_b> bool                 circle<view_a>::contains        (const circle<view_b>& other) const noexcept { return {}; } //TODO
	}