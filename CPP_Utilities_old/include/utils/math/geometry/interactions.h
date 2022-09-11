#pragma once

#include "../vec2.h"
#include "../angle.h"
#include "segment.h"
#include "point.h"
#include "polygon.h"
#include "circle.h"
#include "aabb.h"
#include "../constants.h"

namespace utils::math::geometry
	{
	enum class collision_strictness_t { strict, loose };


#pragma region Point
	#pragma region Point-point
		inline bool intersects(const point& a, const point& b) { return a == b; };
		
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const point& a, const point& b) { return a == b; };
		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const point& a, const point& b) { return a == b; };

	#pragma endregion Point-point

#pragma endregion Point

#pragma region Segment
	#pragma region Segment-point
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool intersects(const segment& segment, const point& point) noexcept { return segment.contains(point); }
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains  (const segment& segment, const point& point) noexcept { return segment.contains(point); }
		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides  (const segment& segment, const point& point) noexcept { return segment.contains(point); }

		inline bool intersects(const point& point, const segment& segment) noexcept { return segment.contains(point); }
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains  (const point& point, const segment& segment) noexcept { return false; }
		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides  (const point& point, const segment& segment) noexcept { return segment.contains(point); }
	#pragma endregion Segment-point

	#pragma region Segment-segment
		inline bool intersects(const segment& a, const segment& b) noexcept { return a.intersects(b); }
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains  (const segment& a, const segment& b) noexcept { return a.contains(b.a) && a.contains(b.b); }
		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides  (const segment& a, const segment& b) noexcept { return a.intersects(b); }
	#pragma endregion Segment-segment
#pragma endregion Segment

