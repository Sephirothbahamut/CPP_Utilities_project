#pragma once

#include "base.h"

namespace utils::math::geometry::interactions
	{
	template <bool clamp_a, bool clamp_b>
	utils_gpu_available constexpr shape::point closest_point(const shape::concepts::ab auto& ab, const vec2f& point) noexcept
		{
		const vec2f delta{ab.b - ab.a};
		const float t{ab.projected_percent(point)};
		if constexpr (clamp_a) { if (t < 0.f) { return ab.a; } }
		if constexpr (clamp_b) { if (t > 1.f) { return ab.b; } }
		return {ab.a.x() + t * delta.x(), ab.a.y() + t * delta.y()};
		}

	template <bool clamp_a, bool clamp_b>
	utils_gpu_available constexpr float minimum_distance(const shape::concepts::ab auto& ab, const vec2f& point) noexcept
		{
		if constexpr (clamp_a || clamp_b)
			{
			const float t{ab.projected_percent<false, false>(point)};
			if constexpr (clamp_a) { if (t < 0.f) { return vec2f::distance(ab.a, point); } }
			if constexpr (clamp_b) { if (t > 1.f) { return vec2f::distance(ab.b, point); } }
			}
		const auto tmp_0{ab.some_significant_name_ive_yet_to_figure_out(point)};
		const auto tmp_1{ab.a_to_b()};
		const auto tmp_2{tmp_1.get_length()};
		const auto tmp_3{tmp_0 / tmp_2};
		return std::abs(tmp_3);
		}

	utils_gpu_available constexpr return_types::side side(const shape::concepts::ab auto& ab, const vec2f& point) noexcept
		{
		return {ab.some_significant_name_ive_yet_to_figure_out(point)};
		}

	template <bool clamp_a, bool clamp_b>
	utils_gpu_available constexpr return_types::signed_distance signed_distance(const shape::concepts::ab auto& ab, const vec2f& point) noexcept
		{
		if constexpr (clamp_a || clamp_b)
			{
			const float t{ab.projected_percent<false, false>(point)};
			if constexpr (clamp_a) { if (t < 0.f) { return {vec2f::distance(ab.a, point) * side(ab, point).sign()}; } }
			if constexpr (clamp_b) { if (t > 1.f) { return {vec2f::distance(ab.b, point) * side(ab, point).sign()}; } }
			}
		const auto tmp_0{ab.some_significant_name_ive_yet_to_figure_out(point)};
		const auto tmp_1{ab.a_to_b()};
		const auto tmp_2{tmp_1.get_length()};
		const auto tmp_3{tmp_0 / tmp_2};
		return {tmp_3};
		}

	template <bool clamp_a, bool clamp_b>
	utils_gpu_available constexpr return_types::closest_point_with_distance closest_with_distance(const shape::concepts::ab auto& ab, const vec2f& point) noexcept
		{
		const auto closest{closest_point<clamp_a, clamp_b>(ab, point)};
		return {minimum_distance<clamp_a, clamp_b>(ab, point), closest};
		}

	template <bool clamp_a, bool clamp_b>
	utils_gpu_available constexpr return_types::closest_point_with_signed_distance closest_with_signed_distance(const shape::concepts::ab auto& ab, const vec2f& point) noexcept
		{
		const auto closest{closest_point<clamp_a, clamp_b>(ab, point)};
		return {signed_distance<clamp_a, clamp_b>(ab, point), closest};
		}


	utils_gpu_available constexpr shape::point                                     closest_point               (const shape::concepts::line    auto& ab, const vec2f& point) noexcept { return closest_point               <false, false>(ab, point); }
	utils_gpu_available constexpr float                                            minimum_distance            (const shape::concepts::line    auto& ab, const vec2f& point) noexcept { return minimum_distance            <false, false>(ab, point); }
	utils_gpu_available constexpr return_types::signed_distance                    signed_distance             (const shape::concepts::line    auto& ab, const vec2f& point) noexcept { return signed_distance             <false, false>(ab, point); }
	utils_gpu_available constexpr return_types::closest_point_with_distance        closest_with_distance       (const shape::concepts::line    auto& ab, const vec2f& point) noexcept { return closest_with_distance       <false, false>(ab, point); }
	utils_gpu_available constexpr return_types::closest_point_with_signed_distance closest_with_signed_distance(const shape::concepts::line    auto& ab, const vec2f& point) noexcept { return closest_with_signed_distance<false, false>(ab, point); }

	utils_gpu_available constexpr shape::point                                     closest_point               (const shape::concepts::ray     auto& ab, const vec2f& point) noexcept { return closest_point               <true , false>(ab, point); }
	utils_gpu_available constexpr float                                            minimum_distance            (const shape::concepts::ray     auto& ab, const vec2f& point) noexcept { return minimum_distance            <true , false>(ab, point); }
	utils_gpu_available constexpr return_types::signed_distance                    signed_distance             (const shape::concepts::ray     auto& ab, const vec2f& point) noexcept { return signed_distance             <true , false>(ab, point); }
	utils_gpu_available constexpr return_types::closest_point_with_distance        closest_with_distance       (const shape::concepts::ray     auto& ab, const vec2f& point) noexcept { return closest_with_distance       <true , false>(ab, point); }
	utils_gpu_available constexpr return_types::closest_point_with_signed_distance closest_with_signed_distance(const shape::concepts::ray     auto& ab, const vec2f& point) noexcept { return closest_with_signed_distance<true , false>(ab, point); }
	
	utils_gpu_available constexpr shape::point                                     closest_point               (const shape::concepts::segment auto& ab, const vec2f& point) noexcept { return closest_point               <true , true >(ab, point); }
	utils_gpu_available constexpr float                                            minimum_distance            (const shape::concepts::segment auto& ab, const vec2f& point) noexcept { return minimum_distance            <true , true >(ab, point); }
	utils_gpu_available constexpr return_types::signed_distance                    signed_distance             (const shape::concepts::segment auto& ab, const vec2f& point) noexcept { return signed_distance             <true , true >(ab, point); }
	utils_gpu_available constexpr return_types::closest_point_with_distance        closest_with_distance       (const shape::concepts::segment auto& ab, const vec2f& point) noexcept { return closest_with_distance       <true , true >(ab, point); }
	utils_gpu_available constexpr return_types::closest_point_with_signed_distance closest_with_signed_distance(const shape::concepts::segment auto& ab, const vec2f& point) noexcept { return closest_with_signed_distance<true , true >(ab, point); }
	}