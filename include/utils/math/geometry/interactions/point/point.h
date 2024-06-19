#pragma once

#include "base.h"

namespace utils::math::geometry::interactions
	{
	utils_gpu_available constexpr shape::point closest_point(const shape::concepts::point auto& point, const vec2f&) noexcept
		{
		return point; 
		}

	utils_gpu_available constexpr float minimum_distance(const shape::concepts::point auto& point, const vec2f& other) noexcept
		{
		return vec2f::distance(point, other);
		}

	utils_gpu_available constexpr return_types::side side(const shape::concepts::point auto& point, const vec2f& other) noexcept
		{
		return ((*this) == other) ? return_types::side::create::coincident() : return_types::side::create::outside();
		}

	utils_gpu_available constexpr return_types::signed_distance signed_distance(const shape::concepts::point auto& point, const vec2f& other) noexcept
		{
		return {minimum_distance(point, other)};
		}

	utils_gpu_available constexpr return_types::closest_point_with_distance closest_with_distance(const shape::concepts::point auto& point, const vec2f& other) noexcept
		{
		return {point, minimum_distance(point, other)};
		}

	utils_gpu_available constexpr return_types::closest_point_with_signed_distance closest_with_signed_distance(const shape::concepts::point auto& point, const vec2f& other) noexcept
		{
		return {point, minimum_distance(point, other)};
		}
	}