#pragma once

#include <algorithm>
#include <optional>

#include "../../../compilation/debug.h"
#include "../../constants.h"

#include "../common/begin.h"

namespace utils::math::geometry
	{
	enum class vertex_name { A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z };
	enum class side_t { left = -1, equal = 0, right = 1 };

	struct segment : shape_base<segment>
		{
		public:
			segment() = default;
			segment(const vec2f& a, const vec2f& b) : a{a}, b{b} {}
			vec2f a;
			vec2f b;

			float length2() const noexcept { return vec2f::distance2(a, b); }
			float length () const noexcept { return vec2f::distance (a, b); }

			const vec2f& operator[](size_t index) const noexcept { return index == 0 ? a : b; }
			      vec2f& operator[](size_t index)       noexcept { return index == 0 ? a : b; }

			vec2f closest_point_in_line_to(const vec2f& point) const noexcept
				{//http://csharphelper.com/blog/2016/09/find-the-shortest-distance-between-a-point-and-a-line-segment-in-c/
				vec2f delta = b - a;

				float t = ((point.x - a.x) * delta.x + (point.y - a.y) * delta.y) / (delta.x * delta.x + delta.y * delta.y);

				return {a.x + t * delta.x, a.y + t * delta.y};
				}

			/// <summary> Unit vector from a towards b. </summary>
			vec2f forward() const noexcept { return (b - a).normalize(); }
			/// <summary> Unit vector perpendicular on the left from a to b. </summary>
			vec2f perpendicular_right() const noexcept { return forward().perpendicular_right(); }
			/// <summary> Unit vector perpendicular on the right from a to b. </summary>
			vec2f perpendicular_left()  const noexcept { return forward().perpendicular_left (); }


			float minimum_distance(const vec2f& point) const noexcept 
				{
				// https://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment

				// Return minimum distance between line segment vw and point p
				const float l2 = vec2f::distance2(a, b);  // i.e. |b-a|^2 -  avoid a sqrt
				if (l2 == 0.0) { return vec2f::distance(point, a); } // a == b case
				// Consider the line extending the segment, parameterized as a + t (b - a).
				// We find projection of point p onto the line. 
				// It falls where t = [(p-a) . (b-a)] / |b-a|^2
				// We clamp t from [0,1] to handle points outside the segment vw.
				//const float t = std::max(0.f, std::min(1.f, ((point - a) <utils::math::operators::dot> (b - a)) / l2));
				const float t = utils::math::clamp(((point - a) < utils::math::operators::dot > (b - a)) / l2, 0.f, 1.f);
				const vec2f projection{a + (b - a) * t};  // Projection falls on the segment
				return vec2f::distance(point, projection);
				}

			float distance_line(const vec2f& point) const noexcept
				{
				return vec2f::distance(point, closest_point_in_line_to(point));
				}

			vec2f vector() const noexcept { return b - a; }

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
			
			aabb bounding_box() const noexcept
				{
				return aabb{rect<float>
					{
					.ll = std::min(a.x,  b.x),
					.up = std::min(a.y,  b.y),
					.rr = std::max(a.x,  b.x),
					.dw = std::max(a.y,  b.y),
					}};
				}


			using shape_base<segment>::closest_point_and_distance;
			using shape_base<segment>::closest_point_to;
			using shape_base<segment>::distance_min;
			using shape_base<segment>::vector_to;
			using shape_base<segment>::intersects;
			using shape_base<segment>::intersection;
			using shape_base<segment>::contains;
			using shape_base<segment>::collides_with;

			vec2f closest_point_to(const point& other) const noexcept;
			float distance_min    (const point& other) const noexcept;
			bool  contains        (const point& other) const noexcept;

			closest_point_and_distance_t closest_point_and_distance(const segment& other) const noexcept;
			bool                 intersects      (const segment& other) const noexcept;
			std::optional<vec2f> intersection    (const segment& other) const noexcept;
			bool                 contains        (const segment& other) const noexcept;

			closest_point_and_distance_t closest_point_and_distance(const aabb& other) const noexcept;
			bool                 intersects      (const aabb& other) const noexcept;
			std::optional<vec2f> intersection    (const aabb& other) const noexcept;
			bool                 contains        (const aabb& other) const noexcept;

			closest_point_and_distance_t closest_point_and_distance(const polygon& other) const noexcept;
			bool                 intersects      (const polygon& other) const noexcept;
			std::optional<vec2f> intersection    (const polygon& other) const noexcept;
			bool                 contains        (const polygon& other) const noexcept;

			closest_point_and_distance_t closest_point_and_distance(const circle& other) const noexcept;
			bool                 intersects      (const circle& other) const noexcept;
			std::optional<vec2f> intersection    (const circle& other) const noexcept;
			bool                 contains        (const circle& other) const noexcept;
			
			segment& scale_self    (const float      & scaling    ) noexcept { a.scale_self    (scaling    ); b.scale_self    (scaling    ); return *this; }
			segment& rotate_self   (const angle::radf& rotation   ) noexcept { a.rotate_self   (rotation   ); b.rotate_self   (rotation   ); return *this; }
			segment& translate_self(const vec2f      & translation) noexcept { a.translate_self(translation); b.translate_self(translation); return *this; }
		private:
		};
	}