#pragma once

#include <optional>
#include <limits>

#include "../vec2.h"
#include "../angle.h"
#include "segment.h"
#include "polygon.h"
#include "circle.h"
#include "aabb.h"

#include "continuous_point.h"
#include "interactions.h"

#include "../constants.h"

// NOTES
/*
Inside parameter expects true if the source of the continuous collider was already inside the 
other shape. Please use interactions/contains(shape, continuous.a) to retrieve this information.
*/

namespace utils::math::geometry
	{
	struct collision_data
		{
		vec2f normal;
		vec2f impact_point;
		float t;
		};

	using collision = std::optional<collision_data>;

	namespace details
		{
		inline void check_continuous_point_to_edge(const continuous_point& cpoint, const segment& edge, std::optional<vec2f>& intersection, segment& hit_edge, float& distance)
			{
			auto tmp_intersection{cpoint.intersection(edge)};
			if (!tmp_intersection) { return; }

			float tmp_distance{vec2f::distance2(tmp_intersection.value(), cpoint.a)};

			/*
			We are excluding cases when the continuous movement ends exactly ON another collider's edge.
			This case will NOT return a continuous collision. Instead in the next step we will have a continuous collision with t = 0;
			*/
			float distance_from_b{vec2f::distance2(tmp_intersection.value(), cpoint.b)};

			if (tmp_distance < distance && distance_from_b > 0)
				{
				intersection = tmp_intersection;
				distance = tmp_distance;
				hit_edge = edge;
				}
			}
		}

	inline collision continuous_collides(const continuous_point& cpoint, const aabb& aabb, bool inside = false) noexcept
		{

		std::optional<vec2f> intersection{std::nullopt};
		segment hit_edge;
		float distance{std::numeric_limits<float>::infinity()};

		details::check_continuous_point_to_edge(cpoint, segment{aabb.ul, aabb.ur}, intersection, hit_edge, distance);
		details::check_continuous_point_to_edge(cpoint, segment{aabb.ur, aabb.dr}, intersection, hit_edge, distance);
		details::check_continuous_point_to_edge(cpoint, segment{aabb.dr, aabb.dl}, intersection, hit_edge, distance);
		details::check_continuous_point_to_edge(cpoint, segment{aabb.dl, aabb.ul}, intersection, hit_edge, distance);

		if (!intersection) { return std::nullopt; }

		return collision_data
			{
			.normal{inside ? hit_edge.perpendicular_right() : hit_edge.perpendicular_left()},
			.impact_point{intersection.value()},
			.t{std::sqrt(distance) / cpoint.length()}
			};
		}
	inline collision continuous_collides(const continuous_point& cpoint, const polygon& polygon, bool inside = false) noexcept
		{
		std::optional<vec2f> intersection{std::nullopt};
		segment hit_edge;
		float distance2{std::numeric_limits<float>::infinity()};

		for (const auto& edge : polygon.get_edges()) { details::check_continuous_point_to_edge(cpoint, edge, intersection, hit_edge, distance2); }

		if (!intersection) { return std::nullopt; }

		return collision_data
			{
			.normal{inside ? hit_edge.perpendicular_right() : hit_edge.perpendicular_left()},
			.impact_point{intersection.value()},
			.t{std::sqrt(distance2) / cpoint.length()}
			};
		}

	inline collision continuous_collides(const continuous_point& cpoint, const circle& circle, bool inside = false) noexcept
		{
		bool intersection{intersects(circle, cpoint)};
		if (!intersection) { return std::nullopt; }
		
		// https://answers.unity.com/questions/1658184/circle-line-intersection-points.html
		using namespace operators;
		//  get the distance between X and Z on the segment
		vec2f delta_point = cpoint.b - cpoint.a;

		float a = delta_point <dot> delta_point;
		float b = 2.f * (delta_point <dot> (cpoint.a - circle.center));
		float c = (circle.center <dot> circle.center) - 2.f * (circle.center <dot> cpoint.a) + (cpoint.a <dot> cpoint.a) - circle.radius * circle.radius;
		float bb4ac = b * b - 4.f * a * c;
		if (std::abs(a) < constants::epsilonf || bb4ac < 0.f)
			{
			return std::nullopt;
			}
		float μ1 = (-b + std::sqrt(bb4ac)) / (2.f * a);
		float μ2 = (-b - std::sqrt(bb4ac)) / (2.f * a);

		vec2f chosen_one;

		if (μ1 >= 1.f && μ2 >= 1.f) { return std::nullopt; }
		if (μ1 <= 1.f && μ2 <= 1.f)
			{
			vec2f r1 = cpoint.a + (cpoint.b - cpoint.a) * μ1;
			vec2f r2 = cpoint.a + (cpoint.b - cpoint.a) * μ2;

			if (vec2f::distance2(r1, cpoint.a) < vec2f::distance2(r2, cpoint.a)) { chosen_one = r1; }
			else { chosen_one = r2; }

			}
		else
			{
			float μ{std::min(μ1, μ2)};
			chosen_one = cpoint.a + (cpoint.b - cpoint.a) * μ;
			}
		collision_data ret
			{
			.normal{(chosen_one - circle.center).normalize() * (inside ? -1.f : 1.f)},
			.impact_point{chosen_one},
			.t{vec2f::distance(cpoint.a, chosen_one) / cpoint.length()}//std::sqrt(vec2f::distance(chosen_one, circle.center)) / std::sqrt(vec2f::distance(cpoint.a, circle.center))}
			};

		return ret;
#pragma region Chosen One reference
//            .
//            |\
//            | \
//            |  \
//            |   \
//            |    \
//            |     \
//            |      \
//            |       \       ____O
//            |        \     .' ./
//            |   _.,-~"\  .',/~'
//            <-~"   _.,-~" ~ |
//^"~-,._.,-~"^"~-,._\       /,._.,-~"^"~-,._.,-~"^"~-,._
//~"^"~-,._.,-~"^"~-,._.,-~"^"~-,._.,-~"^"~-,._.,-~"^"~-,._
//^"~-,._.,-~"^"~-,._.,-~"^"~-,._.,-~"^"~-,._.,-~"^"~-,._
//~-,._.,-~"^"~-,._.,-~"^"~-,._.,-~"^"~-,._.,-~"^"~-,._
#pragma endregion Chosen One reference
		}

	}