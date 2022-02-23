#pragma once

#include "../vec2.h"
#include "../angle.h"
#include "segment.h"
#include "polygon.h"
#include "circle.h"

namespace utils::math::geometry
	{
	enum class collision_strictness_t { strict, loose };

#pragma region Point
	#pragma region Point-point
		bool intersects(const vec2f& a, const vec2f& b) noexcept { return a == b; }
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		bool contains  (const vec2f& a, const vec2f& b) noexcept { return a == b; }
		bool collides  (const vec2f& a, const vec2f& b) noexcept { return a == b; }
	#pragma endregion Point-point

#pragma endregion Point

#pragma region Segment
	#pragma region Point-segment
			bool intersects(const vec2f& point, const segment& segment) noexcept { return segment.contains(point); }
			template <collision_strictness_t strictness = collision_strictness_t::loose>
			bool contains(const vec2f& point, const segment& segment) noexcept { return segment.contains(point); }
			bool collides(const vec2f& point, const segment& segment) noexcept { return segment.contains(point); }

			template <collision_strictness_t strictness = collision_strictness_t::loose>
			bool intersects(const segment& segment, const vec2f& point) noexcept { return intersects(point, segment); }
			template <collision_strictness_t strictness = collision_strictness_t::loose>
			bool contains(const segment& segment, const vec2f& point) noexcept { return contains(point, segment); }
			bool collides(const segment& segment, const vec2f& point) noexcept { return collides(point, segment); }
	#pragma endregion Point-segment

	#pragma region Segment-segment
		bool intersects(const segment& a, const segment& b) noexcept { return a.intersects(b); }
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		bool contains  (const segment& a, const segment& b) noexcept { return a.intersects(b); }
		bool collides  (const segment& a, const segment& b) noexcept { return a.intersects(b); }
	#pragma endregion Segment-segment
#pragma endregion Segment

#pragma region Polygon
	#pragma region Polygon-point
		bool intersects(const convex_polygon& polygon, const vec2f& point) noexcept
			{
			for (const auto& edge : polygon.get_edges())
				{
				if (intersects(edge, point)) { return true; }
				}
			return false;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		bool contains(const convex_polygon& polygon, const vec2f& point) noexcept
			{
			for (const auto& edge : polygon.get_edges())
				{
				side_t side{edge.point_side(point)};

				if constexpr (strictness == collision_strictness_t::strict)
					{
					if (side == side_t::left || side == side_t::equal) { return false; }
					}
				else
					{
					if (side == side_t::left) { return false; }
					}
				}
			return true;
			}
		bool collides(const convex_polygon& polygon, const vec2f& point) noexcept
			{
			return contains<collision_strictness_t::loose>(polygon, point);
			}
		bool intersects(const vec2f& point, const convex_polygon& polygon) noexcept { return intersects(polygon, point); }
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		bool contains  (const vec2f& point, const convex_polygon& polygon) noexcept { return contains  (polygon, point); }
		bool collides  (const vec2f& point, const convex_polygon& polygon) noexcept { return collides  (polygon, point); }
	#pragma endregion Polygon-point

	#pragma region Polygon-segment

		bool intersects(const convex_polygon& polygon, const segment& segment) noexcept
			{
			for (const auto& edge : polygon.get_edges())
				{
				if (edge.intersects(segment)) { return true; }
				}
			return false;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		bool contains(const convex_polygon& polygon, const segment& segment) noexcept
			{
			return contains<strict>(polygon, segment.a) && contains<strict>(polygon, segment.b);
			}
		bool collides(const convex_polygon& polygon, const segment& segment) noexcept { return intersects(polygon, segment) || contains<collision_strictness_t::loose>(polygon, segment); }

		//TODO dige
	#pragma endregion Polygon-segment

	#pragma region Polygon-polygon

		bool intersects(const convex_polygon& a, const convex_polygon& b) noexcept
			{
			for (const auto& edge_a : a.get_edges())
				{
				for (const auto& edge_b : b.get_edges())
					{
					if (intersects(edge_a, edge_b)) { return true; }
					}
				}
			return false;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		bool contains(const convex_polygon& a, const convex_polygon& b) noexcept
			{
			for (const auto& vertex : b.get_vertices())
				{
				if (!contains<strictness>(a, vertex)) { return false; }
				}
			return true;
			}
		bool collides(const convex_polygon& a, const convex_polygon& b) noexcept 
			{
			for (const auto& vertex : a.get_vertices())
				{
				if (collides(b, vertex)) { return true; }
				}
			for (const auto& vertex : b.get_vertices())
				{
				if (collides(a, vertex)) { return true; }
				}
			return false;
			}
	#pragma endregion Polygon-polygon
#pragma endregion Polygon

#pragma region Circle
	#pragma region Circle-point

		bool intersects(const circle& circle, const vec2f& point) noexcept
			{
			return vec2f::distance2(circle.center, point) == circle.radius * circle.radius;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		bool contains(const circle& circle, const vec2f& point) noexcept
			{
			if constexpr (strictness == collision_strictness_t::strict)
				{
				return vec2f::distance2(circle.center, point) <  (circle.radius * circle.radius);
				}
			else
				{
				return vec2f::distance2(circle.center, point) <= (circle.radius * circle.radius);
				}
			}
		bool collides(const circle& circle, const vec2f& point) noexcept
			{
			return contains<collision_strictness_t::loose>(circle, point);
			}

	//TODO dige
	#pragma endregion Circle-point

	#pragma region Circle-segment

		bool intersects(const circle& circle, const segment& segment) noexcept
			{
			return segment.minimum_distance(circle.center) <= circle.radius;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		bool contains(const circle& circle, const segment& segment) noexcept
			{
			return contains<strictness>(circle, segment.a) && contains<strictness>(circle, segment.b);
			}
		bool collides(const circle& circle, const segment& segment) noexcept
			{
			return contains<collision_strictness_t::loose>(circle, segment) || intersects(circle, segment);
			}

		//TODO dige
	#pragma endregion Circle-segment

	#pragma region Circle-polygon
		bool intersects(const circle& circle, const convex_polygon& polygon) noexcept
			{
			for (const auto& edge : polygon.get_edges())
				{
				if (intersects(circle, edge)) { return true; }
				}
			return false;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		bool contains(const circle& circle, const convex_polygon& polygon) noexcept
			{
			for (const auto& vertex : b.get_vertices())
				{
				if (!contains<strictness>(circle, vertex)) { return false; }
				}
			return true;
			}
		bool collides(const circle& circle, const convex_polygon& polygon) noexcept
			{
			for (const auto& vertex : polygon.get_vertices())
				{
				if (collides(circle, vertex)) { return true; }
				}
			
			if (collides(polygon, circle.center)) { return true; }

			return false;
			}
	#pragma endregion Circle-polygon

	#pragma region Circle-circle
		bool intersects(const circle& a, const circle& b) noexcept
			{
			return vec2f::distance2(a.center, b.center) < ((a.radius + b.radius) * (a.radius + b.radius)) && !contains(a, b);
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		bool contains(const circle& a, const circle& b) noexcept
			{
			if constexpr (strictness == collision_strictness_t::strict)
				{
				return vec2f::distance(a.center, b.center) <  a.radius - b.radius;
				}
			else
				{
				return vec2f::distance(a.center, b.center) <= a.radius - b.radius;
				}
			}
		bool collides(const circle& a, const circle& b) noexcept
			{
			return vec2f::distance2(a.center, b.center) < ((a.radius + b.radius) * (a.radius + b.radius));
			}

		//TODO dige
	#pragma endregion Circle-circle
#pragma endregion Circle
		}