#pragma region Polygon
	#pragma region Polygon-point
		
		inline bool intersects(const polygon& polygon, const point& point) noexcept
			{
			for (const auto& edge : polygon.get_edges())
				{
				if (intersects(edge, point)) { return true; }
				}
			return false;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const polygon& polygon, const point& point) noexcept
			{
			// A point is inside a polygon if given a line in any direction, it intersects the polygon segments an uneven number of times
			segment tmp_segment{point, vec2f{point.x + static_cast<aabb>(polygon).right, point.y}}; //A segment which lies on a generic horizontal line

			bool is_inside = false;

			const auto& vertices{polygon.get_vertices()};
			for (size_t i{0}; i < vertices.size(); i++)
				{
				const auto& a{vertices[i]};
				const auto& b{vertices[(i + 1) % vertices.size()]};

				//no intersection
				if ((a.y < point.y && b.y < point.y) || (a.y > point.y && b.y > point.y)) { continue; }
				if (a.x < point.x && b.x < point.x) { continue; }
				if (!segment{a, b}.intersects(tmp_segment)) { continue; } //TODO optimize for tmp_segment being an horizontal line to infinity

				if (b.y == point.y) // If vertex b is on line
					{
					const auto& c{vertices[(i + 2) % vertices.size()]};

					if (tmp_segment.point_side(a) != tmp_segment.point_side(c)) { is_inside = !is_inside; }
					}
				else if(a.y != point.y) // If edge intersects line (excluding vertex a)
					{
					is_inside = !is_inside;
					}
				}

				
			return is_inside;
			}

		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const convex_polygon& polygon, const point& point) noexcept
			{
			size_t i{ 0 };
			for (const auto& edge : polygon.get_edges())
			{
				i++;
				side_t side{ edge.point_side(point) };

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


		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const polygon& polygon, const point& point) noexcept
			{
			if constexpr (hollow_a) { return intersects(polygon, point);}
			else					{ return contains<collision_strictness_t::loose>(polygon, point);}
			}

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const convex_polygon& polygon, const point& point) noexcept
			{
			if constexpr (hollow_a) { return intersects(polygon, point); }
			else					{ return contains<collision_strictness_t::loose>(polygon, point); }
			}

		
		inline bool intersects(const point& point, const polygon& polygon) noexcept { return intersects(polygon, point); }

		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains  (const point& point, const        polygon& polygon) noexcept { return false; }

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides  (const point& point, const        polygon& polygon) noexcept { return collides<hollow_b, hollow_a>(polygon, point); }
		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides  (const point& point, const convex_polygon& polygon) noexcept { return collides<hollow_b, hollow_a>(polygon, point); }
	#pragma endregion Polygon-point

	#pragma region Polygon-segment
		inline bool intersects(const polygon& polygon, const segment& segment) noexcept
			{
			for (const auto& edge : polygon.get_edges())
				{
				if (edge.intersects(segment)) { return true; }
				}
			return false;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const polygon& polygon, const segment& segment) noexcept
			{
			if (!(contains<strictness>(polygon, segment.a) || contains<strictness>(polygon, segment.b))) { return false; }

			for (const auto& edge : polygon.get_edges())
				{
				if (edge.intersects(segment)) { return false; }
				}

			return true;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const convex_polygon& polygon, const segment& segment) noexcept
			{
			return contains<strictness>(polygon, segment.a) && contains<strictness>(polygon, segment.b);
			}

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const        polygon& polygon, const segment& segment) noexcept 
			{
			if constexpr (hollow_a) { return intersects(polygon, segment); }
			else { return intersects(polygon, segment) || contains<collision_strictness_t::loose>(polygon, segment.a) || contains<collision_strictness_t::loose>(polygon, segment.b); }
			}
		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const convex_polygon& polygon, const segment& segment) noexcept
			{
			if constexpr (hollow_a) { return intersects(polygon, segment); }
			else { return intersects(polygon, segment) || contains<collision_strictness_t::loose>(polygon, segment.a) || contains<collision_strictness_t::loose>(polygon, segment.b); }
			}

		inline bool intersects(const segment& segment, const polygon& polygon) noexcept { return intersects(polygon, segment); }

		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains  (const segment& segment, const polygon& polygon) noexcept { return false; }

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const segment& segment, const        polygon& polygon) noexcept { return collides<hollow_b, hollow_a>(polygon, segment); }
		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const segment& segment, const convex_polygon& polygon) noexcept { return collides<hollow_b, hollow_a>(polygon, segment); }
	#pragma endregion Polygon-segment

	#pragma region Polygon-polygon
		inline bool intersects(const polygon& a, const polygon& b) noexcept
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
		inline bool contains(const polygon& a, const polygon& b) noexcept
			{
			for (const auto& segment : b.get_edges())
				{
				if (!contains<strictness>(a, segment)) { return false; }
				}
			return true;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const convex_polygon& a, const polygon& b) noexcept
			{
			for (const auto& vertex : b.get_vertices())
				{
				if (!contains<strictness>(a, vertex)) { return false; }
				}
			return true;
			}

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const polygon& a, const polygon& b) noexcept
			{
			for (const auto& vertex : a.get_vertices())
				{
				if (collides<hollow_b, false>(b, vertex)) { return true; }
				}
			for (const auto& vertex : b.get_vertices())
				{
				if (collides<hollow_a, false>(a, vertex)) { return true; }
				}
			return intersects(a, b);
			}

		template <>
		inline bool collides<true, true>(const polygon& a, const polygon& b) noexcept
		{
			return intersects(a, b);
		}

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const convex_polygon& a, const polygon& b) noexcept
			{
			for (const auto& vertex : a.get_vertices())
				{
				if (collides<hollow_b, false>(b, vertex)) { return true; }
				}
			for (const auto& vertex : b.get_vertices())
				{
				if (collides<hollow_a, false>(a, vertex)) { return true; }
				}
			return intersects(a, b);
			}

		template <>
		inline bool collides<true, true>(const convex_polygon& a, const polygon& b) noexcept
		{
			return intersects(a, b);
		}

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const polygon& a, const convex_polygon& b) noexcept { return collides<hollow_b, hollow_a>(b, a); }

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const convex_polygon& a, const convex_polygon& b) noexcept
			{
			for (const auto& vertex : a.get_vertices())
				{
				if (collides<hollow_b, false>(b, vertex)) { return true; }
				}
			for (const auto& vertex : b.get_vertices())
				{
				if (collides<hollow_a, false>(a, vertex)) { return true; }
				}
			return intersects(a, b);
			}

		template <>
		inline bool collides<true, true>(const convex_polygon& a, const convex_polygon& b) noexcept
		{
			return intersects(a, b);
		}
	#pragma endregion Polygon-polygon
