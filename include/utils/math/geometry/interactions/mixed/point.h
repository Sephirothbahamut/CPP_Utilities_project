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

		return_types::closest_point_with_signed_distance ret;
		size_t closest_index{0};

		for (size_t i{0}; i < edges.size(); i++)
			{
			const auto candidate_edge{edges[i]};
			const auto candidate{closest_with_signed_distance(candidate_edge, point)};
			if (candidate.distance.absolute() < ret.distance.absolute())
				{
				ret = candidate;
				closest_index = i;
				}
			else if (candidate.distance.absolute() == ret.distance.absolute() && i == (edges.size() - 1))
				{//if first and last are equidistant to the point, it means the "first" edge clockwise is the last edge
				closest_index = i;
				}
			}

		if (ret.closest == edges[closest_index].b)
			{
			//Common closest means the closest is a corner, we have to check for the weird regions (see "side_corner_case_visualization.png")
			//pick the farthest infinite line (not segment) to get the correct side
			const shape::line line_previous{edges[closest_index                  ]};
			const shape::line line_next    {edges[edges.index_next(closest_index)]};
		
			const return_types::signed_distance line_signed_distance_previous{signed_distance(line_previous, point)};
			const return_types::signed_distance line_signed_distance_next    {signed_distance(line_next    , point)};
		
			const auto side{line_signed_distance_previous.absolute() > line_signed_distance_next.absolute() ? line_signed_distance_previous.side() : line_signed_distance_next.side()};
			ret.distance = {ret.distance.absolute() * side};
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