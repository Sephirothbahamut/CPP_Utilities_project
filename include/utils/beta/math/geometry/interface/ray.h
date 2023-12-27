#pragma once

#include "../common.h"
#include "../details/ab_types.h"

namespace utils::math::geometry::shape::interface
	{
	template <typename derived_t>
	struct ray : utils::math::geometry::shape::details::ab_types::interface<derived_t>
		{
		using crtp = details::base<derived_t>::crtp;

		#pragma region point
			utils_gpu_available constexpr float distance(const concepts::point auto& other) const noexcept
				{// from shadertoy example linked below
				const utils::math::vec2f b_a{crtp::derived().b() - crtp::derived().a()};
				const utils::math::vec2f p_a{other               - crtp::derived().a()};
				float h{std::max(utils::math::vec2f::dot(p_a, b_a) / utils::math::vec2f::dot(b_a, b_a), 0.f)};
				return (p_a - (b_a * h)).get_length();
				}
			utils_gpu_available constexpr geometry::distance_signed distance_signed(const concepts::point auto& other) const noexcept { return details::base<derived_t>::default_distance_signed(other); }

			utils_gpu_available constexpr vec2f closest_point(const concepts::point auto& other) noexcept { return closest_point_custom_clamp<true, false>(other); }
		#pragma endregion point
		};
	}

namespace utils::math::geometry::shape
	{
	struct ray : utils::math::geometry::shape::interface::ray<ray>
		{
		utils_gpu_available ray(const ::utils::math::vec2f& point_a, const ::utils::math::vec2f& point_b) : point_a{point_a}, point_b{point_b} {}
		::utils::math::vec2f point_a;
		::utils::math::vec2f point_b;
		utils_gpu_available constexpr const auto& a() const noexcept { return point_a; }
		utils_gpu_available constexpr const auto& b() const noexcept { return point_b; }
		};

	namespace view
		{
		struct ray : utils::math::geometry::shape::interface::ray<ray>, utils::math::geometry::shape::details::ab_types::view {};
		}
	}