#pragma endregion Polygon

#pragma region AABB

	#pragma region AABB-point
		inline bool intersects(const aabb& a, const point& b) noexcept
			{
			return b.y == a.up && b.x >= a.ll && b.x <= a.rr 
				|| b.y == a.dw && b.x >= a.ll && b.x <= a.rr
				|| b.x == a.rr && b.y >= a.up && b.y <= a.dw
				|| b.x == a.ll && b.y >= a.up && b.y <= a.dw;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const aabb& a, const point& b) noexcept
			{
			if constexpr (strictness == collision_strictness_t::strict)
				{
				return b.y >  a.up && b.y <  a.dw
					&& b.x >  a.ll && b.x <  a.rr;
				}
			else
				{
				return b.y >= a.up && b.y <= a.dw
					&& b.x >= a.ll && b.x <= a.rr;
				}
			}

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const aabb& aabb, const point& point) noexcept
		{
			if constexpr (hollow_a) { return intersects(aabb, point); }
			else { return contains<collision_strictness_t::loose>(aabb, point); }
		}

		inline bool intersects(const point& point, const aabb& aabb) noexcept { return intersects(aabb, point); }
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const point& point, const aabb& aabb) noexcept { return false; }

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const point& point, const aabb& aabb) noexcept { return collides<hollow_b, hollow_a>(aabb, point); }
	#pragma endregion AABB-point
	
	#pragma region AABB - segment
		inline bool intersects(const aabb& aabb, const segment& s) noexcept
			{
			return intersects(segment{ aabb.ul, aabb.ur}, s) || intersects(segment{ aabb.ur, aabb.dr}, s)
				|| intersects(segment{ aabb.dr, aabb.dl}, s) || intersects(segment{ aabb.dl, aabb.ul}, s);
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const aabb& aabb, const segment& segment) noexcept
			{
			return contains<strictness>(aabb, segment.a) && contains<strictness>(aabb, segment.b);
			}

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const aabb& aabb, const segment& segment) noexcept 
			{
			if constexpr (hollow_a) { return intersects(aabb, segment); }
			else 
				{
				if (contains<collision_strictness_t::loose>(aabb, segment.a) || contains<collision_strictness_t::loose>(aabb, segment.b)) { return true; }
				return intersects(aabb, segment);
				}
			}

		inline bool intersects(const segment& segment, const aabb& aabb) noexcept { return intersects(aabb, segment); }
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const segment& segment, const aabb& aabb) noexcept { return false; }

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const segment& segment, const aabb& aabb) noexcept { return collides<hollow_b, hollow_a>(aabb, segment); }
	#pragma endregion AABB - segment
	#pragma region AABB - polygon
		inline bool intersects(const aabb& a, const polygon& b) noexcept
			{
			return intersects(segment{a.ul, a.ur}, b) || intersects(segment{a.ur, a.dr}, b)
				|| intersects(segment{a.dr, a.dl}, b) || intersects(segment{a.dl, a.ul}, b);
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const aabb& a, const polygon& b) noexcept
			{
			for (const auto& vertex : b.get_vertices())
				{
				if (!contains<strictness>(a, vertex)) { return false; }
				}
			return true;
			}

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const aabb& a, const polygon& b) noexcept
			{
			for (const auto& vertex : b.get_vertices())
				{
				if (contains<collision_strictness_t::loose>(a, vertex)) { return true; }
				}
			if (contains(b, a.ul) || contains(b, a.ur) || contains(b, a.dr) || contains(b, a.dl)) { return true; }
			return intersects(a, b);
			}

		template <>
		inline bool collides<true, true>(const aabb& a, const polygon& b) noexcept
		{
			return intersects(a, b);
		}

		template <>
		inline bool collides<true, false>(const aabb& a, const polygon& b) noexcept
		{
			if (contains(b, a.ul) || contains(b, a.ur) || contains(b, a.dr) || contains(b, a.dl)) { return true; }
			return intersects(a, b);
		}

		template <>
		inline bool collides<false, true>(const aabb& a, const polygon& b) noexcept
		{
			for (const auto& vertex : b.get_vertices())
			{
				if (contains<collision_strictness_t::loose>(a, vertex)) { return true; }
			}
			return intersects(a, b);
		}


		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const aabb& a, const convex_polygon& b) noexcept
			{
			for (const auto& vertex : b.get_vertices())
				{
				if (contains<collision_strictness_t::loose>(a, vertex)) { return true; }
				}
			if (contains(b, a.ul) || contains(b, a.ur) || contains(b, a.dr) || contains(b, a.dl)) { return true; }
			return intersects(a, b);
			}

		template <>
		inline bool collides<true, true>(const aabb& a, const convex_polygon& b) noexcept
		{
			return intersects(a, b);
		}

		template <>
		inline bool collides<true, false>(const aabb& a, const convex_polygon& b) noexcept
		{
			if (contains(b, a.ul) || contains(b, a.ur) || contains(b, a.dr) || contains(b, a.dl)) { return true; }
			return intersects(a, b);
		}

		template <>
		inline bool collides<false, true>(const aabb& a, const convex_polygon& b) noexcept
		{
			for (const auto& vertex : b.get_vertices())
			{
				if (contains<collision_strictness_t::loose>(a, vertex)) { return true; }
			}
			return intersects(a, b);
		}

		inline bool intersects(const polygon& polygon, const aabb& aabb) noexcept { return intersects(aabb, polygon); }
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const polygon& polygon, const aabb& aabb) noexcept
			{//generic polygon contains AABB only if it contains all its edges
			return contains<strictness>(polygon, segment{aabb.ul, aabb.ur}) && contains<strictness>(polygon, segment{aabb.ur, aabb.dr})
				&& contains<strictness>(polygon, segment{aabb.dr, aabb.dl}) && contains<strictness>(polygon, segment{aabb.dl, aabb.ul});
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const convex_polygon& polygon, const aabb& aabb) noexcept
			{//convex polygon only has to check for vertices
			return contains<strictness>(polygon, aabb.ul) && contains<strictness>(polygon, aabb.ur)
				&& contains<strictness>(polygon, aabb.dr) && contains<strictness>(polygon, aabb.dl);
			}
		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const        polygon& polygon, const aabb& aabb) noexcept { return collides<hollow_b, hollow_a>(aabb, polygon); }
		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const convex_polygon& polygon, const aabb& aabb) noexcept { return collides<hollow_b, hollow_a>(aabb, polygon); }
	#pragma endregion AABB - polygon
	#pragma region AABB - AABB
		inline bool intersects(const aabb& a, const aabb& b) noexcept
			{
			return intersects(segment{a.ul, a.ur}, b) || intersects(segment{a.ur, a.dr}, b)
				|| intersects(segment{a.dr, a.dl}, b) || intersects(segment{a.dl, a.ul}, b);
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const aabb& a, const aabb& b) noexcept
			{
			return contains<strictness>(a, b.ul) && contains<strictness>(a, b.ur) 
				&& contains<strictness>(a, b.dr) && contains<strictness>(a, b.dl);
			}

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const aabb& a, const aabb& b) noexcept
			{
				return a.ll <= b.rr && a.rr >= b.ll && a.up <= b.dw && a.dw >= b.up; 
			}

		template <>
		inline bool collides<true, true>(const aabb& a, const aabb& b) noexcept
		{
			return intersects(a, b);
		}

		template <>
		inline bool collides<true, false>(const aabb& a, const aabb& b) noexcept
		{
			return contains<collision_strictness_t::loose>(b, a) || intersects(a, b);
		}

		template <>
		inline bool collides<false, true>(const aabb& a, const aabb& b) noexcept
		{
			return collides<true, false>(b, a);
			//return contains<collision_strictness_t::loose>(a, b) || intersects(a, b);
		}
			
	#pragma endregion AABB - AABB

