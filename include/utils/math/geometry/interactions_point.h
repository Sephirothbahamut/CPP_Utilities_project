#pragma once

#include "inner_shapes.h"

namespace utils::math::geometry
	{
	vec2f point::closest_point_to(const point         & b) const noexcept { return *this; }
	vec2f point::closest_point_to(const segment       & b) const noexcept { return *this; }
	vec2f point::closest_point_to(const aabb          & b) const noexcept { return *this; }
	vec2f point::closest_point_to(const polygon       & b) const noexcept { return *this; }
	vec2f point::closest_point_to(const convex_polygon& b) const noexcept { return *this; }
	vec2f point::closest_point_to(const circle        & b) const noexcept { return *this; }
	vec2f point::closest_point_to(const capsule       & b) const noexcept { return *this; }
	float point::distance_min    (const point         & b) const noexcept { return vec2f::distance(*this, b); }
	float point::distance_min    (const segment       & b) const noexcept { return distance_min(b.closest_point_to(*this)); }
	float point::distance_min    (const aabb          & b) const noexcept { return distance_min(b.closest_point_to(*this)); }
	float point::distance_min    (const polygon       & b) const noexcept { return distance_min(b.closest_point_to(*this)); }
	float point::distance_min    (const convex_polygon& b) const noexcept { return distance_min(b.closest_point_to(*this)); }
	float point::distance_min    (const circle        & b) const noexcept { return distance_min(b.closest_point_to(*this)); }
	//float point::distance_min    (const capsule       & b) const noexcept { return distance_min(b.closest_point_to(*this)); }

	bool point::contains(const point         & b) const noexcept { return *this == b; }
	bool point::contains(const segment       & b) const noexcept { return *this == b.a  && *this == b.b; }
	bool point::contains(const aabb          & b) const noexcept { return *this == b.ul() && *this == b.ur() && *this == b.dr() && *this == b.dl(); }
	bool point::contains(const polygon       & b) const noexcept { for (const auto& vertex : b.get_vertices()) { if (vertex != *this) { return false; } } return true; }
	bool point::contains(const convex_polygon& b) const noexcept { for (const auto& vertex : b.get_vertices()) { if (vertex != *this) { return false; } } return true; }
	bool point::contains(const circle        & b) const noexcept { return (*this == b.center) && (b.radius == 0); }
	bool point::contains(const capsule       & b) const noexcept { return false; } //TODO
	}