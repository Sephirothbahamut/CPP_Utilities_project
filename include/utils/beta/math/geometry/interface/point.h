#pragma once

#include "../common.h"

namespace utils::math::geometry::shape::interface
	{
	template <typename derived_t>
	struct point : details::base<derived_t>
		{
		using crtp = details::base<derived_t>::crtp;
		
		#pragma region common
			utils_gpu_available constexpr geometry::side            side                (const concepts::any auto& other) const noexcept { return side::out(); }
			utils_gpu_available constexpr vec2f                     closest_point       (const concepts::any auto& other) const noexcept { return crtp::derived(); }
			utils_gpu_available constexpr vec2f                     vectorto            (const concepts::any auto& other) const noexcept { return other.closest_point(crtp::derived()) - crtp::derived(); }
			utils_gpu_available constexpr bool                      intersects          (const concepts::any auto& other) const noexcept { return crtp::derived().distance(other) == 0.f; }
			utils_gpu_available constexpr bool                      contains            (const concepts::any auto& other) const noexcept { return false; }
			utils_gpu_available constexpr distance_signed           distance_signed     (const concepts::any auto& other) const noexcept { return crtp::derived().distance(other); }
		#pragma endregion common

		#pragma region point
			float distance(const oop::concepts::derived_from_crtp<point> auto& other) const noexcept { return (other.vec2f() - crtp::derived().vec2f()).magnitude(); }
		
			utils_gpu_available constexpr float                     distance            (const concepts::point   auto& other) const noexcept { return crtp::derived().vector_to(other).length(); }
			utils_gpu_available constexpr bool                      collides_with       (const concepts::point   auto& other) const noexcept { return crtp::derived().vec == other.vec; }
		#pragma endregion point
		};
	}

namespace utils::math::geometry::shape
	{
	struct point : utils::math::geometry::shape::interface::point<point>, ::utils::math::vec2f { using ::utils::math::vec2f::vec2f; };

	namespace view
		{
		struct point : utils::math::geometry::shape::interface::point<point>, ::utils::math::vecref2f { using ::utils::math::vecref2f::vecref2f; };
		}
	}