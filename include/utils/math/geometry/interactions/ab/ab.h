#pragma once

#include "base.h"
#include "point.h"

namespace utils::math::geometry::interactions
	{
	namespace return_types
		{
		struct percentages
			{
			float b_to_reach_a{utils::math::constants::finf};
			float a_to_reach_b{utils::math::constants::finf};
			};
		}

	return_types::percentages percentages(const shape::concepts::ab auto& ab_a, const shape::concepts::ab auto& ab_b) noexcept
		{//https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
		const vec2f s1{ab_a.a_to_b()};
		const vec2f s2{ab_b.a_to_b()};
	
		const float b_percentage_to_reach_a{(-s1.y() * (ab_a.a.x() - ab_b.a.x()) + s1.x() * (ab_a.a.y() - ab_b.a.y())) / (-s2.x() * s1.y() + s1.x() * s2.y())};
		const float a_percentage_to_reach_b{( s2.x() * (ab_a.a.y() - ab_b.a.y()) - s2.y() * (ab_a.a.x() - ab_b.a.x())) / (-s2.x() * s1.y() + s1.x() * s2.y())};
		
		return {b_percentage_to_reach_a, a_percentage_to_reach_b};
		}

	utils_gpu_available constexpr bool intersects(const shape::concepts::line auto& a, const shape::concepts::line auto& b) noexcept
		{
		return a.forward().angle() != b.forward().angle();
		}
	
	utils_gpu_available constexpr bool intersects(const shape::concepts::ab auto& a, const shape::concepts::ab auto& b) noexcept
		{
		const auto percents{percentages(a, b)};
		if constexpr (decltype(a)::ends.is_a_finite()) { if (percents.a_to_reach_b < 0.f) { return false; } }
		if constexpr (decltype(a)::ends.is_b_finite()) { if (percents.a_to_reach_b > 1.f) { return false; } }
		if constexpr (decltype(b)::ends.is_a_finite()) { if (percents.b_to_reach_a < 0.f) { return false; } }
		if constexpr (decltype(b)::ends.is_b_finite()) { if (percents.b_to_reach_a > 1.f) { return false; } }
		return intersects(shape::line{a.a, a.b}, shape::line{b.a, b.b});
		}

	std::optional<vec2f> intersection(const shape::concepts::ab auto& a, const shape::concepts::ab auto& b) noexcept
		{
		if (!intersects(a, b)) { return std::nullopt; }

		const vec2f s1{a.a_to_b()};
		const vec2f s2{b.a_to_b()};
		const auto percents{percentages(a, b)};

		return vec2f{a.x() + (percents.a_to_reach_b * s1.x()), a.y() + (percents.a_to_reach_b * s1.y())};
		}

	utils_gpu_available constexpr bool intersects(const shape::concepts::segment auto& a, const shape::concepts::line auto& b) noexcept
		{
		const auto point_side_a{side(b, a.a)};
		const auto point_side_b{side(b, a.b)};
		return (point_side_a != point_side_b) || (point_side_a == point_side_b && point_side_a.is_coincident());
		}

	utils_gpu_available constexpr bool intersects(const shape::concepts::segment auto& a, const shape::concepts::segment auto& b) noexcept
		{
		const auto a_intersects_b_line{intersects(a, shape::line{b.a, b.b})};
		const auto b_intersects_a_line{intersects(b, shape::line{a.a, a.b})};
		return a_intersects_b_line || b_intersects_a_line;
		}
	}