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
		inline bool intersects(const vec2f& a, const vec2f& b) noexcept { return a == b; }
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains  (const vec2f& a, const vec2f& b) noexcept { return a == b; }
		inline bool collides  (const vec2f& a, const vec2f& b) noexcept { return a == b; }
	#pragma endregion Point-point

#pragma endregion Point

#pragma region Segment
	#pragma region Point-segment
			inline bool intersects(const vec2f& point, const segment& segment) noexcept { return segment.contains(point); }
			template <collision_strictness_t strictness = collision_strictness_t::loose>
			inline bool contains(const vec2f& point, const segment& segment) noexcept { return segment.contains(point); }
			inline bool collides(const vec2f& point, const segment& segment) noexcept { return segment.contains(point); }

			template <collision_strictness_t strictness = collision_strictness_t::loose>
			inline bool intersects(const segment& segment, const vec2f& point) noexcept { return intersects(point, segment); }
			template <collision_strictness_t strictness = collision_strictness_t::loose>
			inline bool contains(const segment& segment, const vec2f& point) noexcept { return contains(point, segment); }
			inline bool collides(const segment& segment, const vec2f& point) noexcept { return collides(point, segment); }
	#pragma endregion Point-segment

	#pragma region Segment-segment
			inline bool intersects(const segment& a, const segment& b) noexcept { return a.intersects(b); }
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains  (const segment& a, const segment& b) noexcept { return a.intersects(b); }
		inline bool collides  (const segment& a, const segment& b) noexcept { return a.intersects(b); }
	#pragma endregion Segment-segment
#pragma endregion Segment

