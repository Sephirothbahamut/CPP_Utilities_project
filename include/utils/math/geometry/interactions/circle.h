#pragma once
#include "../shapes.h"
#include "../../../compilation/warnings.h"

namespace utils::math::geometry
	{
	inline vec2f circle::closest_point_to(const point& other) const noexcept
		{
		vec2f vec{vec2f{other} - vec2f{center}};
		vec.length = radius;
		return vec;
		}

	inline bool circle::contains(const point& other) const noexcept
		{
		return point{utils::math::vec2f{center}}.distance_min(other) <= radius;
		}
	
	inline vec2f circle::closest_point_to(const segment& b) const noexcept
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
		
	inline bool circle::intersects(const segment& other) const noexcept
		{
		if ( contains(other.a) && !contains(other.b)) { return true; }
		if ( contains(other.b) && !contains(other.a)) { return true; }
		if (!contains(other.b) && !contains(other.a)) { return other.distance_min(center) <= radius; }
		return false;
		}
		
	inline std::optional<vec2f> circle::intersection_with(const segment& other) const noexcept
		{
		//TODO
		return std::nullopt;
		}
		
	inline bool circle::contains(const segment& other) const noexcept
		{
		return contains(other.a) && contains(other.b);
		}

	//TODO someday
	utils_disable_warnings_begin
	utils_disable_warning_msvc(4100)

	inline vec2f                circle::closest_point_to(const aabb& other) const noexcept { return {}; } //TODO
	inline vec2f                circle::distance_min    (const aabb& other) const noexcept { return {}; } //TODO
	inline bool                 circle::intersects      (const aabb& other) const noexcept { return {}; } //TODO
	inline std::optional<vec2f> circle::intersection_with    (const aabb& other) const noexcept { return {}; } //TODO
	inline bool                 circle::contains        (const aabb& other) const noexcept { return {}; } //TODO
	
	inline vec2f                circle::closest_point_to(const polygon& other) const noexcept { return {}; } //TODO
	inline vec2f                circle::distance_min    (const polygon& other) const noexcept { return {}; } //TODO
	inline bool                 circle::intersects      (const polygon& other) const noexcept { return {}; } //TODO
	inline std::optional<vec2f> circle::intersection_with    (const polygon& other) const noexcept { return {}; } //TODO
	inline bool                 circle::contains        (const polygon& other) const noexcept { return {}; } //TODO
	
	inline vec2f                circle::closest_point_to(const circle& other) const noexcept { return {}; } //TODO
	inline vec2f                circle::distance_min    (const circle& other) const noexcept { return {}; } //TODO
	inline bool                 circle::intersects      (const circle& other) const noexcept { return {}; } //TODO
	inline std::optional<vec2f> circle::intersection_with    (const circle& other) const noexcept { return {}; } //TODO
	inline bool                 circle::contains        (const circle& other) const noexcept { return {}; } //TODO
	utils_disable_warnings_end
	}