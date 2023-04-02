#pragma once
#include "../shapes.h"

namespace utils::math::geometry
	{
	inline closest_point_and_distance_t polygon::closest_point_and_distance(const point& other) const noexcept
		{
		closest_point_and_distance_t best;
		for (const auto& edge : get_edges())
			{
			closest_point_and_distance_t tmp{segment{edge}.closest_point_and_distance(other)};
			if (tmp.distance < best.distance) { best = tmp; }
			}
		return best;
		}

	inline bool polygon::intersects(const point& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (segment{edge}.intersects(other)) { return true; }
			}
		return false;
		}

	inline std::optional<vec2f> polygon::intersection(const point& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (auto opt{segment{edge}.intersection(other)}) { return opt; }
			}
		return std::nullopt;
		}
	inline bool polygon::contains(const point& other) const noexcept
		{
		// A point is inside a polygon if given a line in any direction, it intersects the polygon segments an uneven number of times
		segment tmp_segment{other, vec2f{other.x + bounding_box().right, other.y}}; //A segment which lies on a generic horizontal line

		bool is_inside = false;

		const auto& vertices{get_vertices()};
		for (size_t i{0}; i < vertices.size(); i++)
			{
			const auto& a{vertices[i]};
			const auto& b{vertices[(i + 1) % vertices.size()]};

			//no intersection
			if ((a.y < other.y && b.y < other.y) || (a.y > other.y && b.y > other.y)) { continue; }
			if (a.x < other.x && b.x < other.x) { continue; }
			if (!segment{a, b}.intersects(tmp_segment)) { continue; } //TODO optimize for tmp_segment being an horizontal line to infinity

			if (b.y == other.y) // If vertex b is on line
				{
				const auto& c{vertices[(i + 2) % vertices.size()]};

				if (tmp_segment.point_side(a) != tmp_segment.point_side(c)) { is_inside = !is_inside; }
				}
			else if (a.y != other.y) // If edge intersects line (excluding vertex a)
				{
				is_inside = !is_inside;
				}
			}
		return is_inside;
		}

	inline bool convex_polygon::contains(const point& other) const noexcept
		{
		for (const auto& edge : polygon::get_edges())
			{
			if (segment{edge}.point_side(other) != side_t::right) { return false; }
			}
		return true;
		}

	inline closest_point_and_distance_t polygon::closest_point_and_distance(const segment& other) const noexcept
		{
		closest_point_and_distance_t best;
		for (const auto& edge : get_edges())
			{
			closest_point_and_distance_t tmp{segment{edge}.closest_point_and_distance(other)};
			if (tmp.distance < best.distance) { best = tmp; }
			}
		return best;
		}

	inline bool polygon::intersects(const segment& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (segment{edge}.intersects(other)) { return true; }
			}
		return false;
		}
	inline std::optional<vec2f> polygon::intersection(const segment& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (auto opt{segment{edge}.intersection(other)}) { return opt; }
			}
		return std::nullopt;
		}
	inline bool polygon::contains(const segment& other) const noexcept { return contains(other.a) && contains(other.b); }

	inline closest_point_and_distance_t polygon::closest_point_and_distance(const aabb& other) const noexcept
		{
		closest_point_and_distance_t best;
		for (const auto& edge : get_edges())
			{
			closest_point_and_distance_t tmp{segment{edge}.closest_point_and_distance(other)};
			if (tmp.distance < best.distance) { best = tmp; }
			}
		return best;
		}
	inline bool polygon::intersects(const aabb& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (segment{edge}.intersects(other)) { return true; }
			}
		return false;
		}
	inline std::optional<vec2f> polygon::intersection(const aabb& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (auto opt{segment{edge}.intersection(other)}) { return opt; }
			}
		return std::nullopt;
		}
	inline bool polygon::contains(const aabb& other) const noexcept
		{
		return contains(other.ul()) 
			&& contains(other.ur()) 
			&& contains(other.dr()) 
			&& contains(other.dl())
			&& !intersects(other);
		}
	inline bool convex_polygon::contains(const aabb& other) const noexcept
		{
		return contains(other.ul())
			&& contains(other.ur())
			&& contains(other.dr())
			&& contains(other.dl());
		}

	inline closest_point_and_distance_t polygon::closest_point_and_distance(const polygon& other) const noexcept
		{
		closest_point_and_distance_t best;
		for (const auto& edge : get_edges())
			{
			closest_point_and_distance_t tmp{segment{edge}.closest_point_and_distance(other)};
			if (tmp.distance < best.distance) { best = tmp; }
			}
		return best;
		}
	inline bool polygon::intersects(const polygon& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (segment{edge}.intersects(other)) { return true; }
			}
		return false;
		}
	inline std::optional<vec2f> polygon::intersection(const polygon& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (auto opt{segment{edge}.intersection(other)}) { return opt; }
			}
		return std::nullopt;
		}
	inline bool polygon::contains(const polygon& other) const noexcept
		{
		for (const auto& vertex : other.get_vertices())
			{
			if (!contains(vertex)) { return false; }
			}
		return !intersects(other);
		}
	inline bool convex_polygon::contains(const polygon& other) const noexcept
		{
		for (const auto& vertex : other.get_vertices())
			{
			if (!contains(vertex)) { return false; }
			}
		return true;
		}

	inline closest_point_and_distance_t polygon::closest_point_and_distance(const circle& other) const noexcept
		{
		closest_point_and_distance_t best;
		for (const auto& edge : get_edges())
			{
			closest_point_and_distance_t tmp{segment{edge}.closest_point_and_distance(other)};
			if (tmp.distance < best.distance) { best = tmp; }
			}
		return best;
		}
	inline bool polygon::intersects(const circle& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (segment{edge}.intersects(other)) { return true; }
			}
		return false;
		}
	inline std::optional<vec2f> polygon::intersection(const circle& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (auto opt{segment{edge}.intersection(other)}) { return opt; }
			}
		return std::nullopt;
		}

	inline bool polygon::contains(const circle& other) const noexcept
		{
		return contains(other.center) && (distance_min(other.center) < other.radius);
		}
	}