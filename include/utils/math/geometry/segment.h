#pragma once

#include <algorithm>
#include <optional>

#include "../../compilation/debug.h"
#include "../constants.h"
#include "../vec2.h"
#include "aabb.h"

#ifdef utils_is_debug
#include <stdexcept>
#include <string>
#endif

namespace utils::math::geometry
	{
	enum class vertex_name { A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z };
	enum class side_t { left = -1, equal = 0, right = 1 };

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

			/// <summary> Unit vector from a towards b. </summary>
			vec2f forward() const noexcept { return (b - a).normalize(); }
			/// <summary> Unit vector perpendicular on the left from a to b. </summary>
			vec2f perpendicular_right() const noexcept { const auto tmp{forward()}; return tmp.perpendicular_right(); }
			/// <summary> Unit vector perpendicular on the right from a to b. </summary>
			vec2f perpendicular_left()  const noexcept { const auto tmp{forward()}; return tmp.perpendicular_left(); }


			float minimum_distance(const vec2f& point) const noexcept {
				// https://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment

				// Return minimum distance between line segment vw and point p
				const float l2 = vec2f::distance2(a, b);  // i.e. |b-a|^2 -  avoid a sqrt
				if (l2 == 0.0) { return vec2f::distance(point, a); } // a == b case
				// Consider the line extending the segment, parameterized as a + t (b - a).
				// We find projection of point p onto the line. 
				// It falls where t = [(p-a) . (b-a)] / |b-a|^2
				// We clamp t from [0,1] to handle points outside the segment vw.
				//const float t = std::max(0.f, std::min(1.f, ((point - a) <utils::math::operators::dot> (b - a)) / l2));
				const float t = utils::math::clamp(0.f, 1.f, ((point - a) < utils::math::operators::dot > (b - a)) / l2);
				const vec2f projection = a + (b - a) * t;  // Projection falls on the segment
				return vec2f::distance(point, projection);
				}

			vec2f vector() const noexcept { return b - a; }

			bool contains(const vec2f& point) const noexcept
				{
				// https://stackoverflow.com/questions/17692922/check-is-a-point-x-y-is-between-two-points-drawn-on-a-straight-line
				auto dist{minimum_distance(point)};
				return dist < constants::epsilonf;
				}

			side_t point_side(const vec2f& point) const noexcept
				{
				// a-b-point triangle signed area^2
				const float some_significant_name_ive_yet_to_figure_out{((b.x - a.x) * (point.y - a.y)) - ((point.x - a.x) * (b.y - a.y))};
				return some_significant_name_ive_yet_to_figure_out < -constants::epsilonf? side_t::left : some_significant_name_ive_yet_to_figure_out > constants::epsilonf ? side_t::right : side_t::equal;
				}

			bool intersects_line(const segment& other) const noexcept
				{
				auto point_side_a{other.point_side(a)};
				auto point_side_b{other.point_side(b)};
				return (point_side_a != point_side_b) || (point_side_a == side_t::equal); // second condition returns true when the two lines are the same
				}
			bool intersects(const segment& other) const noexcept
				{
				return intersects_line(other) && other.intersects_line(*this);
				}

			std::optional<vec2f> intersection(const segment& other) const noexcept
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
			
			aabb bounding_box() const noexcept
				{
				return aabb
					{rect<float>{
					.ll = std::min(a.x,  b.x),
					.up = std::min(a.y,  b.y),
					.rr = std::max(a.x,  b.x),
					.dw = std::max(a.y,  b.y),
					}};
				}

		private:
		};
	}