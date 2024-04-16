#pragma once
#include "../shapes.h"

namespace utils::math::geometry
	{
	inline vec2f segment::closest_point_to(const point& other) const noexcept
		{//http://csharphelper.com/blog/2016/09/find-the-shortest-distance-between-a-point-and-a-line-segment-in-c/
		vec2f delta = {b.x - a.x, b.y - a.y};

		float t = ((other.x - a.x) * delta.x + (other.y - a.y) * delta.y) / (delta.x * delta.x + delta.y * delta.y);

		return t < 0 ? a : t > 1 ? b : vec2f{a.x + t * delta.x, a.y + t * delta.y};
		}
	inline float segment::distance_min(const point& other) const noexcept { return other.distance_min(closest_point_to(other)); }

	inline bool segment::contains(const point& other) const noexcept
		{
		// https://stackoverflow.com/questions/17692922/check-is-a-point-x-y-is-between-two-points-drawn-on-a-straight-line
		auto dist{distance_min(other)};
		return dist == 0;
		}

	inline closest_point_and_distance_t segment::closest_point_and_distance(const segment& other) const noexcept
		{
		point closest_to_a{closest_point_to(point{other.a})};
		point closest_to_b{closest_point_to(point{other.b})};
		float distance_a{closest_to_a.distance_min(point{other.a})};
		float distance_b{closest_to_b.distance_min(point{other.b})};
		if (distance_a < distance_b)
			{
			return {closest_to_a, distance_a};
			}
		else
			{
			return {closest_to_b, distance_b};
			}
		}

	inline bool segment::intersects(const segment& other) const noexcept
		{
		return intersects_line(other) && other.intersects_line(*this);
		}

	inline std::optional<vec2f> segment::intersection_with(const segment& other) const noexcept
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

	inline closest_point_and_distance_t segment::closest_point_and_distance(const aabb& other) const noexcept
		{
		point ret{closest_point_to(segment{other.ul(), other.ur()})};
		float dist_min{ret.distance_min(segment{other.ul(), other.ur()})};

		if (true) { point pt{closest_point_to(segment{other.ur(), other.dr()})}; auto dist{pt.distance_min(segment{other.ur(), other.dr()})}; if (dist < dist_min) { dist_min = dist; ret = pt; } }
		if (true) { point pt{closest_point_to(segment{other.dr(), other.dl()})}; auto dist{pt.distance_min(segment{other.dr(), other.dl()})}; if (dist < dist_min) { dist_min = dist; ret = pt; } }
		if (true) { point pt{closest_point_to(segment{other.dl(), other.ul()})}; auto dist{pt.distance_min(segment{other.dl(), other.ul()})}; if (dist < dist_min) { dist_min = dist; ret = pt; } }

		return {ret, dist_min};
		}

	inline bool segment::intersects(const aabb& other) const noexcept
		{
		return intersects(segment{other.ul(), other.ur()}) || intersects(segment{other.ur(), other.dr()}) || intersects(segment{other.dr(), other.dl()}) || intersects(segment{other.dl(), other.ul()});
		}

	inline std::optional<vec2f> segment::intersection_with(const aabb& other) const noexcept
		{//https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
		if (auto opt{intersection_with(segment{other.ul(), other.ur()})}) { return opt; }
		if (auto opt{intersection_with(segment{other.ur(), other.dr()})}) { return opt; }
		if (auto opt{intersection_with(segment{other.dr(), other.dl()})}) { return opt; }
		return       intersection_with(segment{other.dl(), other.ul()});
		}

	inline bool segment::contains(const aabb& other) const noexcept
		{//https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
		return contains(other.ul()) && contains(other.ur()) && contains(other.dr()) && contains(other.dl());
		}

	inline closest_point_and_distance_t segment::closest_point_and_distance(const polygon& other) const noexcept
		{
		point ret;
		float dist_min{constants::finf};

		for (const auto& edge : other.get_edges())
			{
			point closest{closest_point_to(segment{edge})};
			float dist{closest.distance_min(segment{edge})};
			if (dist < dist_min) { dist_min = dist; ret = closest; }
			}

		return {ret, dist_min};
		}

	inline bool segment::intersects(const polygon& other) const noexcept
		{
		for (const auto& edge : other.get_edges())
			{
			if (intersects(segment{edge})) { return true; }
			}
		return false;
		}

	inline std::optional<vec2f> segment::intersection_with(const polygon& other) const noexcept
		{
		for (const auto& edge : other.get_edges())
			{
			if (auto opt{intersection_with(segment{edge})}) { return opt; }
			}
		return std::nullopt;
		}

	inline bool segment::contains(const polygon& other) const noexcept
		{//https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
		for (const auto& vertex : other.get_vertices())
			{
			if (!contains(vertex)) { return false; }
			}
		return true;
		}

	inline closest_point_and_distance_t segment::closest_point_and_distance(const circle& other) const noexcept
		{
		//TODO
		return {};
		}

	inline bool segment::intersects(const circle& other) const noexcept
		{
		return distance_min(other.center) < other.radius;
		}

	inline std::optional<vec2f> segment::intersection_with(const circle& other) const noexcept
		{
		//TODO
		return std::nullopt;
		}

	inline bool segment::contains(const circle& other) const noexcept
		{
		return other.radius == 0 && contains(other.center);
		}
	}