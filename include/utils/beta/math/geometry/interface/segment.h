#pragma once

#include "../common.h"
#include "../details/ab_types.h"

namespace utils::math::geometry::shape::interface
	{
	template <typename derived_t>
	struct segment : utils::math::geometry::shape::details::ab_types::interface<derived_t>
		{
		using crtp = details::base<derived_t>::crtp;

		#pragma region point
			utils_gpu_available constexpr float distance(const concepts::point auto& other) const noexcept
				{// from shadertoy example linked below
				const utils::math::vec2f b_a{derived_t::b - derived_t::a};
				const utils::math::vec2f p_a{other - derived_t::a};
				float h{utils::math::vec2f::dot(p_a, b_a) / utils::math::vec2f::dot(b_a, b_a)};
				return (p_a - (b_a * h)).length;
				}
			utils_gpu_available constexpr signed_distance_t distance(const concepts::point auto& other) const noexcept { return default_distance_signed(other); }
			
			utils_gpu_available constexpr vec2f closest_point_to(const concepts::point auto& other) noexcept { return closest_point_custom_clamp<true, true>(other); }

		#pragma endregion point
		};
	}

namespace utils::math::geometry::shape
	{
	struct segment : utils::math::geometry::shape::interface::segment<segment>, utils::math::geometry::shape::details::ab_types::data
		{
		using utils::math::geometry::shape::details::ab_types::data::data;
		utils_gpu_available segment(const ::utils::math::vecref2f& a, const ::utils::math::vecref2f& b) : utils::math::geometry::shape::details::ab_types::data{a, b} {}
		};

	namespace view
		{
		struct segment : utils::math::geometry::shape::interface::segment<segment>, utils::math::geometry::shape::details::ab_types::view 
			{
			using utils::math::geometry::shape::details::ab_types::view::view;
			utils_gpu_available segment(const ::utils::math::vecref2f& a, const ::utils::math::vecref2f& b) : utils::math::geometry::shape::details::ab_types::view{a, b} {}
			};
		}
	}