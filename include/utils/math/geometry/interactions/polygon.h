#pragma once
#include "../shapes.h"

namespace utils::math::geometry
	{
	template <bool view_a> template <bool view_b>
	closest_point_and_distance_t polygon<view_a>::closest_point_and_distance(const point<view_b>& other) const noexcept
		{
		closest_point_and_distance_t best;
		for (const auto& edge : get_edges())
			{
			closest_point_and_distance_t tmp{edge.closest_point_and_distance(other)};
			if (tmp.distance < best.distance) { best = tmp; }
			}
		return best;
		}

	template <bool view_a> template <bool view_b>
	bool polygon<view_a>::intersects(const point<view_b>& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (edge.intersects(other)) { return true; }
			}
		return false;
		}

	template <bool view_a> template <bool view_b>
	std::optional<vec2f> polygon<view_a>::intersection(const point<view_b>& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (auto opt{edge.intersection(other)}) { return opt; }
			}
		return std::nullopt;
		}
	template <bool view_a> template <bool view_b>
	bool polygon<view_a>::contains(const point<view_b>& other) const noexcept
		{
		// A point<view_b> is inside a polygon<view_b> if given a line in any direction, it intersects the polygon<view_b> segments an uneven number of times
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

	template <bool view_a> template <bool view_b>
	bool convex_polygon<view_a>::contains(const point<view_b>& other) const noexcept
		{
		for (const auto& edge : polygon<view_a>::get_edges())
			{
			if (edge.point_side(other) != side_t::right) { return false; }
			}
		return true;
		}

	template <bool view_a> template <bool view_b>
	closest_point_and_distance_t polygon<view_a>::closest_point_and_distance(const segment<view_b>& other) const noexcept
		{
		closest_point_and_distance_t best;
		for (const auto& edge : get_edges())
			{
			closest_point_and_distance_t tmp{edge.closest_point_and_distance(other)};
			if (tmp.distance < best.distance) { best = tmp; }
			}
		return best;
		}

	template <bool view_a> template <bool view_b>
	bool polygon<view_a>::intersects(const segment<view_b>& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (edge.intersects(other)) { return true; }
			}
		return false;
		}
	template <bool view_a> template <bool view_b>
	std::optional<vec2f> polygon<view_a>::intersection(const segment<view_b>& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (auto opt{edge.intersection(other)}) { return opt; }
			}
		return std::nullopt;
		}
	template <bool view_a> template <bool view_b>
	bool polygon<view_a>::contains(const segment<view_b>& other) const noexcept { return contains(other.a) && contains(other.b); }

	template <bool view_a> template <bool view_b>
	closest_point_and_distance_t polygon<view_a>::closest_point_and_distance(const aabb<view_b>& other) const noexcept
		{
		closest_point_and_distance_t best;
		for (const auto& edge : get_edges())
			{
			closest_point_and_distance_t tmp{edge.closest_point_and_distance(other)};
			if (tmp.distance < best.distance) { best = tmp; }
			}
		return best;
		}
	template <bool view_a> template <bool view_b>
	bool polygon<view_a>::intersects(const aabb<view_b>& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (edge.intersects(other)) { return true; }
			}
		return false;
		}
	template <bool view_a> template <bool view_b>
	std::optional<vec2f> polygon<view_a>::intersection(const aabb<view_b>& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (auto opt{edge.intersection(other)}) { return opt; }
			}
		return std::nullopt;
		}
	template <bool view_a> template <bool view_b>
	bool polygon<view_a>::contains(const aabb<view_b>& other) const noexcept 
		{
		return contains(other.ul()) 
			&& contains(other.ur()) 
			&& contains(other.dr()) 
			&& contains(other.dl())
			&& !intersects(other);
		}
	template <bool view_a> template <bool view_b>
	bool convex_polygon<view_a>::contains(const aabb<view_b>& other) const noexcept
		{
		return contains(other.ul())
			&& contains(other.ur())
			&& contains(other.dr())
			&& contains(other.dl());
		}

	template <bool view_a> template <bool view_b>
	closest_point_and_distance_t polygon<view_a>::closest_point_and_distance(const polygon<view_b>& other) const noexcept
		{
		closest_point_and_distance_t best;
		for (const auto& edge : get_edges())
			{
			closest_point_and_distance_t tmp{edge.closest_point_and_distance(other)};
			if (tmp.distance < best.distance) { best = tmp; }
			}
		return best;
		}
	template <bool view_a> template <bool view_b>
	bool polygon<view_a>::intersects(const polygon<view_b>& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (edge.intersects(other)) { return true; }
			}
		return false;
		}
	template <bool view_a> template <bool view_b>
	std::optional<vec2f> polygon<view_a>::intersection(const polygon<view_b>& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (auto opt{edge.intersection(other)}) { return opt; }
			}
		return std::nullopt;
		}
	template <bool view_a> template <bool view_b>
	bool polygon<view_a>::contains(const polygon<view_b>& other) const noexcept
		{
		for (const auto& vertex : other.get_vertices())
			{
			if (!contains(vertex)) { return false; }
			}
		return !intersects(other);
		}
	template <bool view_a> template <bool view_b>
	bool convex_polygon<view_a>::contains(const polygon<view_b>& other) const noexcept
		{
		for (const auto& vertex : other.get_vertices())
			{
			if (!contains(vertex)) { return false; }
			}
		return true;
		}

	template <bool view_a> template <bool view_b>
	closest_point_and_distance_t polygon<view_a>::closest_point_and_distance(const circle<view_b>& other) const noexcept
		{
		closest_point_and_distance_t best;
		for (const auto& edge : get_edges())
			{
			closest_point_and_distance_t tmp{edge.closest_point_and_distance(other)};
			if (tmp.distance < best.distance) { best = tmp; }
			}
		return best;
		}
	template <bool view_a> template <bool view_b>
	bool polygon<view_a>::intersects(const circle<view_b>& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (edge.intersects(other)) { return true; }
			}
		return false;
		}
	template <bool view_a> template <bool view_b>
	std::optional<vec2f> polygon<view_a>::intersection(const circle<view_b>& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (auto opt{edge.intersection(other)}) { return opt; }
			}
		return std::nullopt;
		}

	template <bool view_a> template <bool view_b>
	bool polygon<view_a>::contains(const circle<view_b>& other) const noexcept
		{
		return contains(other.center) && (distance_min(other.center) < other.radius);

		}
	}