#pragma once

#include "../common.h"
#include "point.h"

namespace utils::math::geometry::shape::interface
	{
	template <typename derived_t>
	struct circle : details::base<derived_t>
		{
		using crtp = details::base<derived_t>::crtp;
		utils_gpu_available constexpr view ::point center_point()       noexcept { return {crtp::derived().center}; }
		utils_gpu_available constexpr shape::point center_point() const noexcept { return {crtp::derived().center}; }
		
		#pragma region common
			utils_gpu_available constexpr float                     distance            (const concepts::any auto& other) const noexcept { return default_distance              (other); }
			utils_gpu_available constexpr vec2f                     closest_point       (const concepts::any auto& other) const noexcept { return default_closest_point         (other); }
			utils_gpu_available constexpr closest_point_distance_t  closest_and_distance(const concepts::any auto& other) const noexcept { return default_closest_and_distance  (other); }
			utils_gpu_available constexpr closest_points_distance_t closest_pair        (const concepts::any auto& other) const noexcept { return other_closest_pair_inverted   (other); }
			utils_gpu_available constexpr vec2f                     vector_to           (const concepts::any auto& other) const noexcept { return other_vector_to_inverted      (other); }
			utils_gpu_available constexpr bool                      intersects          (const concepts::any auto& other) const noexcept { return other_intersects              (other); }
			utils_gpu_available constexpr std::optional<vec2f>      intersection        (const concepts::any auto& other) const noexcept { return other_intersection            (other); }
			utils_gpu_available constexpr bool                      contains            (const concepts::any auto& other) const noexcept { return default_contains              (other); }
			utils_gpu_available constexpr bool                      collides_with       (const concepts::any auto& other) const noexcept { return other_collides_with           (other); }
		#pragma endregion common
		#pragma region point
			utils_gpu_available constexpr geometry::side            side                (const concepts::point auto& other) const noexcept { return default_side                  (other); }
			utils_gpu_available constexpr float                     distance            (const concepts::point auto& other) const noexcept { return default_distance              (other); }
			utils_gpu_available constexpr distance_signed           distance_signed     (const concepts::point auto& other) const noexcept { return center_point().distance(other) - crtp::derived().radius; }
			utils_gpu_available constexpr vec2f                     closest_point       (const concepts::point auto& other) const noexcept 
				{
				auto vec{center_point().vector_to(other)};
				vec.length = crtp::derived().radius;
				return vec;
				}
			utils_gpu_available constexpr closest_point_distance_t  closest_and_distance(const concepts::point auto& other) const noexcept { return default_closest_and_distance  (other); }
			utils_gpu_available constexpr closest_points_distance_t closest_pair        (const concepts::point auto& other) const noexcept { return other_closest_pair_inverted   (other); }
			utils_gpu_available constexpr vec2f                     vector_to           (const concepts::point auto& other) const noexcept 
				{
				auto vec{center_point().vector_to(other)};
				vec.length -= crtp::derived().radius;
				return vec;
				}
			utils_gpu_available constexpr bool                      intersects          (const concepts::point auto& other) const noexcept { return other_intersects              (other); }
			utils_gpu_available constexpr std::optional<vec2f>      intersection        (const concepts::point auto& other) const noexcept { return other_intersection            (other); }
			utils_gpu_available constexpr bool                      contains            (const concepts::point auto& other) const noexcept { return default_contains              (other); }
			utils_gpu_available constexpr bool                      collides_with       (const concepts::point auto& other) const noexcept { return other_collides_with           (other); }
		#pragma endregion point
		};
	}

namespace utils::math::geometry::shape
	{
	struct circle : utils::math::geometry::shape::interface::circle<circle>
		{
		::utils::math::vec2f center;
		float radius;
		};

	namespace view
		{
		struct circle : utils::math::geometry::shape::interface::circle<circle>
			{
			::utils::math::vecref2f center;
			std::reference_wrapper<float> radius;
			};
		}
	}