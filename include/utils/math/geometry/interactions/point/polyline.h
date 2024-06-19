#pragma once

#include "../base_interactions.h"

namespace utils::math::geometry::interactions
	{
	namespace point
		{
		namespace polyline
			{
			/// <summary></summary>
			/// <typeparam name="extended_inverse">When there's an acute angle and two consecutive segments are both closest, pick the one that is farthest as a non-finite line, so the side is correct.</typeparam>
			/// <param name="a"></param>
			/// <param name="b"></param>
			/// <returns></returns>
			template <bool extended_inverse = false>
			utils_gpu_available constexpr shape::segment closest_segment_of(const shape::concepts::point auto& a, const shape::concepts::polyline_open auto& b) noexcept
				{
				const auto edges{b.get_edges()};

				closest_segment_with_distance_t found;

				for (const auto& edge : edges)
					{
					const closest_segment_with_distance_t candidate{edge, interactions::point::ab::distance(a, edge)};
					if constexpr (extended_inverse)
						{
						if (found.distance == candidate.distance && found.closest.b == candidate.closest.a)
							{
							const shape::line found_as_line    {found    .closest.a, found    .closest.b};
							const shape::line candidate_as_line{candidate.closest.a, candidate.closest.b};
							const auto distance_found_as_line    {interactions::point::ab::distance(a, found_as_line    )};
							const auto distance_candidate_as_line{interactions::point::ab::distance(a, candidate_as_line)};
							if (distance_found_as_line < distance_candidate_as_line) { found = candidate; }
							}
						else
							{
							found = closest_segment_with_distance_t::pick_closest_raw(found, candidate);
							}
						}
					else
						{
						found = closest_segment_with_distance_t::pick_closest_raw(found, candidate);
						}
					}

				return found.closest;
				}
			
			utils_gpu_available constexpr vec2f closest_point_of(const shape::concepts::point auto& a, const shape::concepts::polyline_open auto& b) noexcept
				{
				const auto edges{b.get_edges()};

				closest_point_with_distance_t found;

				for (const auto& edge : b.get_edges())
					{
					const vec2f point{interactions::point::ab::closest_point_of(a, edge)};
					const closest_point_with_distance_t candidate{point, interactions::point::point::distance(a, point)};
					found = closest_point_with_distance_t::pick_closest_raw(found, candidate);
					}

				return found.closest;
				}

			utils_gpu_available constexpr geometry::signed_distance_t distance_signed(const shape::concepts::point auto& a, const shape::concepts::polyline_open auto& b) noexcept
				{
				return interactions::point::ab::distance_signed(a, interactions::point::polyline::closest_segment_of<true>(a, b));
				}

			utils_gpu_available constexpr float distance(const shape::concepts::point auto& a, const shape::concepts::polyline_open auto& b) noexcept
				{
				return interactions::point::ab::distance(a, interactions::point::polyline::closest_segment_of(a, b));
				}
			}

		namespace polygon
			{
			//From inigo quilez's example https://www.shadertoy.com/view/wdBXRW
			//Sidenote: why are graphics programmers scared of using longer more explanatory variable names? Why d and s when you can type distance and sign?

			utils_gpu_available constexpr closest_segment_with_distance_t closest_segment_with_distance(const shape::concepts::point auto& a, const shape::concepts::polygon auto& b) noexcept
				{
				const auto& vertices{b.vertices};

				if (vertices.empty()) { return {}; }

				float sign{1.0f};

				closest_segment_with_distance_t found;

				for (const auto& edge : b.get_edges())
					{
					const closest_segment_with_distance_t candidate{edge, interactions::point::ab::distance(a, edge)};
					found = closest_segment_with_distance_t::pick_closest_raw(found, candidate);

					const auto edge_a_to_point{a - edge.a};
					const short conditions
						{
						(a.y >= edge.a.y) +
						(a.y <  edge.b.y) +
						((edge.a_to_b().x * edge_a_to_point.y) > (edge.a_to_b().y * edge_a_to_point.x))
						};

					if (conditions == 0 || conditions == 3) { sign = -sign; }
					}

				return {found.closest, {found.distance.value * sign}};
				}

			utils_gpu_available constexpr closest_point_with_distance_t closest_with_distance(const shape::concepts::point auto& a, const shape::concepts::polygon auto& b) noexcept
				{
				const auto& vertices{b.vertices};

				if (vertices.empty()) { return {}; }

				float sign{1.0f};

				closest_point_with_distance_t found;

				for (const auto& edge : b.get_edges())
					{
					const auto candidate{interactions::point::ab::closest_with_distance(a, edge)};
					found = closest_point_with_distance_t::pick_closest_raw(found, candidate);

					const auto edge_a_to_point{a - edge.a};
					const short conditions
						{
						(a.y >= edge.a.y) +
						(a.y <  edge.b.y) +
						((edge.a_to_b().x * edge_a_to_point.y) > (edge.a_to_b().y * edge_a_to_point.x))
						};

					if (conditions == 0 || conditions == 3) { sign = -sign; }
					}

				return {found.closest, {found.distance.value * sign}};
				}
			
			utils_gpu_available constexpr geometry::signed_distance_t distance_signed(const shape::concepts::point auto& a, const shape::concepts::polygon auto& b) noexcept
				{
				const auto& vertices{b.vertices};

				if (b.vertices.empty()) { return geometry::signed_distance_t{utils::math::constants::finf}; }

				float found_distance{utils::math::constants::finf};
				float sign{1.0f};

				for (const auto& edge : b.get_edges())
					{
					found_distance = std::min(found_distance, interactions::point::ab::distance(a, edge));

					const auto edge_a_to_point{a - edge.a};

					const short conditions
						{
						(a.y >= edge.a.y) +
						(a.y <  edge.b.y) +
						((edge.a_to_b().x * edge_a_to_point.y) > (edge.a_to_b().y * edge_a_to_point.x))
						};

					if (conditions == 0 || conditions == 3) { sign = -sign; }
					}

				return {found_distance * sign};
				}

			utils_gpu_available constexpr geometry::side side_of(const shape::concepts::point auto& a, const shape::concepts::polygon auto& b) noexcept
				{
				const auto& vertices{b.vertices};

				if (b.vertices.empty()) { return {utils::math::constants::finf}; }
				const float sign{1.0f};

				for (const auto& edge : b.get_edges())
					{
					const auto edge_a_to_point{a - edge.a};

					const short conditions
						{
						(a.y >= edge.a.y) +
						(a.y <  edge.b.y) +
						((edge.a_to_b().x * edge_a_to_point.y) > (edge.a_to_b().y * edge_a_to_point.x))
						};

					if (conditions == 0 || conditions == 3) { sign = -sign; }
					}

				return {sign};
				}
			}
		}
	}