#pragma endregion AABB


#pragma region Circle
	#pragma region Circle-point

		inline bool intersects(const circle& circle, const point& point) noexcept
			{
			return vec2f::distance2(circle.center, point) == circle.radius * circle.radius;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const circle& circle, const point& point) noexcept
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
		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const circle& circle, const point& point) noexcept
			{
			if constexpr (hollow_a) { return intersects(circle, point); }
			else					{ return contains<collision_strictness_t::loose>(circle, point); }
			}



		inline bool intersects(const point& point, const circle& circle) noexcept { return intersects(circle, point); }
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains  (const point& point, const circle& circle) noexcept { return false; }
		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides  (const point& point, const circle& circle) noexcept { return collides<hollow_b, hollow_a>  (circle, point); }
	#pragma endregion Circle-point

	#pragma region Circle-segment

		inline bool intersects(const circle& circle, const segment& segment) noexcept
			{
			if ( contains(circle, segment.a) && !contains(circle, segment.b)) { return true; }
			if ( contains(circle, segment.b) && !contains(circle, segment.a)) { return true; }
			if (!contains(circle, segment.b) && !contains(circle, segment.a)) { return segment.minimum_distance(circle.center) <= circle.radius; }
			return false;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const circle& circle, const segment& segment) noexcept
			{
			return contains<strictness>(circle, segment.a) && contains<strictness>(circle, segment.b);
			}
		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const circle& circle, const segment& segment) noexcept
			{
			if constexpr (hollow_a) { return intersects(circle, segment); }
			else 
				{
				if (contains<collision_strictness_t::loose>(circle, segment)) { return true; }
				return segment.minimum_distance(circle.center) <= circle.radius;
				}
			}

		inline bool intersects(const segment& segment, const circle& circle) noexcept { return intersects(circle, segment); }
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains  (const segment& segment, const circle& circle) noexcept { return false; }
		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides  (const segment& segment, const circle& circle) noexcept { return collides<hollow_b, hollow_a>  (circle, segment); }
	#pragma endregion Circle-segment

	#pragma region Circle-polygon
		
		inline bool intersects(const circle& circle, const polygon& polygon) noexcept
			{
			for (const auto& edge : polygon.get_edges())
				{
				if (intersects(circle, edge)) { return true; }
				}
			return false;
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const circle& circle, const polygon& polygon) noexcept
			{
			for (const auto& vertex : polygon.get_vertices())
				{
				if (!contains<strictness>(circle, vertex)) { return false; }
				}
			return true;
			}

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const circle& circle, const polygon& polygon) noexcept
			{
			for (const auto& vertex : polygon.get_vertices())
				{
				if (collides<hollow_a, false>(circle, vertex)) { return true; }
				}
			
			if (collides<hollow_a, false>(polygon, circle.center)) { return true; }

			for (const auto& edge : polygon.get_edges())
				{
				if (collides<hollow_a, false>(circle, edge)) { return true; }
				}

			return false;
			}

		template <>
		inline bool collides<true, true>(const circle& circle, const polygon& polygon) noexcept
		{
			return intersects(circle, polygon);
		}

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const circle& circle, const convex_polygon& polygon) noexcept
		{//TODO
			for (const auto& vertex : polygon.get_vertices())
			{
				if (collides<hollow_a, false>(circle, vertex)) { return true; }
			}

			if (collides<hollow_a, false>(polygon, circle.center)) { return true; }

			for (const auto& edge : polygon.get_edges())
			{
				if (collides<hollow_a, false>(circle, edge)) { return true; }
			}

			return false;
		}

		template <>
		inline bool collides<true, true>(const circle& circle, const convex_polygon& polygon) noexcept
		{
			return intersects(circle, polygon);
		}

		inline bool intersects(const polygon& polygon, const circle& circle) noexcept { return intersects(circle, polygon); }
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains  (const polygon& polygon, const circle& circle) noexcept 
			{
			if (!contains(polygon, circle.center)) { return false; }

			for (const auto& edge : polygon.get_edges())
				{
				if constexpr (strictness == collision_strictness_t::loose) { if (edge.minimum_distance(circle.center) <  circle.radius) { return false; } }
				else                                                       { if (edge.minimum_distance(circle.center) <= circle.radius) { return false; } }
				}
			return true;
			}

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides  (const        polygon& polygon, const circle& circle) noexcept { return collides<hollow_b, hollow_a>  (circle, polygon); }
		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides  (const convex_polygon& polygon, const circle& circle) noexcept { return collides<hollow_b, hollow_a>  (circle, polygon); }
	#pragma endregion Circle-polygon

