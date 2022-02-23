#pragma once
#include "../../compilation/debug.h"
#include "../vec2.h"

#ifdef utils_is_debug
#include <stdexcept>
#include <string>
#endif

namespace utils::math::geometry
	{
	enum class vertex_name { A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z };
	enum class side_t { left = -1, equal = 0, right = 1 };

	class polygon;

	class segment
		{
		public:
			vec2f a;
			vec2f b;

			float length2() const noexcept { return vec2f::distance2(a, b); }
			float length () const noexcept { return vec2f::distance (a, b); }

			const vec2f& operator[](size_t index) const noexcept { return index == 0 ? a : b; }
			      vec2f& operator[](size_t index)       noexcept { return index == 0 ? a : b; }

			vec2f closest_point(const vec2f& point) const noexcept
				{//http://csharphelper.com/blog/2016/09/find-the-shortest-distance-between-a-point-and-a-line-segment-in-c/
				vec2f delta = {b.x - a.x, b.y - a.y};

				float t = ((point.x - a.x) * delta.x + (point.y - a.y) * delta.y) / (delta.x * delta.x + delta.y * delta.y);

				return t < 0 ? a : t > 1 ? b : vec2f{a.x + t * delta.x, a.y + t * delta.y};
				}

			vec2f closest_point_in_line(const vec2f& point) const noexcept
				{//http://csharphelper.com/blog/2016/09/find-the-shortest-distance-between-a-point-and-a-line-segment-in-c/
				vec2f delta = b - a;

				float t = ((point.x - a.x) * delta.x + (point.y - a.y) * delta.y) / (delta.x * delta.x + delta.y * delta.y);

				return {a.x + t * delta.x, a.y + t * delta.y};
				}

			vec2f vector() const noexcept { return b - a; }

			bool contains(const vec2f& point) const noexcept
				{
				//TODO
				}

			side_t point_side(const vec2f& point) const noexcept
				{
				return side_t::equal;
				/*
				var dir = triangle(point, a, b).area2;
				return dir < 0 ? position.left : dir == 0 ? position.equal : position.right;
				*/
				}

			bool intersects_line(const segment& other) const noexcept
				{
				return other.point_side(a) != other.point_side(b);
				}
			bool intersects(const segment& other) const noexcept
				{
				return intersects_line(other) && other.intersects_line(*this);
				}

			bool intersects(const polygon& poly) const noexcept;

		private:
		};
	}

#include "polygon.h"

namespace utils::math::geometry
	{
	inline bool segment::intersects(const polygon& polygon) const noexcept
		{
		//for (const segment& edge : polygon.edges()) { if (intersects(edge)) { return true; } }
		return false;
		}
	}