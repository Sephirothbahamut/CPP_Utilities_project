#pragma once

#include "../common.h"
#include "../details/ab_types.h"

namespace utils::math::geometry::shape::interface
	{
	template <typename derived_t>
	struct line : utils::math::geometry::shape::details::ab_types::interface<derived_t>
		{
		using crtp = details::base<derived_t>::crtp;

		#pragma region point
			utils_gpu_available constexpr float distance(const concepts::point auto& other) const noexcept { details::base<derived_t>::default_distance(); }
			utils_gpu_available constexpr distance_signed distance_signed(const concepts::point auto& other) const noexcept { return some_significant_name_ive_yet_to_figure_out(other); }

			utils_gpu_available constexpr vec2f closest_point(const concepts::point auto& other) noexcept { return closest_point_custom_clamp<false, false>(other); }
		#pragma endregion point
		};
	}

namespace utils::math::geometry::shape
	{
	struct line : utils::math::geometry::shape::interface::line<line>, utils::math::geometry::shape::details::ab_types::view {};

	namespace view
		{
		struct line : utils::math::geometry::shape::interface::line<line>, utils::math::geometry::shape::details::ab_types::data {};
		}
	}
			