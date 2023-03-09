#pragma once

#include "inner_shapes.h"

namespace utils::math::geometry
	{
	                        vec2f point::closest_point_to(const point                   & b) const noexcept { return *this; }
	                        vec2f point::closest_point_to(const segment                 & b) const noexcept { return *this; }
	template<bool hollow_b> vec2f point::closest_point_to(const aabb          <hollow_b>& b) const noexcept { return *this; }
	template<bool hollow_b> vec2f point::closest_point_to(const polygon       <hollow_b>& b) const noexcept { return *this; }
	template<bool hollow_b> vec2f point::closest_point_to(const convex_polygon<hollow_b>& b) const noexcept { return *this; }
	template<bool hollow_b> vec2f point::closest_point_to(const circle        <hollow_b>& b) const noexcept { return *this; }
	template<bool hollow_b> vec2f point::closest_point_to(const capsule       <hollow_b>& b) const noexcept { return *this; }

	                        bool point::contains(const point                   & b) const noexcept { return *this == b; }
	                        bool point::contains(const segment                 & b) const noexcept { return *this == b.a  && *this == b.b; }
	template<bool hollow_b> bool point::contains(const aabb          <hollow_b>& b) const noexcept { return *this == b.ul && *this == b.ur && vertex == b.dr && vertex == b.dl; }
	template<bool hollow_b> bool point::contains(const polygon       <hollow_b>& b) const noexcept { for (const auto& vertex : b.get_vertices()) { if (vertex != *this) { return false; } } return true; }
	template<bool hollow_b> bool point::contains(const convex_polygon<hollow_b>& b) const noexcept { for (const auto& vertex : b.get_vertices()) { if (vertex != *this) { return false; } } return true; }
	template<bool hollow_b> bool point::contains(const circle        <hollow_b>& b) const noexcept { return *this == b.center && b.radius = 0; }
	template<bool hollow_b> bool point::contains(const capsule       <hollow_b>& b) const noexcept { return false; } //TODO
	}