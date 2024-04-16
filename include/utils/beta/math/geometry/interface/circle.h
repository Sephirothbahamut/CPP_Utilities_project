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
			utils_gpu_available constexpr side                           side_of              (const concepts::any auto& other) const noexcept { return crtp::derived().side_from_distance_signed                      (other); }
			utils_gpu_available constexpr vec2f                          closest_point_to     (const concepts::any auto& other) const noexcept { return crtp::derived().closest_from_closest_and_distance              (other); }
			utils_gpu_available constexpr closest_point_with_distance_t  closest_with_distance(const concepts::any auto& other) const noexcept { return crtp::derived().closest_with_distance_from_closest_and_distance(other); }
			utils_gpu_available constexpr closest_pair_with_distance_t   closest_pair         (const concepts::any auto& other) const noexcept { return crtp::derived().closest_pair_from_closest_and_other_closest    (other); }
			utils_gpu_available constexpr vec2f                          vector_to            (const concepts::any auto& other) const noexcept { return crtp::derived().vector_to_from_closest_pair                    (other); }
			utils_gpu_available constexpr std::optional<vec2f>           intersection_with    (const concepts::any auto& other) const noexcept { return crtp::derived().intersection_from_intersects_and_closest       (other); }
			utils_gpu_available constexpr bool                           contains             (const concepts::any auto& other) const noexcept { return crtp::derived().contains_from_side                             (other); }
			utils_gpu_available constexpr bool                           collides_with        (const concepts::any auto& other) const noexcept { return crtp::derived().other_collides_with                            (other); } 
			
			//utils_gpu_available constexpr bool                           intersects           (const concepts::any auto& other) const noexcept { crtp::unimplemented_interface(); return {}; }

		#pragma endregion common

		#pragma region point
			utils_gpu_available constexpr signed_distance_t              distance             (const concepts::point auto& other) const noexcept { return {center_point().distance(other).value - crtp::derived().radius}; }
			utils_gpu_available constexpr vec2f                          closest_point_to     (const concepts::point auto& other) const noexcept 
				{
				auto vec{center_point().vector_to(other)};
				vec.length = crtp::derived().radius;
				return crtp::derived().center + vec;
				}
		#pragma endregion point
			
		#pragma region circle
			utils_gpu_available constexpr signed_distance_t              distance             (const concepts::circle auto& other) const noexcept { return {center_point().distance(other).value - crtp::derived().radius}; }
			utils_gpu_available constexpr vec2f                          closest_point_to     (const concepts::circle auto& other) const noexcept
				{
				auto vec{center_point().vector_to(other)};
				vec.length = crtp::derived().radius;
				return crtp::derived().center + vec;
				}
			
			utils_gpu_available constexpr side side_of(const concepts::circle auto& other) const noexcept 
				{
				const auto centers_distance{center_point().distance(other.center_point())};
				if (centers_distance <= (crtp::derived().radius - other.radius)) { return side::create::outside   (); } // other is inside *this
				if (centers_distance <= (other.radius - crtp::derived().radius)) { return side::create::inside    (); }
				if (centers_distance <  (other.radius + crtp::derived().radius)) { return side::create::coincident(); }
				return side::create::outside();
				}

			utils_gpu_available constexpr std::optional<vec2f> intersection_with(const concepts::circle auto& other) const noexcept 
				{
				crtp::unimplemented_interface(); //TODO
				return {};
				}

		#pragma endregion circle

		#pragma region segment
			utils_gpu_available constexpr bool intersects(const concepts::segment auto& other) const noexcept
				{
				if ( contains(other.a) && !contains(other.b)) { return true; }
				if ( contains(other.b) && !contains(other.a)) { return true; }
				if (!contains(other.b) && !contains(other.a)) { return other.distance(center_point()) <= crtp::derived().radius; }
				return false;
				}
		#pragma endregion segment
		};
	}

namespace utils::math::geometry::shape
	{
	struct circle : utils::math::geometry::shape::interface::circle<circle>
		{
		circle(utils::math::vec2f center, float radius) : center{center}, radius{radius} {}
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
