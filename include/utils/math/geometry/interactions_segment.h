#pragma once

#include <algorithm>

#include "inner_shapes.h"

namespace utils::math::geometry
	{
	vec2f segment::closest_point_to(const point& other) const noexcept
		{//http://csharphelper.com/blog/2016/09/find-the-shortest-distance-between-a-point-and-a-line-segment-in-c/
		vec2f delta = {b.x - a.x, b.y - a.y};

		float t = ((other.x - a.x) * delta.x + (other.y - a.y) * delta.y) / (delta.x * delta.x + delta.y * delta.y);

		return t < 0 ? a : t > 1 ? b : vec2f{a.x + t * delta.x, a.y + t * delta.y};
		}

	bool segment::contains(const point& other) const noexcept
		{
		// https://stackoverflow.com/questions/17692922/check-is-a-point-x-y-is-between-two-points-drawn-on-a-straight-line
		auto dist{distance_min(other)};
		return dist == 0;
		}

	vec2f segment::closest_point_to(const segment& other) const noexcept
		{
		point closest_to_a{closest_point_to(point{other.a})};
		point closest_to_b{closest_point_to(point{other.b})};
		if (closest_to_a.distance_min(point{other.a}) < closest_to_b.distance_min(point{other.b}))
			{
			return closest_to_a;
			}
		else
			{
			return closest_to_b;
			}
		}

	bool segment::intersects(const segment& other) const noexcept
		{
		return intersects_line(other) && other.intersects_line(*this);
		}

	std::optional<vec2f> segment::intersection(const segment& other) const noexcept
		{//https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
		vec2f s1, s2;
		s1.x = b.x - a.x;
		s1.y = b.y - a.y;
		s2.x = other.b.x - other.a.x;
		s2.y = other.b.y - other.a.y;

		float s, t;
		s = (-s1.y * (a.x - other.a.x) + s1.x * (a.y - other.a.y)) / (-s2.x * s1.y + s1.x * s2.y);
		t = ( s2.x * (a.y - other.a.y) - s2.y * (a.x - other.a.x)) / (-s2.x * s1.y + s1.x * s2.y);

		if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
			{
			return vec2f{a.x + (t * s1.x), a.y + (t * s1.y)};
			}

		return std::nullopt;
		}

	template<bool hollow_b> 
	vec2f segment::closest_point_to(const aabb<hollow_b>& other) const noexcept
		{
		point ret{closest_point_to(segment{other.ul, other.ur})};
		float dist_min{ret.distance_min(segment{other.ul, other.ur})};

		if (point pt{closest_point_to(segment{other.ur, other.dr})}) { if (pt.distance_min(segment{other.ur, other.dr}) < min_dist) { min_dist = dist; ret = pt; } }
		if (point pt{closest_point_to(segment{other.dr, other.dl})}) { if (pt.distance_min(segment{other.dr, other.dl}) < min_dist) { min_dist = dist; ret = pt; } }
		if (point pt{closest_point_to(segment{other.dl, other.ul})}) { if (pt.distance_min(segment{other.dl, other.ul}) < min_dist) { min_dist = dist; ret = pt; } }

		return pt;
		}

	template<bool hollow_b> 
	vec2f segment::distance_min(const aabb<hollow_b>& other) const noexcept
		{
		float dist_min{distance_min(segment{other.ul, other.ur})};
		if (float dist{distance_min(segment{other.ur, other.dr})}) { if (dist < min_dist) { min_dist = dist; } }
		if (float dist{distance_min(segment{other.dr, other.dl})}) { if (dist < min_dist) { min_dist = dist; } }
		if (float dist{distance_min(segment{other.dl, other.ul})}) { if (dist < min_dist) { min_dist = dist; } }
		return dist_min;
		}

	template<bool hollow_b> 
	bool segment::intersects(const aabb<hollow_b>& other) const noexcept
		{
		return intersects(segment{other.ul, other.ur}) || intersects(segment{other.ur, other.dr}) || intersects(segment{other.dr, other.dl}) || intersects(segment{other.dl, other.ul});
		}

	template<bool hollow_b>
	std::optional<vec2f> segment::intersection(const aabb<hollow_b>& other) const noexcept
		{//https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
		if (auto opt{intersection(segment{other.ul, other.ur})}) { return opt; }
		if (auto opt{intersection(segment{other.ur, other.dr})}) { return opt; }
		if (auto opt{intersection(segment{other.dr, other.dl})}) { return opt; }
		return       intersection(segment{other.dl, other.ul});
		}
	
	template<bool hollow_b>
	bool segment::contains(const aabb<hollow_b>& other) const noexcept
		{//https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
		return contains(other.ul) && contains(other.ur) && contains(other.dr) && contains(other.dl);
		}
	
	template<bool hollow_b> 
	vec2f segment::closest_point_to(const polygon<hollow_b>& other) const noexcept
		{
		point ret;
		float dist_min{constants::finf};

		for (const auto& edge : other.get_edges())
			{
			point closest{closest_point_to(edge)};
			float dist{closest.distance_min(edge)};
			if (dist < dist_min) { dist_min = dist; ret = closest; }
			}

		return ret;
		}

	template<bool hollow_b> 
	vec2f segment::distance_min(const polygon<hollow_b>& other) const noexcept
		{
		float dist_min{constants::finf};

		for (const auto& edge : other.get_edges())
			{
			point closest{closest_point_to(edge)};
			float dist{closest.distance_min(edge)};
			if (dist < dist_min) { dist_min = dist; }
			}

		return dist_min;
		}

	template<bool hollow_b> 
	bool segment::intersects(const polygon<hollow_b>& other) const noexcept
		{
		for (const auto& edge : other.get_edges())
			{
			if (intersects(edge)) { return true; }
			}
		return false;
		}

	template<bool hollow_b>
	std::optional<vec2f> segment::intersection(const polygon<hollow_b>& other) const noexcept
		{
		for (const auto& edge : other.get_edges())
			{
			if (auto opt{intersection(edge)}) { return opt; }
			}
		return std::nullopt;
		}
	
	template<bool hollow_b>
	bool segment::contains(const polygon<hollow_b>& other) const noexcept
		{//https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
		for (const auto& vertex : other.get_vertices())
			{
			if (!contains(vertex)) { return false; }
			}
		return true;
		}
	
	template<bool hollow_b> 
	vec2f segment::closest_point_to(const circle<hollow_b>& other) const noexcept
		{
		//TODO
		return {};
		}

	template<bool hollow_b> 
	vec2f segment::distance_min(const circle<hollow_b>& other) const noexcept
		{
		//TODO
		return {};
		}

	template<bool hollow_b> 
	bool segment::intersects(const circle<hollow_b>& other) const noexcept
		{
		return distance_min(other.center) < other.radius;
		}

	template<bool hollow_b>
	std::optional<vec2f> segment::intersection(const circle<hollow_b>& other) const noexcept
		{
		//TODO
		return std::nullopt;
		}
	
	template<bool hollow_b>
	bool segment::contains(const circle<hollow_b>& other) const noexcept
		{
		return other.radius == 0 && contains(other.center);
		}
	}