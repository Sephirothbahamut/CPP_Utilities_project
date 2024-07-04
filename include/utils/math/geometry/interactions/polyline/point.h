#pragma once

#include "base.h"
#include "../point.h"
#include "../ab.h"

namespace utils::math::geometry::interactions
	{
	utils_gpu_available constexpr shape::point closest_point(const shape::concepts::polyline auto& polyline, const vec2f& point) noexcept
		{
		return closest_with_distance(polyline, point).closest;
		}

	utils_gpu_available constexpr float minimum_distance(const shape::concepts::polyline auto& polyline, const vec2f& point) noexcept
		{
		float ret{utils::math::constants::finf};
		for (const auto edge : polyline.get_edges())
			{
			ret = std::min(ret, minimum_distance(edge, point));
			}
		return ret;
		}

	utils_gpu_available constexpr return_types::closest_point_with_distance closest_with_distance(const shape::concepts::polyline auto& polyline, const vec2f& point) noexcept
		{
		return_types::closest_point_with_distance ret;
		for (const auto edge : polyline.get_edges())
			{
			ret.set_to_closest(closest_with_distance(edge, point));
			}
		return ret;
		}

	utils_gpu_available constexpr return_types::closest_point_with_signed_distance closest_with_signed_distance(const shape::concepts::polyline auto& polyline, const vec2f& point) noexcept
		{
		const auto edges{polyline.get_edges()};
		if (edges.empty()) { return {}; }

		// First edge, special case for infinite start polylines
		auto ret{[&]() -> return_types::closest_point_with_signed_distance
			{
			if constexpr (polyline.ends.is_a_infinite())
				{
				const shape::ray start_ray{edges[0].b, edges[0].a}; //invert so that it goes to infinity backwards
				const auto tmp{closest_with_signed_distance(start_ray, point)};
				return {tmp.closest, -tmp.distance};
				}
			else
				{
				const auto candidate_edge{edges[0]};
				return closest_with_signed_distance(candidate_edge, point);
				}
			}()};
		size_t closest_index{0};

		for (size_t i{1}; i < edges.size() - 1; i++)
			{
			const auto candidate_edge{edges[i]};
			const auto candidate{closest_with_signed_distance(candidate_edge, point)};
			if (candidate.distance.absolute() < ret.distance.absolute())
				{
				ret = candidate;
				closest_index = i;
				}
			}

		const size_t last_edge_index{edges.size() - 1};
		using last_edge_t = std::conditional_t<polyline.ends.is_b_infinite(), shape::ray, shape::segment>;
		const last_edge_t last_edge{edges[last_edge_index]};
		const auto last_candidate{closest_with_signed_distance(last_edge, point)};

		// in closed shapes if first and last are equidistant to the point, use the last edge as closest
		// so we have the correct first-second edge pair for the corner case evaluated later 
		// to achieve that, the previous edges check for distance.absolute() <, while the last one checks for <=, so it can replace the closest index
		// (getting correct side when two edges are equidistant from the point)

		const bool last_replace{[&]()
			{
			if constexpr (polyline.ends.is_closed())
				{
				return last_candidate.distance.absolute() <= ret.distance.absolute();
				}
			else
				{
				return last_candidate.distance.absolute() < ret.distance.absolute();
				}
			}()};

		if (last_replace)
			{
			ret = last_candidate;
			closest_index = last_edge_index;
			}

		if constexpr (polyline.ends.is_closed())
			{
			if (ret.closest == edges[closest_index].b)
				{
				//Common closest means the closest is a corner, we have to check for the weird regions (see "side_corner_case_visualization.png")
				//pick the farthest infinite line (not segment) to get the correct side
				const shape::line line_previous{edges[closest_index                                            ]};
				const shape::line line_next    {edges[polyline.raw_index_to_ends_aware_index(closest_index + 1)]};
		
				const return_types::signed_distance line_signed_distance_previous{signed_distance(line_previous, point)};
				const return_types::signed_distance line_signed_distance_next    {signed_distance(line_next    , point)};
		
				const auto side{line_signed_distance_previous.absolute() > line_signed_distance_next.absolute() ? line_signed_distance_previous.side() : line_signed_distance_next.side()};
				ret.distance = {ret.distance.absolute() * side};
				}
			}

		return ret;
		}

	utils_gpu_available constexpr return_types::side side(const shape::concepts::polyline auto& polyline, const vec2f& point) noexcept
		{
		return closest_with_signed_distance(polyline, point).distance.side();
		}

	utils_gpu_available constexpr return_types::signed_distance signed_distance(const shape::concepts::polyline auto& polyline, const vec2f& point) noexcept
		{
		return closest_with_signed_distance(polyline, point).distance;
		}
	}