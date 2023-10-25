#pragma once

#include "../common.h"


namespace utils::math::geometry::shape::interface
	{
	template <typename derived_t>
	struct line : details::base<derived_t>
		{
		using crtp = details::base<derived_t>::crtp;
		utils_gpu_available constexpr const vec2f& a() const noexcept { crtp::unimplemented_interface(); return {}; }
		utils_gpu_available constexpr const vec2f& b() const noexcept { crtp::unimplemented_interface(); return {}; }

		//point
		utils_gpu_available constexpr side                      side_of                (const concepts::segment auto& other) noexcept { return crtp::derived().signed_distance_to(other).side    (); };
		utils_gpu_available constexpr float                     distance_to            (const concepts::segment auto& other) noexcept { return crtp::derived().signed_distance_to(other).distance(); };
		utils_gpu_available constexpr signed_distance           signed_distance_to     (const concepts::segment auto& other) noexcept { return {crtp::derived().distance_to(other) * crtp::derived().side_of(other)}; };
		utils_gpu_available constexpr vec2f                     closest_point_to       (const concepts::segment auto& other) noexcept { return crtp::derived().closest_and_distance(other).closest ; };
		utils_gpu_available constexpr closest_point_distance_t  closest_and_distance_to(const concepts::segment auto& other) noexcept { return {crtp::derived().closest_point_to(other), crtp::derived().signed_distance(other)}; };
		utils_gpu_available constexpr closest_points_distance_t closest_pair           (const concepts::segment auto& other) noexcept { return {crtp::derived().closest_point_to(other), other.closest_point_to(crtp::derived()), crtp::derived().signed_distance(other)}; };
		utils_gpu_available constexpr vec2f                     vector_to              (const concepts::segment auto& other) noexcept { return other.closest_point_to(crtp::derived()) - crtp::derived().closest_point_to(other); };
		utils_gpu_available constexpr bool                      intersects             (const concepts::segment auto& other) noexcept { return crtp::derived().intersection(other).has_value(); };
		utils_gpu_available constexpr std::optional<vec2f>      intersection           (const concepts::segment auto& other) noexcept { return crtp::derived().intersects(other) ? crtp::derived().closest_point_to(other) : std::nullopt; };
		utils_gpu_available constexpr bool                      contains               (const concepts::segment auto& other) noexcept { return other.distance_signed(crtp::derived()) <= 0; };
		utils_gpu_available constexpr bool                      collides_with          (const concepts::segment auto& other) noexcept { return crtp::derived().intersects(other) || crtp::derived().contains(other); };
		};
	}

namespace utils::beta::math::geometry::shape
	{
	namespace view
		{
		struct line : utils::math::geometry::shape::interface::line<line>
			{
			::utils::math::vec2f point_a;
			::utils::math::vec2f point_b;
			utils_gpu_available constexpr const auto& a() const noexcept { return point_a; }
			utils_gpu_available constexpr const auto& b() const noexcept { return point_b; }
			};
		}

	struct line : utils::math::geometry::shape::interface::line<line>
		{
		::utils::math::vecref2f point_a;
		::utils::math::vecref2f point_b;
		utils_gpu_available constexpr const auto& a() const noexcept { return point_a; }
		utils_gpu_available constexpr const auto& b() const noexcept { return point_b; }
		};
	}