#pragma region Polygon
	#pragma region Polygon-point
		template <polygon_type_t polygon_type>
		inline bool intersects(const polygon<polygon_type>& polygon, const vec2f& point) noexcept
			{
			for (const auto& edge : polygon.get_edges())
				{
				if (intersects(edge, point)) { return true; }
				}
			return false;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose, polygon_type_t polygon_type>
		inline bool contains(const polygon<polygon_type>& polygon, const vec2f& point) noexcept
			{
			if constexpr (polygon_type == polygon_type_t::convex)
				{
				size_t i{0};
				for (const auto& edge : polygon.get_edges())
					{
					i++;
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
			else
				{
				// A point is inside a polygon if given a line in any direction, it intersects the polygon segments an uneven number of times
				bool is_inside = false;
				segment tmp_segment{point, vec2f{point.x + static_cast<aabb>(polygon).right, point.y}}; //A segment which lies on a generic horizontal line
				for (const auto& edge : polygon.get_edges())
					{
					if (intersects(edge, tmp_segment) && tmp_segment.a.y != edge.b.y) { is_inside = !is_inside; }
					}
				return is_inside;
				}
			}

		template <polygon_type_t polygon_type>
		inline bool collides(const polygon<polygon_type>& polygon, const vec2f& point) noexcept
			{
			return contains<collision_strictness_t::loose>(polygon, point);
			}

		template <polygon_type_t polygon_type>
		inline bool intersects(const vec2f& point, const polygon<polygon_type>& polygon) noexcept { return intersects(polygon, point); }
		template <collision_strictness_t strictness = collision_strictness_t::loose, polygon_type_t polygon_type>
		inline bool contains  (const vec2f& point, const polygon<polygon_type>& polygon) noexcept { return contains  (polygon, point); }
		template <polygon_type_t polygon_type>
		inline bool collides  (const vec2f& point, const polygon<polygon_type>& polygon) noexcept { return collides  (polygon, point); }
	#pragma endregion Polygon-point

	#pragma region Polygon-segment

		template <polygon_type_t polygon_type>
		inline bool intersects(const polygon<polygon_type>& polygon, const segment& segment) noexcept
			{
			for (const auto& edge : polygon.get_edges())
				{
				if (edge.intersects(segment)) { return true; }
				}
			return false;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose, polygon_type_t polygon_type>
		inline bool contains(const polygon<polygon_type>& polygon, const segment& segment) noexcept
			{
			return contains<strictness>(polygon, segment.a) && contains<strictness>(polygon, segment.b);
			}
		template <polygon_type_t polygon_type>
		inline bool collides(const polygon<polygon_type>& polygon, const segment& segment) noexcept { return intersects(polygon, segment) || contains<collision_strictness_t::loose>(polygon, segment); }

		template <polygon_type_t polygon_type>
		inline bool intersects(const segment& segment, const polygon<polygon_type>& polygon) noexcept { return intersects(polygon, segment); }
		template <collision_strictness_t strictness = collision_strictness_t::loose, polygon_type_t polygon_type>
		inline bool contains  (const segment& segment, const polygon<polygon_type>& polygon) noexcept { return contains  (polygon, segment); }
		template <polygon_type_t polygon_type>
		inline bool collides  (const segment& segment, const polygon<polygon_type>& polygon) noexcept { return collides  (polygon, segment); }
	#pragma endregion Polygon-segment

	#pragma region Polygon-polygon

		template <polygon_type_t polygon_type_a, polygon_type_t polygon_type_b>
		inline bool intersects(const polygon<polygon_type_a>& a, const polygon<polygon_type_b>& b) noexcept
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
		template <collision_strictness_t strictness = collision_strictness_t::loose, polygon_type_t polygon_type_a, polygon_type_t polygon_type_b>
		inline bool contains(const polygon<polygon_type_a>& a, const polygon<polygon_type_b>& b) noexcept
			{
			for (const auto& vertex : b.get_vertices())
				{
				if (!contains<strictness>(a, vertex)) { return false; }
				}
			return true;
			}
		template <polygon_type_t polygon_type_a, polygon_type_t polygon_type_b>
		inline bool collides(const polygon<polygon_type_a>& a, const polygon<polygon_type_b>& b) noexcept
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

		inline bool intersects(const circle& circle, const vec2f& point) noexcept
			{
			return vec2f::distance2(circle.center, point) == circle.radius * circle.radius;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const circle& circle, const vec2f& point) noexcept
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
		inline bool collides(const circle& circle, const vec2f& point) noexcept
			{
			return contains<collision_strictness_t::loose>(circle, point);
			}

		inline bool intersects(const vec2f& point, const circle& circle) noexcept { return intersects(circle, point); }
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains  (const vec2f& point, const circle& circle) noexcept { return contains  (circle, point); }
		inline bool collides  (const vec2f& point, const circle& circle) noexcept { return collides  (circle, point); }
	#pragma endregion Circle-point

	#pragma region Circle-segment

		inline bool intersects(const circle& circle, const segment& segment) noexcept
			{
			return segment.minimum_distance(circle.center) <= circle.radius;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const circle& circle, const segment& segment) noexcept
			{
			return contains<strictness>(circle, segment.a) && contains<strictness>(circle, segment.b);
			}
		inline bool collides(const circle& circle, const segment& segment) noexcept
			{
			return contains<collision_strictness_t::loose>(circle, segment) || intersects(circle, segment);
			}

		inline bool intersects(const segment& segment, const circle& circle) noexcept { return intersects(circle, segment); }
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains  (const segment& segment, const circle& circle) noexcept { return contains  (circle, segment); }
		inline bool collides  (const segment& segment, const circle& circle) noexcept { return collides  (circle, segment); }
	#pragma endregion Circle-segment

	#pragma region Circle-polygon
		template <polygon_type_t polygon_type>
		inline bool intersects(const circle& circle, const polygon<polygon_type>& polygon) noexcept
			{
			for (const auto& edge : polygon.get_edges())
				{
				if (intersects(circle, edge)) { return true; }
				}
			return false;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose, polygon_type_t polygon_type>
		inline bool contains(const circle& circle, const polygon<polygon_type>& polygon) noexcept
			{
			for (const auto& vertex : polygon.get_vertices())
				{
				if (!contains<strictness>(circle, vertex)) { return false; }
				}
			return true;
			}
		template <polygon_type_t polygon_type>
		inline bool collides(const circle& circle, const polygon<polygon_type>& polygon) noexcept
			{
			for (const auto& vertex : polygon.get_vertices())
				{
				if (collides(circle, vertex)) { return true; }
				}
			
			if (collides(polygon, circle.center)) { return true; }

			for (const auto& edge : polygon.get_edges())
			{
				if (collides(circle, edge)) { return true; }
			}

			return false;
			}

		template <polygon_type_t polygon_type>
		inline bool intersects(const polygon<polygon_type>& polygon, const circle& circle) noexcept { return intersects(circle, polygon); }
		template <collision_strictness_t strictness = collision_strictness_t::loose, polygon_type_t polygon_type>
		inline bool contains  (const polygon<polygon_type>& polygon, const circle& circle) noexcept { return contains  (circle, polygon); }
		template <polygon_type_t polygon_type>
		inline bool collides  (const polygon<polygon_type>& polygon, const circle& circle) noexcept { return collides  (circle, polygon); }
	#pragma endregion Circle-polygon

	#pragma region Circle-circle
		template <collision_strictness_t strictness>
		inline bool contains(const circle& a, const circle& b) noexcept;
		inline bool intersects(const circle& a, const circle& b) noexcept
			{
			return vec2f::distance2(a.center, b.center) < ((a.radius + b.radius) * (a.radius + b.radius)) && !contains<collision_strictness_t::strict>(a, b);
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const circle& a, const circle& b) noexcept
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
		inline bool collides(const circle& a, const circle& b) noexcept
			{
			return vec2f::distance2(a.center, b.center) < ((a.radius + b.radius) * (a.radius + b.radius));
			}
	#pragma endregion Circle-circle
#pragma endregion Circle
		}