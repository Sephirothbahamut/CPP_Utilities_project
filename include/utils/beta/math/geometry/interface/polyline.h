#pragma once

#include "../common.h"
#include "../details/points_types.h"

namespace utils::math::geometry::shape::interface
	{
	template <typename derived_t>
	struct polyline : utils::math::geometry::shape::details::points_types::interface<derived_t>
		{
		using crtp = details::base<derived_t>::crtp;
		using utils::math::geometry::shape::details::points_types::interface<derived_t>::get_points;
		using utils::math::geometry::shape::details::points_types::interface<derived_t>::get_edges;

		#pragma region any
		utils_gpu_available constexpr closest_point_distance_t  closest_and_distance(const concepts::any auto& other) const noexcept
			{
			closest_point_distance_t closest_current;
			for (const auto& edge : get_edges()) { closest_current = closest_point_distance_t::pick_closest(closest_current, edge.closest_and_distance(other)); }
			return closest_current;
			}
		#pragma endregion any
		#pragma region    point
		#pragma endregion point
		};
	}

namespace utils::math::geometry::shape
	{
	struct polyline : utils::math::geometry::shape::interface::polyline<polyline>, utils::math::geometry::shape::details::points_types::data
		{
		using utils::math::geometry::shape::details::points_types::data::data;
		utils_gpu_available polyline(std::initializer_list<utils::math::vec2f> points) : utils::math::geometry::shape::details::points_types::data{points} {}
		};

	namespace view
		{
		struct polyline : utils::math::geometry::shape::interface::polyline<polyline>, utils::math::geometry::shape::details::points_types::view
			{
			using utils::math::geometry::shape::details::points_types::view::view;
			utils_gpu_available polyline(std::span<utils::math::vec2f> span) : utils::math::geometry::shape::details::points_types::view{span} {}
			};
		}
	}