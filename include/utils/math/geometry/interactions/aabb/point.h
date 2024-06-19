#pragma once

#include "base.h"

namespace utils::math::geometry::interactions
	{
	utils_gpu_available constexpr shape::point closest_point(const shape::concepts::aabb auto& aabb, const vec2f& point) noexcept
		{
		if (point.x() <= aabb.ll())
			{
			if (point.y() <= aabb.up()) { return aabb.ul(); }
			if (point.y() >= aabb.dw()) { return aabb.dl(); }
			return {aabb.ll(), point.y()};
			}
		else if (point.x() >= aabb.rr())
			{
			if (point.y() <= aabb.up()) { return aabb.ur(); }
			if (point.y() >= aabb.dw()) { return aabb.dr(); }
			return {aabb.rr(), point.y()};
			}
		else if (point.y() <= aabb.up()) 
			{
			return {point.x(), aabb.up()};
			}
		else if (point.y() >= aabb.dw())
			{
			return {point.x(), aabb.dw()};
			}

		const float distance_ll{std::abs(point.x() - aabb.ll())};
		const float distance_rr{std::abs(point.x() - aabb.rr())};
		const float distance_up{std::abs(point.y() - aabb.up())};
		const float distance_dw{std::abs(point.y() - aabb.dw())};
		const float min_distance_horizontal{std::min(distance_ll, distance_rr)};
		const float min_distance_vertical  {std::min(distance_up, distance_dw)};

		if (min_distance_horizontal < min_distance_vertical)
			{
			return {distance_ll < distance_rr ? aabb.ll() : aabb.rr(), point.y()};
			}
		else
			{
			return {point.x(), distance_up < distance_dw ? aabb.up() : aabb.dw()};
			}
		}

	utils_gpu_available constexpr float minimum_distance(const shape::concepts::aabb auto& aabb, const vec2f& point) noexcept
		{
		return interactions::signed_distance(aabb, point).absolute();
		}

	utils_gpu_available constexpr return_types::side side(const shape::concepts::aabb auto& aabb, const vec2f& point) noexcept
		{
		if (point.x() > aabb.ll() && point.x() < aabb.rr() && point.y() > aabb.up() && point.y() < aabb.dw()) { return return_types::side::create::outside(); }
		if (point.x() < aabb.ll() || point.x() > aabb.rr() || point.y() < aabb.up() || point.y() > aabb.dw()) { return return_types::side::create::inside (); }
		return return_types::side::create::coincident();
		}

	utils_gpu_available constexpr return_types::signed_distance signed_distance(const shape::concepts::aabb auto& aabb, const vec2f& point) noexcept
		{
		const vec2f point_from_center_ur_quadrant{utils::math::abs(point - aabb.centre())};
		const vec2f corner_from_center{aabb.ur() - aabb.centre()};
		const vec2f distances{point_from_center_ur_quadrant - corner_from_center};
		return {utils::math::max(distances, {0.f}).get_length() + utils::math::min(utils::math::max(distances.x(), distances.y()), 0.f)};
		}

	utils_gpu_available constexpr return_types::closest_point_with_distance closest_with_distance(const shape::concepts::aabb auto& aabb, const vec2f& point) noexcept
		{
		const auto closest{closest_point(aabb, point)};
		return {closest, interactions::minimum_distance(closest, point)};
		}

	utils_gpu_available constexpr return_types::closest_point_with_signed_distance closest_with_signed_distance(const shape::concepts::aabb auto& aabb, const vec2f& point) noexcept
		{
		const auto closest{interactions::closest_point(aabb, point)};
		return {closest, interactions::minimum_distance(closest, point) * interactions::side(aabb, point)};
		}
	}