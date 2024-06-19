#pragma once
#include "../shapes.h"

namespace utils::math
	{
	template <typename T>
	vec2f rect<T>::closest_point_to(const geometry::point& other) const noexcept
		{
		if(!contains(other))
			{
			return
				{
				other.x < ll ? ll : rr,
				other.y < up ? up : dw,
				};
			}
		else
			{
			vec2f closest_on_axis
				{
				other.x < (ll + ((rr - ll) / 2.f)) ? ll : rr,
				other.y < (up + ((dw - up) / 2.f)) ? up : dw
				};

			vec2f dist_on_axis{std::abs(closest_on_axis.x - other.x), std::abs(closest_on_axis.y - other.y)};
			if (dist_on_axis.x < dist_on_axis.y)
				{
				return {closest_on_axis.x, other.y};
				}
			else
				{
				return {other.x, closest_on_axis.y};
				}
			}
		}

	template <typename T>
	float rect<T>::distance_min(const geometry::point& other) const noexcept
		{
		if (other.x <= ll)
			{
			if (other.y < up) { return vec2f::distance(other, ul()); }
			if (other.y > dw) { return vec2f::distance(other, dl()); }
			return ll - other.x;
			}
		if (other.x >= rr)
			{
			if (other.y < up) { return vec2f::distance(other, ur()); }
			if (other.y > dw) { return vec2f::distance(other, dr()); }
			return other.x - rr;
			}
		if (other.y <= up)
			{
			return up - other.y;
			}
		if (other.y >= dw)
			{
			return other.y - dw;
			}
		else
			{
			vec2f closest_on_axis
				{
				other.x < (ll + ((rr - ll) / 2.f)) ? ll : rr,
				other.y < (up + ((dw - up) / 2.f)) ? up : dw
				};

			vec2f dist_on_axis{utils::math::abs(other - closest_on_axis)};
			return utils::math::min(std::abs(dist_on_axis.x), std::abs(dist_on_axis.y));
			}
		}

	template <typename T>
	bool rect<T>::intersects(const geometry::point& other) const noexcept
		{
		return (other.x == ll && other.y < dw && other.y > up)
			|| (other.x == rr && other.y < dw && other.y > up)
			|| (other.y == up && other.x < rr && other.x > ll)
			|| (other.y == dw && other.x < rr && other.x > ll);
		}

	template <typename T>
	std::optional<vec2f> rect<T>::intersection_with(const geometry::point& other) const noexcept
		{
		if (intersects(other)) { return other; }
		}

	//template <typename T>
	//bool rect<T>::contains(const geometry::point& other) const noexcept
	//	{
	//	return other.x >= ll && other.x <= rr && other.y >= up && other.y <= dw;
	//	}

	//TODO From here on I copied polygon functions that rely on get_edges(), should be possible to optimize

	template <typename T>
	inline geometry::closest_point_and_distance_t rect<T>::closest_point_and_distance(const geometry::segment& other) const noexcept
		{
		geometry::closest_point_and_distance_t best;
		for (const auto& edge : get_edges())
			{
			geometry::closest_point_and_distance_t tmp{geometry::segment{edge}.closest_point_and_distance(other)};
			if (tmp.distance < best.distance) { best = tmp; }
			}
		return best;
		}

	template <typename T>
	inline bool rect<T>::intersects(const geometry::segment& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (geometry::segment{edge}.intersects(other)) { return true; }
			}
		return false;
		}
	template <typename T>
	inline std::optional<vec2f> rect<T>::intersection_with(const geometry::segment& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (auto opt{geometry::segment{edge}.intersection_with(other)}) { return opt; }
			}
		return std::nullopt;
		}
	template <typename T>
	inline bool rect<T>::contains(const geometry::segment& other) const noexcept { return contains(other.a) && contains(other.b); }

	template <typename T>
	inline geometry::closest_point_and_distance_t rect<T>::closest_point_and_distance(const geometry::aabb& other) const noexcept
		{
		geometry::closest_point_and_distance_t best;
		for (const auto& edge : get_edges())
			{
			geometry::closest_point_and_distance_t tmp{geometry::segment{edge}.closest_point_and_distance(other)};
			if (tmp.distance < best.distance) { best = tmp; }
			}
		return best;
		}
	template <typename T>
	inline bool rect<T>::intersects(const geometry::aabb& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (geometry::segment{edge}.intersects(other)) { return true; }
			}
		return false;
		}
	template <typename T>
	inline std::optional<vec2f> rect<T>::intersection_with(const geometry::aabb& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (auto opt{geometry::segment{edge}.intersection_with(other)}) { return opt; }
			}
		return std::nullopt;
		}
	template <typename T>
	inline bool rect<T>::contains(const geometry::aabb& other) const noexcept
		{
		return contains(other.ul())
			&& contains(other.ur())
			&& contains(other.dr())
			&& contains(other.dl());
			//&& !intersects(other); //TODO understand why I added this, if no reason, delete
		}

	template <typename T>
	inline geometry::closest_point_and_distance_t rect<T>::closest_point_and_distance(const geometry::polygon& other) const noexcept
		{
		geometry::closest_point_and_distance_t best;
		for (const auto& edge : get_edges())
			{
			geometry::closest_point_and_distance_t tmp{geometry::segment{edge}.closest_point_and_distance(other)};
			if (tmp.distance < best.distance) { best = tmp; }
			}
		return best;
		}
	template <typename T>
	inline bool rect<T>::intersects(const geometry::polygon& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (geometry::segment{edge}.intersects(other)) { return true; }
			}
		return false;
		}
	template <typename T>
	inline std::optional<vec2f> rect<T>::intersection_with(const geometry::polygon& other) const noexcept
		{
		for (const auto& edge : get_edges())
			{
			if (auto opt{geometry::segment{edge}.intersection_with(other)}) { return opt; }
			}
		return std::nullopt;
		}
	template <typename T>
	inline bool rect<T>::contains(const geometry::polygon& other) const noexcept
		{
		for (const auto& vertex : other.get_vertices())
			{
			if (!contains(vertex)) { return false; }
			}
		return !intersects(other);
		}
	}