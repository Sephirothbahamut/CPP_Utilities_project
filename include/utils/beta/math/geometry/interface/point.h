#pragma once

#include "../common.h"

namespace utils::math::geometry::shape::interface
	{
	template <typename derived_t>
	struct point : details::base<derived_t>
		{
		using crtp = details::base<derived_t>::crtp;
		utils_gpu_available constexpr const vec2f& value() const noexcept { crtp::unimplemented_interface(); return {}; }

		//common
		utils_gpu_available constexpr vec2f                     closest_point_to       (const concepts::any auto& other) noexcept { return value(); };
		utils_gpu_available constexpr vec2f                     vector_to              (const concepts::any auto& other) noexcept { return other.closest_point_to(crtp::derived()) - crtp::derived().value(); };
		utils_gpu_available constexpr bool                      intersects             (const concepts::any auto& other) noexcept { return crtp::derived().side_of(other) == side::coincident(); };

		//point
		float distance(const oop::concepts::derived_from_crtp<point> auto& other) noexcept { return (other.vec2f() - derived().vec2f()).magnitude(); };
		
		utils_gpu_available constexpr float                     distance_to            (const concepts::point   auto& other) noexcept { return crtp::derived().vector_to(other).length(); };
		utils_gpu_available constexpr bool                      contains               (const concepts::point   auto& other) noexcept { return crtp::derived().intersects(other); };
		utils_gpu_available constexpr bool                      collides_with          (const concepts::point   auto& other) noexcept { return crtp::derived().value() == other.value(); };

		//segment
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
	struct point : utils::math::geometry::shape::interface::point<point>
		{
		::utils::math::vec2f vec;
		utils_gpu_available constexpr const auto value() const noexcept { return vec; }
		};

	namespace view
		{
		struct point : utils::math::geometry::shape::interface::point<point>
			{
			::utils::math::vecref2f vec;
			utils_gpu_available constexpr const auto& value() const noexcept { return vec; }
			};
		}
	}