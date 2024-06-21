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

		return_types::closest_point_with_signed_distance ret{closest_with_signed_distance(edges[1], point)};
		const auto q{ret.closest};
		const auto w{ret.distance};
		
		for (size_t i = 1; i < edges.size(); i++)
			{
			const auto candidate_edge{edges[i]};
			const auto candidate{closest_with_signed_distance(candidate_edge, point)};

			if (candidate.closest != ret.closest)
				{
				ret.set_to_closest(candidate);
				}
			else
				{
				//Common closest means the closest is a corner, we have to check for the weird regions (see "side_corner_case_visualization.png")
				//pick the farthest infinite line (not segment) to get the correct side
				const shape::line line_ret      {edges[i - 1]};
				const shape::line line_candidate{candidate_edge};

				const return_types::signed_distance line_signed_distance_ret      {signed_distance(line_ret      , point)};
				const return_types::signed_distance line_signed_distance_candidate{signed_distance(line_candidate, point)};

				ret.distance = line_signed_distance_ret.absolute() > line_signed_distance_candidate.absolute() ? line_signed_distance_ret : line_signed_distance_candidate;
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