#pragma region Circle-aabb
		inline bool intersects(const circle& circle, const aabb& aabb) noexcept
			{//TODO betterify, look herehttps://arrowinmyknee.com/2021/04/02/how-to-detect-intersection-between-circle-and-aabb/
			return intersects(segment{aabb.ul, aabb.ur}, circle) || intersects(segment{aabb.ur, aabb.dr}, circle)
				|| intersects(segment{aabb.dr, aabb.dl}, circle) || intersects(segment{aabb.dl, aabb.ul}, circle);
			}
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const circle& circle, const aabb& aabb) noexcept
			{
			return contains<strictness>(circle, aabb.ul) && contains<strictness>(circle, aabb.ur)
				&& contains<strictness>(circle, aabb.dr) && contains<strictness>(circle, aabb.dl);
			}

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const circle& circle, const aabb& aabb) noexcept
			{//TODO betterify, look herehttps://arrowinmyknee.com/2021/04/02/how-to-detect-intersection-between-circle-and-aabb/
			return contains<collision_strictness_t::loose>(circle, aabb) || contains<collision_strictness_t::loose>(aabb, circle) || intersects(circle, aabb);
			}

		template <>
		inline bool collides<true, true>(const circle& circle, const aabb& aabb) noexcept
		{
			return intersects(circle, aabb);
		}

		template <>
		inline bool collides<true, false>(const circle& circle, const aabb& aabb) noexcept
		{
			return contains<collision_strictness_t::loose>(aabb, circle) || intersects(circle, aabb);
		}

		template <>
		inline bool collides<false, true>(const circle& circle, const aabb& aabb) noexcept
		{
			return collides<true, false>(aabb, circle);
		}

		inline bool intersects(const aabb& aabb, const circle& circle) noexcept { return intersects(circle, aabb); }
		template <collision_strictness_t strictness = collision_strictness_t::loose>
		inline bool contains(const aabb& aabb, const circle& circle) noexcept 
			{
			if constexpr (strictness == collision_strictness_t::strict)
				{
				return circle.center.x - circle.radius >  aabb.ll
					&& circle.center.x + circle.radius <  aabb.rr
					&& circle.center.y - circle.radius >  aabb.up
					&& circle.center.y + circle.radius <  aabb.dw;
				}
			else
				{
				return circle.center.x - circle.radius >= aabb.ll
					&& circle.center.x + circle.radius <= aabb.rr
					&& circle.center.y - circle.radius >= aabb.up
					&& circle.center.y + circle.radius <= aabb.dw;
				}
			}

		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const aabb& aabb, const circle& circle) noexcept { return collides<hollow_b, hollow_a>(circle, aabb); }
#pragma endregion Circle-aabb

	#pragma region Circle-circle
		template <collision_strictness_t strictness>
		inline bool contains(const circle& a, const circle& b) noexcept;
		inline bool intersects(const circle& a, const circle& b) noexcept
			{
			return vec2f::distance2(a.center, b.center) < ((a.radius + b.radius) * (a.radius + b.radius)) && !contains<collision_strictness_t::strict>(a, b) && !contains<collision_strictness_t::strict>(b, a);
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
		template <bool hollow_a = false, bool hollow_b = false>
		inline bool collides(const circle& a, const circle& b) noexcept
			{
			return vec2f::distance2(a.center, b.center) < ((a.radius + b.radius) * (a.radius + b.radius));
			}

		template <>
		inline bool collides<true, true>(const circle& a, const circle& b) noexcept
		{
			return intersects(a, b);
		}

		template <>
		inline bool collides<true, false>(const circle& a, const circle& b) noexcept
		{
			return contains(b,a) || intersects(a, b);
		}

		template <>
		inline bool collides<false, true>(const circle& a, const circle& b) noexcept
		{
			return collides<true, false>(b, a);
		}
	#pragma endregion Circle-circle
#pragma endregion Circle
		}